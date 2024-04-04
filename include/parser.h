#pragma once

#include "lexer.h"

#include <stdbool.h>

#define MAX_STMTS 2048

typedef union {
    char *Str;
    long int Integer;
    double Float;
    bool Boolean;
    char *Identifier;
    void *null;
} Literal;

typedef struct Expr {
    enum {
        EXPR_UNARY,
        EXPR_BINARY,
        EXPR_LITERAL,
        EXPR_VARIABLE,
        EXPR_ASSIGNMENT,
    } type;
    union {
        struct {
            Token operator;
            struct Expr *right;
        } Unary;
        struct {
            struct Expr *left;
            Token operator;
            struct Expr *right;
        } Binary;
        Literal Literal;
        char *Variable;
        struct {
            Token left;
            Token operator;
            struct Expr *value;
        } Assignment;
    } data;
} Expr;

typedef struct Stmt {
    enum {
        STMT_EXPR,
        STMT_BLOCK,
        STMT_FN,
        STMT_EOF,
        STMT_ERR = 255,
    } type;
    union {
        Expr Expression;
        struct Stmt *Block;
        struct {
            char *name;
            Token *arguments;
            struct Stmt *stmts;
        } Fn;
    } data;
} Stmt;

typedef struct Parser {
    Token *tokens;
    int current;

    Stmt *(*parse)(struct Parser *);
} Parser;

Parser parser_new(Token *tokens);

char *stmttostr(Stmt);
