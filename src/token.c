#include "token.h"

#include <stddef.h>
#include <stdlib.h>

char *ttostr(TokenType t) {
    switch (t) {
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
    case TT_EOF:
        return "EOF";
    default:
        return NULL;
    }
}
