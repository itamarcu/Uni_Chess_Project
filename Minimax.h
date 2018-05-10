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
 * Makes a "best" move for the computer (updates the game board, too).
 *
 * Does not call move_was_made(), you should do it yourself soon after calling this.
 *
 * Uses game->difficulty as the recursion depth.
 * @return the move that was made.
 */
ComputerMove computer_move(Game *game);

/**
 * Applies minimax algorithm recursively. depthRemaining should be at least 1 on topmost call.
 * @param board game board
 * @param player_is_white true if optimizing for white, false if optimizing for black
 * @param alpha minimum score that the maximizing player is assured of. Starts small, only increases
 * @param beta maximum score that the minimizing player is assured of. Starts large, only decreases
 * @param depthRemaining number of extra depth iterations to go (0 = stop here)
 * @return best move to make from the state of the given board (irrelevant if depthRemaining == 0)
 */
ComputerMove
recursively_minimax_best_move(Board *board, bool player_is_white, int alpha, int beta, int depthRemaining);

/**
 * @return calculated score sum, without checking for draws/checkmates
 */
int calculate_simple_board_score(Board *board);


#endif //UNI_CHESS_PROJECT_MINIMAX_H
