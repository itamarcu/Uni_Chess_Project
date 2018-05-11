#include "Auxiliary.h"

void println_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    setbuf(stdout, 0);
    vfprintf(stdout, format, args);
    fprintf(stdout, "%s", "\n");

    va_end(args);
}

void println_output(const char *format, ...) {
    va_list args;
    va_start(args, format);
    setbuf(stdout, 0);
    vfprintf(stdout, format, args);
    fprintf(stdout, "%s", "\n");

    va_end(args);
}


void println_error_weak(const char *format, ...) {
    va_list args;
    va_start(args, format);
    setbuf(stdout, 0);
    vfprintf(stdout, format, args);
    fprintf(stdout, "%s", "\n");

    va_end(args);
}

void println_debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    setbuf(stdout, 0);
    if (DEBUG_MODE) {
        vfprintf(stdout, format, args);
        fprintf(stdout, "%s", "\n");
    }

    va_end(args);
}

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
