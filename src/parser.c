#include "debug.h"
#include "lexer.h"
#include "parser.h"

#include <stdlib.h>

#define INITIAL_STMTS 4
#define GROW_FACTOR 2

Stmt parse_exit(Parser *self) {
    (void)self;
    return (Stmt){0};
}

static Stmt parse_stmt(Parser *self) {
    Token token = self->tokens[self->current];
    switch (token.type) {
    case TT_EXIT:
        return parse_exit(self);
    default:
        error("not yet implemented");
        return (Stmt){0};
    }

    return (Stmt){0};
}

static Stmt *parser_parse(Parser *self) {
    int capacity = INITIAL_STMTS;
    Stmt *stmts = malloc(sizeof(Stmt) * capacity);
    if (!stmts) {
        error("failed to allocate memory");
        return NULL;
    }

    int i = 0;
    do {
        if (i >= capacity) {
            capacity *= GROW_FACTOR;
            Stmt *temp = realloc(stmts, sizeof(Stmt) * capacity);
            if (!temp) {
                error("failed to allocate memory");
                free(stmts);
                return NULL;
            }
            stmts = temp;
        }
        stmts[i] = parse_stmt(self);
        i++;
    } while (i < MAX_STMTS && self->tokens[self->current].type != TT_EOF);

    Stmt *temp = realloc(stmts, sizeof(Stmt) * (i + 1));
    if (!temp) {
        error("failed to allocate memory");
        free(stmts);
        return NULL;
    }
    stmts = temp;

    return stmts;
}

Parser parser_new(Token *tokens) {
    return (Parser){
        .tokens = tokens,
        .current = 0,

        .parse = parser_parse,
    };
}
