#include "debug.h"

#include "lexer.h"
#include "parser.h"

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
        error("not yet implemented");
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

    clock_t start_time, end_time;
    start_time = clock();

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
    // thus no need to print it again here
    if (!tokens) {
        free(buf);
        return 1;
    }
    free(buf);

    end_time = clock();

    double time_elapsed =
        ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    char lexing_msg[64];
    sprintf(lexing_msg, "lexing done in %.3lf ms", time_elapsed);
    success(lexing_msg);

    // print out the tokens
    /* for (int i = 0; tokens[i - 1].type != TT_EOF; i++) { */
    /*     printf("%s: \"%s\"\n", ttostr(tokens[i].type), tokens[i].lexeme); */
    /* } */

    start_time = clock();

    Parser parser = parser_new(tokens);

    Stmt *stmts = parser.parse(&parser);
    // if it fails, it returns NULL, and the error message is printed
    // thus no need to print it again here
    if (!stmts) {
        free_tokens(tokens);
        return 1;
    }

    end_time = clock();
    time_elapsed = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    char parsing_msg[64];
    sprintf(parsing_msg, "parsing done in %.3lf ms", time_elapsed);
    success(parsing_msg);

    free(stmts);
    free_tokens(tokens);

    return 0;
}
