#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 2048

int main(int argc, char **argv) {
    if (argc < 2) {
        error("no input files");
        return 1;
    } else if (argc > 2) {
        error("not yet implemented");
        return 1;
    }

    char *path = argv[1];
    char *ext = strchr(path, '.');
    if (!ext) {
        error("file has no extension");
        return 1;
    }
    ext++; // skip the '.' character

    if (strcmp(ext, "cv") != 0) {
        error("file extention not recognised");
        return 1;
    }

    if (access(path, F_OK | R_OK) != 0) {
        error("file does not exist");
        return 1;
    }

    char *buf = (char *)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    if (!buf) {
        error("failed to allocate memory");
        return 1;
    }

    FILE *fs = fopen(path, "r");
    if (!fs) {
        error("failed to open file");
        free(buf);
        return 1;
    }

    while (fgets(buf, MAX_BUFFER_SIZE, fs))
        ;

    fclose(fs);

    buf = realloc(buf, strlen(buf + 1));
    if (!buf) {
        error("failed to reallocate memory");
        return 1;
    }

    printf("contents:\n%s\n", buf);

    free(buf);

    return 0;
}
