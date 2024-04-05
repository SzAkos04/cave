#include "debug.h"
#include "lexer.h"

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
    case '\0':
        return (Token){
            .type = TT_EOF,
            .lexeme = NULL,
            .line = self->line,
        };
    default: {
        if (isdigit(cur)) {
            self->start = self->current;
            while (isdigit(self->buf[self->current]) ||
                   (self->buf[self->current] == '.' &&
                    isdigit(self->buf[self->current + 1]))) {
                self->current++;
            }
            int len = self->current - self->start;
            char *lexeme = (char *)malloc(sizeof(char) * (len + 1));
            if (!lexeme) {
                error("failed to allocate memory");
                break;
            }
            strncpy(lexeme, self->buf + self->start, len);
            lexeme[len] = '\0';

            Token token = (Token){
                .type = TT_NUMBER,
                .lexeme = strdup(lexeme),
                .line = self->line,
            };

            free(lexeme);

            return token;
        } else if (isalpha(cur)) {
            self->start = self->current;
            while (isalpha(self->buf[self->current])) {
                self->current++;
            }
            int len = self->current - self->start;

            char *lexeme = malloc(sizeof(char) * (len + 1));
            if (!lexeme) {
                error("failed to allocate memory");
                break;
            }
            strncpy(lexeme, self->buf + self->start, len);
            lexeme[len] = '\0';

            TokenType tt;
            if (strcmp(lexeme, "fn") == 0) {
                tt = TT_FN;
            } else {
                tt = TT_IDENTIFIER;
            }

            Token token = (Token){
                .type = tt,
                .lexeme = strdup(lexeme),
                .line = self->line,
            };

            free(lexeme);

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

    // print out the tokens
    /* for (int i = 0; tokens[i - 1].type != TT_EOF; i++) { */
    /*     printf("%s: \"%s\"\n", ttostr(tokens[i].type), tokens[i].lexeme); */
    /* } */

    return tokens;
}

Lexer lexer_new(const char *buf) {
    return (Lexer){
        .buf = buf,
        .start = 0,
        .current = 0,
        .line = 1,

        .lex = lex,
    };
}

void free_tokens(Token *tokens) {
    for (int i = 0; tokens[i].type == TT_EOF; i++) {
        free(tokens[i].lexeme);
    }
    free(tokens);
}
