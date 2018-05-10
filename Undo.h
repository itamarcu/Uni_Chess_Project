#ifndef UNI_CHESS_PROJECT_UNDO_H
#define UNI_CHESS_PROJECT_UNDO_H

#include "ChessGameSettings.h"

GAME_ACTION_RESULT undo_move(Game *game);

void push_move_to_history(Game *game, int r1, int c1, int r2, int c2);

#endif //UNI_CHESS_PROJECT_UNDO_H
