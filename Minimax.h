#ifndef UNI_CHESS_PROJECT_MINIMAX_H
#define UNI_CHESS_PROJECT_MINIMAX_H

#include "ChessGameSettings.h"

typedef struct computer_move_t {
    int r1, c1, r2, c2;
    char moving_piece;
} ComputerMove;

/**
 * Makes a move for the computer (updates the game board, too)
 * @return the move that was made
 */
ComputerMove computer_move(game_t *game);

#endif //UNI_CHESS_PROJECT_MINIMAX_H
