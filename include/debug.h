#pragma once

#include <stdio.h>

/* #define DEBUG 1 */

#ifdef __clang__
// Check if optimization flags are enabled
#if defined(__OPTIMIZE__) || defined(__OPTIMIZE_SIZE__)
#define DEBUG 0
#else
#define DEBUG 1
#endif
#else
// Not using clang
#define DEBUG 0
#endif

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
