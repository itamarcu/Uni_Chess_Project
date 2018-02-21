#ifndef UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
#define UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H

#include "ChessGameSettings.h"
#include <stdlib.h>

typedef struct possible_move_type {
    int row, col;
    bool is_capturing;
    bool is_threatening;
} PossibleMove;

/**
 * @return an array with size 32
 */
PossibleMove *get_possible_moves(Game *game, int r1, int c1);

#endif //UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
