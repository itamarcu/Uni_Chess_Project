#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Auxiliary.h"

void println_error(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vfprintf(stderr, format, args);
    fprintf(stderr, "%s", "\n");

    va_end(args);
}

void println_output(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vfprintf(stdout, format, args);
    fprintf(stdout, "%s", "\n");

    va_end(args);
}

void println_debug(const char *format, ...) {
    va_list args;
    va_start(args, format);

    if (DEBUG_MODE) {
        vfprintf(stderr, format, args);
        fprintf(stderr, "%s", "\n");
    }

    va_end(args);
}

bool cast_str_to_int(const char *str, int *int_ptr) {
    char *temp;
    *int_ptr = strtol(str, &temp, 10); /*base 10*/
    println_debug("&temp temp *temp:    %d    %d    %d", &temp, temp, *temp);
    return temp == NULL;
}


char *difficulty_int_to_string(int difficulty) {
    if (difficulty == 1) {
        return "amateur";
    }
    if (difficulty == 2) {
        return "easy";
    }
    if (difficulty == 3) {
        return "moderate";
    }
    if (difficulty == 4) {
        return "hard";
    }
    if (difficulty == 5) {
        return "expert";
    }
}

int sign(int x) {
    return (x > 0) - (x < 0);
}