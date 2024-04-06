#include "debug.h"
#include "parser.h"
#include "stmt.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 32

#define INITIAL_STMTS 4
#define GROW_FACTOR 2

static Stmt parse_stmt(Parser *self);

/* static Literal parse_literal(Parser *self) { */
/*     Token token = self->tokens[self->current]; */
/*     Literal literal; */

/*     switch (token.type) { */
/*     case TT_NUMBER: */
/*         literal.Integer = strtol(token.lexeme, NULL, 10); */
/*         break; */
/*     default: */
/*         error("parsing literals not yet implemented"); */
/*         literal.null = NULL; */
/*     } */

/*     return literal; */
/* } */

static Stmt parse_fn(Parser *self) {
    char error_msg[64];
    if (self->tokens[self->current].type != TT_FN) {
        sprintf(error_msg, "expected `fn` keyword at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current]));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `fn`

    if (self->tokens[self->current].type != TT_IDENTIFIER) {
        sprintf(error_msg, "expected identifier at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current]));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    char *name = self->tokens[self->current].lexeme;
    self->current++; // consume identifier

    if (self->tokens[self->current].type != TT_LEFT_PAREN) {
        sprintf(error_msg, "expected `(` at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current]));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `(`

    // TODO: Parse function arguments
    while (self->tokens[self->current].type != TT_RIGHT_PAREN) {
        self->current++;
    }

    if (self->tokens[self->current].type != TT_RIGHT_PAREN) {
        sprintf(error_msg, "expected `)` at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current]));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `)`

    Token ret_type = (Token){.type = TT_VOID,
                             .lexeme = NULL,
                             .line = self->tokens[self->current].line};
    if (is_type(self->tokens[self->current])) {
        ret_type = self->tokens[self->current];
        self->current++; // consume type

        if (self->tokens[self->current].type != TT_LEFT_BRACE) {
            sprintf(error_msg, "expected `{` at line %i, found %s",
                    self->tokens[self->current].line,
                    ttostr(self->tokens[self->current]));
            error(error_msg);
            return (Stmt){.type = STMT_ERR};
        }
    } else if (self->tokens[self->current].type != TT_LEFT_BRACE) {
        sprintf(error_msg, "expected `{` at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current]));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `{`

    Stmt stmts[MAX_STMTS]; // TODO: change this
    int n;
    for (n = 0; self->tokens[self->current].type != TT_RIGHT_BRACE; n++) {
        stmts[n] = parse_stmt(self);
        if (stmts[n].type == STMT_ERR) {
            return (Stmt){.type = STMT_ERR};
        }
    }

    if (self->tokens[self->current].type != TT_RIGHT_BRACE) {
        sprintf(error_msg, "expected `}` at line %i, found %s",
                self->tokens[self->current].line,
                ttostr(self->tokens[self->current]));
        error(error_msg);
        return (Stmt){.type = STMT_ERR};
    }
    self->current++; // consume `}`

    return (Stmt){.type = STMT_FN,
                  .data.Fn = {
                      .name = name,
                      .args = NULL,
                      .arg_n = 0,
                      .ret_type = ret_type,
                      .stmts = stmts,
                      .stmt_n = n,
                  }};
}

static Stmt parse_stmt(Parser *self) {
    Token token = self->tokens[self->current];
    switch (token.type) {
    case TT_FN:
        return parse_fn(self);
    case TT_EOF:
        return (Stmt){.type = STMT_EOF};
    default:
        error("other tokens are not yet implemented");
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
        if (stmts[i].type == STMT_ERR) {
            free(stmts);
            return NULL;
        }
        i++;
    } while (i < MAX_STMTS && stmts[i - 1].type != STMT_EOF);

    // print out the statement types
#if DEBUG
    for (int j = 0; j < i; j++) {
        print_stmt(stmts[j]);
    }
#endif

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
