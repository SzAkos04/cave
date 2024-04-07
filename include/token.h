#pragma once

#include <stdbool.h>

#define MAX_TOKENS 2048

#define MAX_TOKEN_LEN 1024

typedef enum {
    // single character tokens
    TT_LEFT_PAREN,  // `(`
    TT_RIGHT_PAREN, // `)`
    TT_LEFT_BRACE,  // `{`
    TT_RIGHT_BRACE, // `}`
    TT_SEMICOLON,   // `;`
    TT_COLON,       // `:`

    // one or two character tokens
    TT_BANG,          // `!`
    TT_BANG_EQUAL,    // `!=`
    TT_EQUAL,         // `=`
    TT_EQUAL_EQUAL,   // `==`
    TT_GREATER,       // `>`
    TT_GREATER_EQUAL, // `>=`
    TT_LESS,          // `<`
    TT_LESS_EQUAL,    // `<=`
    TT_PLUS,          // `+`
    TT_PLUS_EQUAL,    // `+=`
    TT_MINUS,         // `-`
    TT_MINUS_EQUAL,   // `-=`
    TT_SLASH,         // `/`
    TT_SLASH_EQUAL,   // `/=`
    TT_STAR,          // `*`
    TT_STAR_EQUAL,    // `*=`
    TT_MODULO,        // `%`
    TT_MODULO_EQUAL,  // `%=`

    // two character tokens
    TT_AND, // `&&`
    TT_OR,  // `||`

    // literals
    TT_IDENTIFIER,
    TT_NUMBER,

    // keywords
    TT_CONST,  // `const`
    TT_FN,     // `fn`
    TT_RETURN, // `return`

    // types
    TT_VOID, // `void`
    TT_I32,  // `i32`

    TT_COMMENT,

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
