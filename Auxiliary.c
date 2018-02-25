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

/**
 * Parses a string into an integer value, returns success value.
 * Uses base 10 (but if you want a different base you can edit this code or whatever).
 * Should work on numbers of these format examples: 1234, +1234, -1234, 001234
 * @param str pointer to string that contains string representation (of a number, hopefully)
 * @param int_ptr pointer to int that will be filled with result if possible
 * @return true (1) if succeeded, false (0) if failed
 */
bool cast_str_to_int(const char *str, int *int_ptr) {
    /*
     * the reason this uses a weird pointer-of-pointer-of-char to test for NULL (0)...
     * ...is that any sane person who works on C code is a moron
     */
    char *temp;
    *int_ptr = strtol(str, &temp, 10);
    return *temp == 0; //should be NULL be the build warns about it (justifiably)
}

int sign(int x) {
    return (x > 0) - (x < 0);
}