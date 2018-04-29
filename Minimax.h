#ifndef UNI_CHESS_PROJECT_MINIMAX_H
#define UNI_CHESS_PROJECT_MINIMAX_H

#include "ChessGameSettings.h"

void init_scoring_function();

typedef struct computer_move_t {
    int r1, c1, r2, c2;
    char moving_piece;
} ComputerMove;

/**
 * Makes a move for the computer (updates the game board, too)
 * @return the move that was made
 */
ComputerMove computer_move(game_t *game);

ComputerMove recursively_minimax_best_move(board_t *board, bool player_is_white, int board_score);

/**
 * @return calculated score sum, without checking for draws/checkmates
 */
int calculate_simple_board_score(board_t *board);


#endif //UNI_CHESS_PROJECT_MINIMAX_H
