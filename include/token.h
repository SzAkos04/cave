#pragma once

#define MAX_TOKENS 2048

#define MAX_TOKEN_LEN 1024

typedef enum {
    // single character tokens
    TT_LEFT_PAREN,
    TT_RIGHT_PAREN,
    TT_LEFT_BRACE,
    TT_RIGHT_BRACE,
    TT_SEMICOLON,

    // literals
    TT_IDENTIFIER,
    TT_NUMBER,

    // keywords
    TT_FN,

    TT_EOF,
    TT_UNKNOWN,
} TokenType;

char *ttostr(TokenType);

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
} Token;

void print_token(Token);
