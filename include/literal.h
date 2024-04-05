#pragma once

#include <stdbool.h>

typedef enum {
    LITERAL_STRING,
    LITERAL_INTEGER,
    LITERAL_FLOAT,
    LITERAL_BOOLEAN,
    LITERAL_IDENTIFIER,
    LITERAL_NULL,
} LiteralType;

typedef struct {
    LiteralType type;
    union {
        char *Str;
        long int Integer;
        double Float;
        bool Boolean;
        char *Identifier;
        void *null;
    } data;
} Literal;

char *literal_to_str(Literal);
