#ifndef UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
#define UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H

#include <stdlib.h>
#include "ChessGameSettings.h"
#include "ChessGameLogic.h"

#define MOVES_ARRAY_SIZE 32

typedef struct possible_move_t {
    int row, col;
    bool is_capturing;
    bool is_threatened_by_opponent;
    bool is_possible;  // if this is false, ignore all others
} possible_move_t;

/**
 * @param possible_moves empty array of PossibleMove objects, to be filled. The array is guaranteed to
 * have only possible moves up to a certain index, and then only impossible moves afterwards.
 * @return result of action. Will not fill any moves if input is invalid.
 */
GAME_ACTION_RESULT get_possible_moves(board_t *board, int r1, int c1, possible_move_t possible_moves[MOVES_ARRAY_SIZE]);

/**
 * Returns true if the move is valid (this is faster than update_move_by_potential_threats)
 *
 * NOTE: this assumes correct input (correct r1, r2, nonempty...)
 */
bool optimized_move_legality_check(board_t *board, int r1, int c1, int r2, int c2);

/**
 * Returns true if the piece on the board has any valid moves (this is faster than get_possible_moves)
 *
 * NOTE: this assumes correct input (correct r1, r2, nonempty...)
 */
bool has_any_possible_moves(board_t *board, int r1, int c1);

#endif //UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
