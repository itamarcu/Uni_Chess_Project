
#ifndef UNI_CHESS_PROJECT_BOARD_H
#define UNI_CHESS_PROJECT_BOARD_H

#define EMPTY_SPACE '_'
#define WHITE_PAWN 'm'
#define WHITE_BISHOP 'b'
#define WHITE_ROOK 'r'
#define WHITE_KNIGHT 'n'
#define WHITE_QUEEN 'q'
#define WHITE_KING 'k'
#define BLACK_PAWN 'M'
#define BLACK_BISHOP 'B'
#define BLACK_ROOK 'R'
#define BLACK_KNIGHT 'N'
#define BLACK_QUEEN 'Q'
#define BLACK_KING 'K'


#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * An 8x8 grid of characters. Rows 1-8, columns A-H.
 * Black on top (rows 7 and 8), white on bottom.
 * White pieces are lowercase, black pieces are uppercase.
 * R = Rook, K = King, Q = Queen, B = Bishop,
 * N = kNight, M = pawn (no idea why).
 * for example: a white knight would be 'n'.
 */

typedef struct board_t {
    char grid[8][8]; //first row then column. grid[0] is the first row (Row 1).
} Board;

/**
 * Creates a starting chess board (with all pieces) and returns a pointer to it.
 *
 * Pointer should be freed by the function caller.
 */
Board *make_starting_board();

/**
 * Creates a new chess board as a deep copy of another chess board, and returns a pointer to it.
 *
 * Pointer should be freed by the function caller.
 */
Board *copy_board(Board *board);

/**
 * Frees a board pointer.
 */
void free_board(Board *board);

/**
 * Returns true if piece is EMPTY_SPACE
 */
bool is_empty_space(char piece);

/**
 * Returns true if piece is WHITE_???.
 *
 * Returns false if piece is BLACK_???, EMPTY_SPACE, or anything unspecified
 */
bool is_white_piece(char piece);

/**
 * Returns name of piece, without color.
 *
 * e.g. name_of_piece(BLACK_BISHOP) == "bishop"
 */
const char *name_of_piece(char piece);

/**
 * Returns true unless the character is undefined (not BLACK_???, WHITE_??? or EMPTY_SPACE)
 */
bool is_piece_or_empty_space(char c);

#endif //UNI_CHESS_PROJECT_BOARD_H
