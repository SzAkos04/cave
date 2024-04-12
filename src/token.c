#include "token.h"

#include <stdbool.h>

#include <stdio.h>

bool is_type(Token t) { return t.type == TT_I32 || t.type == TT_VOID; }

bool is_unary_op(Token t) { return t.type == TT_BANG || t.type == TT_MINUS; }

char *ttostr(Token t) {
    switch (t.type) {
    // single character tokens
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
    case TT_COLON:
        return "COLON";
    // one or two character tokens
    case TT_BANG:
        return "BANG";
    case TT_BANG_EQUAL:
        return "BANG_EQUAL";
    case TT_EQUAL:
        return "EQUAL";
    case TT_EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case TT_GREATER:
        return "GREATER";
    case TT_GREATER_EQUAL:
        return "GREATER_EQUAL";
    case TT_LESS:
        return "LESS";
    case TT_LESS_EQUAL:
        return "LESS_EQUAL";
    case TT_PLUS:
        return "PLUS";
    case TT_PLUS_EQUAL:
        return "PLUS_EQUAL";
    case TT_MINUS:
        return "MINUS";
    case TT_MINUS_EQUAL:
        return "MINUS_EQUAL";
    case TT_SLASH:
        return "SLASH";
    case TT_SLASH_EQUAL:
        return "SLASH_EQUAL";
    case TT_STAR:
        return "STAR";
    case TT_STAR_EQUAL:
        return "STAR_EQUAL";
    case TT_MODULO:
        return "MODULO";
    case TT_MODULO_EQUAL:
        return "MODULO_EQUAL";
    // two character tokens
    case TT_AND:
        return "AND";
    case TT_OR:
        return "OR";
    // literals
    case TT_IDENTIFIER:
        return "IDENTIFIER";
    case TT_NUMBER:
        return "NUMBER";
    // keywords
    case TT_CONST:
        return "CONST";
    case TT_FN:
        return "FN";
    case TT_RETURN:
        return "RETURN";
    // types
    case TT_VOID:
        return "VOID";
    case TT_I32:
        return "I32";

    case TT_COMMENT:
        return "COMMENT";

    case TT_EOF:
        return "EOF";
    default:
        return "UNKNOWN";
    }
}

void print_token(Token token) {
    printf("%s: \"%s\"\n", ttostr(token), token.lexeme);
}
