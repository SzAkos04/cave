#pragma once

#include "expr.h"
#include "literal.h"
#include "token.h"

typedef struct Stmt {
    enum {
        STMT_EXPR,
        STMT_BLOCK,
        STMT_CONST,
        STMT_FN,
        STMT_RETURN,
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
        struct {
            Literal name;
            enum {
                CONST_I32,
            } type;
            Expr value;
        } Const;
        struct {
            Expr value;
        } Return;
    } data;
} Stmt;
