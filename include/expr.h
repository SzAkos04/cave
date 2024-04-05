#pragma once

#include "literal.h"
#include "token.h"

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

char *expr_to_str(Expr);
