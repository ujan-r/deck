#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void exitWithError(char const *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
