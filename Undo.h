#ifndef UNI_CHESS_PROJECT_UNDO_H
#define UNI_CHESS_PROJECT_UNDO_H

#include "ChessGameSettings.h"

GAME_ACTION_RESULT undo_move(game_t *game);

void push_move_to_history(game_t *game, int r1, int c1, int r2, int c2);

#endif //UNI_CHESS_PROJECT_UNDO_H
