
#ifndef UNI_CHESS_PROJECT_GAMELOGIC_H
#define UNI_CHESS_PROJECT_GAMELOGIC_H

#include "PossibleMoveLogic.h"
#include "ChessGameSettings.h"
#include <minmax.h>
#include <strings.h>
#include <ctype.h>

typedef enum {
    SUCCESS,
    INVALID_POS,
    NO_PEICE_IN_POS,
    ILLEGAL_MOVE,
    KING_STILL_THREATENED,
    KING_WILL_BE_THREATENED,
    CANT_SAVE_FILE,
    EMPTY_HISTORY

} GAME_ACTION_RESULT;

/**
 * @return true when the king of the current turn color is checked
 */
bool is_current_checked(Game *game);

GAME_ACTION_RESULT move_was_made(Game *game);

void change_current_player(Game *game);

void cmd_start(Game *game);

GAME_ACTION_RESULT cmd_move(Game *game, int r1, int c1, int r2, int c2);

GAME_ACTION_RESULT cmd_get_moves(Game *game, int r, int c);

GAME_ACTION_RESULT cmd_save(Game *game);

GAME_ACTION_RESULT cmd_undo(Game *game);

void cmd_reset(Game *game);

void cmd_quit(Game *game);

#endif //UNI_CHESS_PROJECT_GAMELOGIC_H
