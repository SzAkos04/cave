#pragma once

#include "expr.h"
#include "token.h"

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
        struct {
            struct Stmt *stmts;
            int stmt_n;
        } Block;
        struct {
            char *name;
            struct {
                Token type;
                Token name;
            } *args;
            int arg_n;
            Token ret_type;
            struct Stmt *stmts;
            int stmt_n;
        } Fn;
    } data;
} Stmt;

void print_stmt(Stmt);
