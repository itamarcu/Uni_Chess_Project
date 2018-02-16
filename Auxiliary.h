
#ifndef UNI_CHESS_PROJECT_AUXILIARY_H
#define UNI_CHESS_PROJECT_AUXILIARY_H

#include "Game.h"


#define DEBUG_MODE 1

#define CAST_STR_TO_INT_OR_ERROR_AND_RETURN(var_name, str_name) do { \
    char *temp; \
    (var_name) = strtol((str_name), &temp, 10); \
    if (*temp != NULL) \
    { \
        println_error("Error: invalid command"); \
        return; \
    } \
} while (0);
#define NULL_CHAR '\0'

/**
 * Read user input in the form of "string arg1 arg2 arg3...".
 * Returns NULL if invalid input
 */
Command *get_user_input_as_command();
void println_error(const char *format, ...);
void println_output(const char *format, ...);
void println_debug(const char *format, ...);

#endif //UNI_CHESS_PROJECT_AUXILIARY_H
