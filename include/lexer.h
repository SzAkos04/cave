#pragma once

#include "token.h"

typedef struct Lexer {
    const char *buf;
    int current;
    int line;

    Token *(*lex)(struct Lexer *);
} Lexer;

Lexer lexer_new(const char *buf);
void free_tokens(Token *tokens);
