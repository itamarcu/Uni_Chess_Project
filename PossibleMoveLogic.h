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

GAME_ACTION_RESULT get_possible_moves(game_t *game, int r1, int c1, possible_move_t *possible_moves);

#endif //UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
