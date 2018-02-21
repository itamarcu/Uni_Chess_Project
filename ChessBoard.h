
#ifndef UNI_CHESS_PROJECT_BOARD_H
#define UNI_CHESS_PROJECT_BOARD_H

#define EMPTY_ENTRY '_'
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
#include "Auxiliary.h"

/**
 * An 8x8 grid of characters. Rows 1-8, columns A-H.
 * Black on top (rows 7 and 8), white on bottom.
 * White pieces are lowercase, white pieces are uppercase.
 * R = Rook, K = King, Q = Queen, B = Bishop,
 * N = kNight, M = pawn (no idea why).
 * for example: a white knight would be 'n'.
 */

typedef struct board_type {
    char grid[8][8]; //first row then column. grid[0] is the first row (Row 1).
} Board;

Board* make_starting_board();

void free_board(Board *board);

bool is_empty_space(char piece);
bool is_white_piece(char piece);
/**
 * Returns false if either iece is empty space, or if they are different color (black and white)
 */
bool are_same_color(char piece1, char piece2);
#endif //UNI_CHESS_PROJECT_BOARD_H
