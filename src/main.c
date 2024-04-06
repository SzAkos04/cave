#include "debug.h"

#include "codegen.h"
#include "lexer.h"
#include "parser.h"
#include "stmt.h"

#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        error("no input files");
        return 1;
    } else if (argc > 2) {
        error("more arguments are not yet implemented");
        return 1;
    }

    char *path = argv[1];
    char *ext = strchr(path, '.');
    if (!ext) {
        error("file has no extension");
        return 1;
    }
    ext++; // skip the '.' character

    if (strcmp(ext, "cv") != 0) {
        error("file extention not recognised");
        return 1;
    }

    if (access(path, F_OK | R_OK) != 0) {
        error("file does not exist");
        return 1;
    }

    clock_t start_time = clock();

    FILE *fs = fopen(path, "r");
    if (!fs) {
        error("failed to open file");
        return 1;
    }

    // get the size of the file
    fseek(fs, 0, SEEK_END);
    long file_size = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    char *buf = (char *)malloc(sizeof(char) * (file_size + 1));
    if (!buf) {
        error("failed to allocate memory");
        fclose(fs);
        return 1;
    }

    size_t files_read = fread(buf, sizeof(char), file_size, fs);
    if (files_read != (size_t)file_size) {
        error("failed to read file");
        fclose(fs);
        free(buf);
        return 1;
    }
    buf[file_size] = '\0';

    fclose(fs);

    Lexer lexer = lexer_new(buf);

    Token *tokens = lexer.lex(&lexer);
    // if it fails, it returns NULL, and the error message is printed
    if (!tokens) {
        free(buf);
        return 1;
    }
    free(buf);

#if DEBUG
    char lexing_msg[64];
    sprintf(lexing_msg, "lexing done in %.3lfms",
            ((double)(clock() - start_time)) / CLOCKS_PER_SEC * 1000);
    success(lexing_msg);

    clock_t parse_start_time = clock();
#endif

    Parser parser = parser_new(tokens);

    Stmt *stmts = parser.parse(&parser);
    // if it fails, it returns NULL, and the error message is printed
    if (!stmts) {
        free_tokens(tokens);
        return 1;
    }

    free_tokens(tokens);

#if DEBUG
    char parsing_msg[64];
    sprintf(parsing_msg, "parsing done in %.3lfms",
            ((double)(clock() - parse_start_time)) / CLOCKS_PER_SEC * 1000);
    success(parsing_msg);

    clock_t codegen_start_time = clock();
#endif

    // if it fails, it returns 1, and the error message is printed
    LLVMBackend backend = backend_new(stmts);
    if (backend.generate_IR(&backend) != 0) {
        free(stmts);
        free_backend(backend);
        return 1;
    }

    free(stmts);

#if DEBUG
    char IR_gen_msg[64];
    sprintf(IR_gen_msg, "IR generation done in %.3lfms",
            ((double)(clock() - codegen_start_time)) / CLOCKS_PER_SEC * 1000);
    success(IR_gen_msg);
#endif

    // write IR to temp
    if (LLVMWriteBitcodeToFile(backend.module, "temp.ll") != 0) {
        error("failed to write IR to file");
        free_backend(backend);
        return 1;
    }

    free_backend(backend);

    // compile IR code
    if (system("llc -filetype=obj temp.ll -o temp.o") != 0) {
        error("llc failed");
        remove("cave.ll");
        return 1;
    }
    remove("temp.ll");
    if (system("clang temp.o -o a.out") != 0) {
        error("clang failed");
        remove("cave.o");
        return 1;
    }
    remove("temp.o");

    char success[64];
    sprintf(success, "compilation done in %.3lfms",
            ((double)(clock() - start_time)) / CLOCKS_PER_SEC * 1000);
    success(success);

    return 0;
}
