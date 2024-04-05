#include "expr.h"
#include "stmt.h"

#include <stdio.h>

void print_stmt(Stmt stmt) {
    switch (stmt.type) {
    case STMT_EXPR:
        printf("EXPR: %s\n", expr_to_str(stmt.data.Expression));
        break;
    case STMT_BLOCK:
        printf("BLOCK:\n");
        for (int i = 0; i < stmt.data.Block.stmt_n; i++) {
            print_stmt(stmt.data.Block.stmts[i]);
        }
        break;
    case STMT_FN:
        printf("FN %s:\n", stmt.data.Fn.name);
        for (int i = 0; i < stmt.data.Fn.stmt_n; i++) {
            print_stmt(stmt.data.Fn.stmts[i]);
        }
        break;
    case STMT_EOF:
        printf("EOF: (null)\n");
        break;
    default:
        printf("ERROR\n");
        break;
    }
}
