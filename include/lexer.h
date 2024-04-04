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

char *ttostr(TokenType t);

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
} Token;

typedef struct Lexer {
    const char *buf;
    int start;
    int current;
    int line;

    Token *(*lex)(struct Lexer *);
} Lexer;

Lexer lexer_new(const char *buf);
void free_tokens(Token *tokens);
