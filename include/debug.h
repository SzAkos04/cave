#pragma once

#include <stdio.h>

#define RESET "\033[;0m"
#define BOLD "\033[0;1m"
#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"

// Print error message to
#define error(msg)                                                             \
    do {                                                                       \
        fprintf(stderr, "%scave: %serror%s: %s\n", BOLD, BOLD_RED, RESET,      \
                msg);                                                          \
    } while (0)

#define success(msg)                                                           \
    do {                                                                       \
        printf("%scave: %ssuccess%s: %s\n", BOLD, BOLD_GREEN, RESET, msg);     \
    } while (0)
