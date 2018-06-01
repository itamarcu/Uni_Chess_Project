
#ifndef UNI_CHESS_PROJECT_AUXILIARY_H
#define UNI_CHESS_PROJECT_AUXILIARY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define NULL_CHAR '\0'

#define WHITE 1
#define BLACK 2

#if defined(WIN32) || defined(_WIN32)
#define SEP "\\"
#else
#define SEP "/"
#endif

#define PICTURES_DIRECTORY "pictures"

bool DEBUG_MODE;
bool NO_MESSAGE_BOX_MODE;
bool SORT_POSSIBLE_MOVES;

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

/**
 * just like printf, but prints to stderr, and ends with a newline
 */
void println_error(const char *format, ...);

/**
 * just like printf, but ends with a newline
 */
void println_output(const char *format, ...);

/**
 * just like println_output. Used for expected error messages (e.g. invalid input)
 */
void println_error_weak(const char *format, ...);

/**
 * just like println_output, but only activates if DEBUG_MODE is true.
 */
void println_debug(const char *format, ...);

/**
 * Parses a string into an integer value, returns success value.
 * Uses base 10 (but if you want a different base you can edit this code or whatever).
 * Should work on numbers of these format examples: 1234, +1234, -1234, 001234
 * @param str pointer to string that contains string representation (of a number, hopefully)
 * @param int_ptr pointer to int that will be filled with result if possible
 * @return true (1) if succeeded, false (0) if failed
 */
bool cast_str_to_int(const char *str, int *int_ptr);

/**
 * Signs of a number. Useful for comparisons: sign(x1-x2) == 1 if x1>x2, 0 if x1=x2, -1 if x1<x2
 *
 * sign(3) == sign(17) == 1
 * sign(0) == sign(-0) == 0
 * sign(-7) == sign(-1) == -1
 * @param x
 * @return sign of x
 */
int sign(int x);

#endif //UNI_CHESS_PROJECT_AUXILIARY_H
