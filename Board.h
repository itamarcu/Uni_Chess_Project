
#ifndef UNI_CHESS_PROJECT_BOARD_H
#define UNI_CHESS_PROJECT_BOARD_H


#include <stdlib.h>
#include <vss.h>

/**
 * An 8x8 grid of characters. Rows 1-8, columns A-H.
 * Black on top (rows 7 and 8), white on bottom.
 * White pieces are lowercase, white pieces are uppercase.
 * R = Rook, K = King, Q = Queen, B = Bishop,
 * N = kNight, M = pawn (no idea why).
 * for example: a white knight would be 'n'.
 */
typedef struct board_type
{
    char grid[8][8]; //first row then column. grid[0] is the first row (Row 1).
} Board;


Board* make_starting_board();

void print_board(Board *board);

void free_board(Board *board);

bool is_empty_space(char piece);
bool is_white_piece(char piece);

#endif //UNI_CHESS_PROJECT_BOARD_H
