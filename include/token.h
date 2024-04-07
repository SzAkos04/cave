#pragma once

#include <stdbool.h>

#define MAX_TOKENS 2048

#define MAX_TOKEN_LEN 1024

typedef enum {
    // single character tokens
    TT_LEFT_PAREN,
    TT_RIGHT_PAREN,
    TT_LEFT_BRACE,
    TT_RIGHT_BRACE,
    TT_SEMICOLON,
    TT_COLON,

    // one or two character tokens
    TT_EQUAL,

    // literals
    TT_IDENTIFIER,
    TT_NUMBER,

    // keywords
    TT_CONST,
    TT_FN,
    TT_RETURN,

    // types
    TT_VOID,
    TT_I32,

    TT_EOF,
    TT_UNKNOWN,
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
} Token;

bool is_type(Token);

char *ttostr(Token);

void print_token(Token);
