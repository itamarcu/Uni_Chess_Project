#ifndef UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
#define UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H

#include <stdlib.h>
#include "ChessGameSettings.h"
#include "ChessGameLogic.h"

#define MOVES_ARRAY_SIZE 32

GAME_ACTION_RESULT get_possible_moves(Game *game, int r1, int c1, PossibleMove *possibleMoves);

#endif //UNI_CHESS_PROJECT_POSSIBLEMOVELOGIC_H
