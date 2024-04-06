#include "token.h"

#include <stdbool.h>

#include <stdio.h>

bool is_type(Token t) { return t.type == TT_I32 || t.type == TT_VOID; }

char *ttostr(Token t) {
    switch (t.type) {
    case TT_LEFT_PAREN:
        return "LEFT_PAREN";
    case TT_RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TT_LEFT_BRACE:
        return "LEFT_BRACE";
    case TT_RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TT_SEMICOLON:
        return "SEMICOLON";
    case TT_IDENTIFIER:
        return "IDENTIFIER";
    case TT_NUMBER:
        return "NUMBER";
    case TT_FN:
        return "FN";
    case TT_RETURN:
        return "RETURN";
    case TT_VOID:
        return "VOID";
    case TT_I32:
        return "I32";
    case TT_EOF:
        return "EOF";
    default:
        return NULL;
    }
}

void print_token(Token token) {
    printf("%s: \"%s\"\n", ttostr(token), token.lexeme);
}
