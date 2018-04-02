
#ifndef UNI_CHESS_PROJECT_AUXILIARY_H
#define UNI_CHESS_PROJECT_AUXILIARY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define DEBUG_MODE 1
#define NULL_CHAR '\0'

#define WHITE 1
#define BLACK 2

typedef enum {
    SUCCESS,
    INVALID_POS,
    NO_PIECE_IN_LOCATION,
    ILLEGAL_MOVE,
    KING_STILL_THREATENED,
    KING_WILL_BE_THREATENED,
    CANT_SAVE_FILE,
    EMPTY_HISTORY

} GAME_ACTION_RESULT;

void println_error(const char *format, ...);

void println_output(const char *format, ...);

void println_debug(const char *format, ...);

bool cast_str_to_int(const char *str, int *int_ptr);

int sign(int x);

int min(int n1, int n2);

int max(int n1, int n2);

#endif //UNI_CHESS_PROJECT_AUXILIARY_H
