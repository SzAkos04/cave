#pragma once

#include "stmt.h"
#include "token.h"

#define MAX_STMTS 2048

typedef struct Parser {
    Token *tokens;
    int current;

    Stmt *(*parse)(struct Parser *);
} Parser;

Parser parser_new(Token *tokens);
