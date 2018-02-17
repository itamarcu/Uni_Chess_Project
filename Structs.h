
#ifndef UNI_CHESS_PROJECT_STRUCTS_H
#define UNI_CHESS_PROJECT_STRUCTS_H

#include <stdbool.h>

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

typedef struct game_type {
    int program_mode; // PROGRAM_MODE_*
    int state; // GAME_STATE_*
    int game_mode; // GAME_MODE_*
    int difficulty; // 1 .. 5
    int user_color; // USER_COLOR_*
    int current_user; // ^ ditto ^
    Board *board;
    int winner; // GAME_CURRENT_WINNER_*
} Game;

typedef struct command_type {
    char *cmd;
    char **args;
} Command;

typedef struct possible_move_type {
    int row, col;
    bool is_capturing;
    bool is_threatening;
} PossibleMove;


#endif //UNI_CHESS_PROJECT_STRUCTS_H
