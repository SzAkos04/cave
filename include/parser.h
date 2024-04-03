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
        UNARY_EXPR,
        BINARY_EXPR,
        LITERAL_EXPR,
        VARIABLE_EXPR,
        ASSIGNMENT_EXPR,
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
        EXPR_STMT,
        BLOCK_STMT,
        EXIT_STMT,
    } type;
    union {
        Expr Expression;
        struct Stmt *Block;
        struct {
            Literal code;
        } Exit;
    } data;
} Stmt;

typedef struct Parser {
    Token *tokens;
    int current;

    Stmt *(*parse)(struct Parser *);
} Parser;

Parser parser_new(Token *tokens);
