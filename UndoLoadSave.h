#ifndef UNI_CHESS_PROJECT_UNDOLOADSAVE_H
#define UNI_CHESS_PROJECT_UNDOLOADSAVE_H

#include "ChessGameSettings.h"

#define HISTORY_SIZE 6

GAME_ACTION_RESULT undo_move(game_t *game);

void push_current_board_to_history(game_t *game);

#endif //UNI_CHESS_PROJECT_UNDOLOADSAVE_H
