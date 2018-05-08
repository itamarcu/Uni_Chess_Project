#ifndef UNI_CHESS_PROJECT_MINIMAX_H
#define UNI_CHESS_PROJECT_MINIMAX_H

#include "ChessGameSettings.h"

void init_scoring_function();

// +Positive score+ for WHITE
// -Negative score- for BLACK
#define MAX_SCORE_VALUE 1234567
#define MIN_SCORE_VALUE (-MAX_SCORE_VALUE)
#define SCORE_WHITE_CHECKMATE 1000
#define SCORE_BLACK_CHECKMATE (-1000)

typedef struct computer_move_t {
    int r1, c1, r2, c2;
    int score; // used for minimax only
} ComputerMove;

/**
 * Makes a move for the computer (updates the game board, too)
 * @return the move that was made
 */
ComputerMove computer_move(game_t *game);

ComputerMove
recursively_minimax_best_move(board_t *board, bool player_is_white, int alpha, int beta, int depthRemaining);

/**
 * @return calculated score sum, without checking for draws/checkmates
 */
int calculate_simple_board_score(board_t *board);


#endif //UNI_CHESS_PROJECT_MINIMAX_H
