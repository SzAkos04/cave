#include "debug.h"
#include "lexer.h"
#include "token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token next(Lexer *self) {
    char cur = self->buf[self->current];

    while (isspace(cur)) {
        if (cur == '\n') {
            self->line++;
        }
        self->current++;
        cur = self->buf[self->current];
    }

    switch (cur) {
    case '(':
        self->current++;
        return (Token){
            .type = TT_LEFT_PAREN,
            .lexeme = "(",
            .line = self->line,
        };
    case ')':
        self->current++;
        return (Token){
            .type = TT_RIGHT_PAREN,
            .lexeme = ")",
            .line = self->line,
        };
    case '{':
        self->current++;
        return (Token){
            .type = TT_LEFT_BRACE,
            .lexeme = "{",
            .line = self->line,
        };
    case '}':
        self->current++;
        return (Token){
            .type = TT_RIGHT_BRACE,
            .lexeme = "}",
            .line = self->line,
        };
    case ';':
        self->current++;
        return (Token){
            .type = TT_SEMICOLON,
            .lexeme = ";",
            .line = self->line,
        };
    case ':':
        self->current++;
        return (Token){
            .type = TT_COLON,
            .lexeme = ":",
            .line = self->line,
        };
    case '=':
        self->current++;
        return (Token){
            .type = TT_EQUAL,
            .lexeme = "=",
            .line = self->line,
        };
    case '\0':
        return (Token){
            .type = TT_EOF,
            .lexeme = NULL,
            .line = self->line,
        };
    default: {
        if (isdigit(cur)) {
            int start = self->current;
            while (isdigit(self->buf[self->current]) ||
                   (self->buf[self->current] == '.' &&
                    isdigit(self->buf[self->current + 1]))) {
                self->current++;
            }
            int len = self->current - start;
            char *lexeme = (char *)malloc(sizeof(char) * (len + 1));
            if (!lexeme) {
                error("failed to allocate memory");
                return (Token){.type = TT_UNKNOWN};
            }
            strncpy(lexeme, self->buf + start, len);
            lexeme[len] = '\0';

            Token token = (Token){
                .type = TT_NUMBER,
                .lexeme = lexeme,
                .line = self->line,
            };

            return token;
        } else if (isalpha(cur)) {
            int start = self->current;
            while (isalnum(self->buf[self->current])) {
                self->current++;
            }
            int len = self->current - start;

            char *lexeme = (char *)malloc(sizeof(char) * (len + 1));
            if (!lexeme) {
                error("failed to allocate memory");
                return (Token){.type = TT_UNKNOWN};
            }
            strncpy(lexeme, self->buf + start, len);
            lexeme[len] = '\0';

            TokenType tt = TT_IDENTIFIER;
            if (strcmp(lexeme, "const") == 0) {
                tt = TT_CONST;
            } else if (strcmp(lexeme, "fn") == 0) {
                tt = TT_FN;
            } else if (strcmp(lexeme, "return") == 0) {
                tt = TT_RETURN;
            } else if (strcmp(lexeme, "void") == 0) {
                tt = TT_VOID;
            } else if (strcmp(lexeme, "i32") == 0) {
                tt = TT_I32;
            }

            Token token = (Token){
                .type = tt,
                .lexeme = lexeme,
                .line = self->line,
            };

            return token;
        }
        break;
    }
    }

    self->current++;
    return (Token){
        .type = TT_UNKNOWN,
        .lexeme = NULL,
        .line = self->line,
    };
}

#define INITIAL_TOKENS 4
#define GROW_FACTOR 2

static Token *lex(Lexer *self) {
    int capacity = INITIAL_TOKENS;
    Token *tokens = malloc(sizeof(Token) * capacity);
    if (!tokens) {
        error("failed to allocate memory");
        return NULL;
    }

    int i = 0;
    do {
        if (i >= capacity) {
            capacity *= GROW_FACTOR;
            Token *temp = realloc(tokens, sizeof(Token) * capacity);
            if (!temp) {
                error("failed to allocate memory");
                free(tokens);
                return NULL;
            }
            tokens = temp;
        }
        tokens[i] = next(self);
        i++;
    } while (i < MAX_TOKENS && tokens[i].type != TT_EOF);

    Token *temp = realloc(tokens, sizeof(Token) * (i + 1));
    if (!temp) {
        error("failed to allocate memory");
        free(tokens);
        return NULL;
    }
    tokens = temp;

#if DEBUG
    // print out the tokens
    // -1 so it prints out the EOF token too
    for (int i = 0; tokens[i - 1].type != TT_EOF; i++) {
        print_token(tokens[i]);
    }
#endif

    return tokens;
}

Lexer lexer_new(const char *buf) {
    return (Lexer){
        .buf = buf,
        .current = 0,
        .line = 1,

        .lex = lex,
    };
}

void free_tokens(Token *tokens) {
    for (int i = 0; tokens[i].type == TT_EOF; i++) {
        if (tokens[i].type == TT_NUMBER || tokens[i].type == TT_FN ||
            tokens[i].type == TT_RETURN || tokens[i].type == TT_VOID ||
            tokens[i].type == TT_I32 || tokens[i].type == TT_IDENTIFIER) {
            free(tokens[i].lexeme);
        }
    }
    free(tokens);
}
