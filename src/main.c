#include <stdio.h>
#include <stdlib.h>

#include "present.h"

char * readFile(char const *filename);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *text = readFile(argv[1]);
    present(text);

    return 0;
}

char * readFile(char const *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "error: unable to open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = calloc(size + 1, sizeof *buffer);
    if (buffer == NULL) {
        fclose(file);
        fprintf(stderr, "error: unable to allocate buffer\n");
        exit(EXIT_FAILURE);
    }

    if (fread(buffer, 1, size, file) != size) {
        free(buffer);
        fclose(file);
        fprintf(stderr, "error: unable to read file into buffer\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);

    return buffer;
}
