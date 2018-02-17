
#ifndef UNI_CHESS_PROJECT_AUXILIARY_H
#define UNI_CHESS_PROJECT_AUXILIARY_H

#include <stdbool.h>

#define DEBUG_MODE 1
#define NULL_CHAR '\0'

void println_error(const char *format, ...);

void println_output(const char *format, ...);

void println_debug(const char *format, ...);

bool cast_str_to_int(const char *str, int *int_ptr);

char *difficulty_int_to_string(int difficulty);

int sign(int x);

#endif //UNI_CHESS_PROJECT_AUXILIARY_H
