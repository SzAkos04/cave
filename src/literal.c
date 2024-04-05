#include "literal.h"

#include <stdio.h>
#include <string.h>

char *literal_to_str(Literal literal) {
    switch (literal.type) {
    case LITERAL_STRING:
        return literal.data.Str;
    case LITERAL_INTEGER: {
        char str[16];
        sprintf(str, "%li", literal.data.Integer);
        return strdup(str);
    }
    case LITERAL_FLOAT: {
        char str[16];
        sprintf(str, "%lf", literal.data.Float);
        return strdup(str);
    }
    case LITERAL_BOOLEAN: {
        char str[16];
        sprintf(str, "%s", literal.data.Boolean ? "true" : "false");
        return strdup(str);
    }
    case LITERAL_IDENTIFIER:
        return literal.data.Identifier;
    case LITERAL_NULL:
        return "(null)";
    }
}
