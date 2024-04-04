#include "debug.h"
#include "lexer.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_STMTS 4
#define GROW_FACTOR 2

char *stmttostr(Stmt s) {
    switch (s.type) {
    case STMT_EXPR:
        return "EXPR";
    case STMT_BLOCK:
        return "BLOCK";
    case STMT_EXIT:
        return "EXIT";
    default:
        return "UNKNOWN";
    }
}

static Literal parse_literal(Parser *self) {
    Token token = self->tokens[self->current];
    Literal literal;

    switch (token.type) {
    case TT_NUMBER:
        literal.Integer = strtol(token.lexeme, NULL, 10);
        break;
    default:
        error("parsing literals not yet implemented");
        literal.null = NULL;
    }

    return literal;
}

static Stmt parse_exit(Parser *self) {
    char error_msg[64];
    if (self->tokens[self->current].type != TT_EXIT) {
        sprintf(error_msg, "expected `exit` keyword at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current].type));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `exit`

    if (self->tokens[self->current].type != TT_LEFT_PAREN) {
        sprintf(error_msg, "expected `(` at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current].type));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `(`

    Literal literal = parse_literal(self);
    self->current++; // consume literal

    if (self->tokens[self->current].type != TT_RIGHT_PAREN) {
        sprintf(error_msg, "expected `)` at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current].type));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `)`

    if (self->tokens[self->current].type != TT_SEMICOLON) {
        sprintf(error_msg, "expected `;` at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current].type));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `;`

    return (Stmt){
        .type = STMT_EXIT,
        .data.Exit.code = literal,
    };
}

static Stmt parse_stmt(Parser *self) {
    Token token = self->tokens[self->current];
    switch (token.type) {
    case TT_EXIT:
        return parse_exit(self);
    default:
        error("not yet implemented");
        return (Stmt){.type = STMT_ERR};
    }
}

static Stmt *parser_parse(Parser *self) {
    int capacity = INITIAL_STMTS;
    Stmt *stmts = malloc(sizeof(Stmt) * capacity);
    if (!stmts) {
        error("failed to allocate memory");
        return NULL;
    }

    int i = 0;
    while (i < MAX_STMTS && self->tokens[self->current].type != TT_EOF) {
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
        if (stmts[i].type == STMT_ERR) {
            free(stmts);
            return NULL;
        }
        i++;
    }

    // print out the statement types
    /* for (int j = 0; j < i; j++) { */
    /*     printf("%s\n", stmttostr(stmts[j])); */
    /* } */

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
