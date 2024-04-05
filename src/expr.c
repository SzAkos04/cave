#include "expr.h"
#include "literal.h"

#include <stdio.h>
#include <string.h>

char *expr_to_str(Expr expr) {
    switch (expr.type) {
    case EXPR_UNARY: {
        char str[128];
        sprintf(str, "EXPR_UNARY [\n");
        sprintf(str, "\toperator: %s\n", expr.data.Unary.operator.lexeme);
        sprintf(str, "\tright: %s\n", expr_to_str(*expr.data.Unary.right));
        sprintf(str, "]");

        return strdup(str);
    }
    case EXPR_BINARY: {
        char str[128];
        sprintf(str, "EXPR_BINARY [\n");
        sprintf(str, "\tleft: %s\n", expr_to_str(*expr.data.Binary.left));
        sprintf(str, "\toperator: %s\n", expr.data.Binary.operator.lexeme);
        sprintf(str, "\tright: %s\n", expr_to_str(*expr.data.Binary.right));
        sprintf(str, "]");

        return strdup(str);
    }
    case EXPR_LITERAL:
        return literal_to_str(expr.data.Literal);
    case EXPR_VARIABLE: {
        char str[128];
        sprintf(str, "EXPR_VARIABLE: %s", expr.data.Variable);
        return strdup(str);
    }
    case EXPR_ASSIGNMENT: {
        char str[128];
        sprintf(str, "EXPR_ASSIGNMENT [\n");
        sprintf(str, "\tleft: %s\n", expr.data.Assignment.left.lexeme);
        sprintf(str, "\toperator: %s\n", expr.data.Assignment.operator.lexeme);
        sprintf(str, "\tvalue: %s\n", expr_to_str(*expr.data.Assignment.value));
        sprintf(str, "]");
        return strdup(str);
    }
    }
}
