#ifndef UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
#define UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H

#include <stdlib.h>
#include "ChessGameSettings.h"
#include "Auxiliary.h"

#define MOVES_ARRAY_SIZE 32

typedef struct possible_move_t {
    int row, col;
    bool is_capturing;
    bool is_threatened_by_opponent;
    bool is_legal;  // if this is false, ignore all others
} PossibleMove;

/**
 * @param possible_moves empty array of PossibleMove objects, to be filled. The array is guaranteed to
 * have only possible moves up to a certain index, and then only impossible moves afterwards.
 * @return result of action. Will not fill any moves if input is invalid.
 */
GAME_ACTION_RESULT get_possible_moves(Board *board, int r1, int c1, PossibleMove possible_moves[MOVES_ARRAY_SIZE]);

/**
 * Returns true if the piece on the board has any valid moves (this is faster than get_possible_moves)
 *
 * NOTE: this assumes correct input (correct r1, r2, nonempty...)
 */
bool has_any_possible_moves(Board *board, int r1, int c1);

#endif //UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
