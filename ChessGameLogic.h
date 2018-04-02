
#ifndef UNI_CHESS_PROJECT_GAMELOGIC_H
#define UNI_CHESS_PROJECT_GAMELOGIC_H

#include "PossibleMoveLogic.h"
#include "ChessGameSettings.h"
#include "Minimax.h"
#include <strings.h>
#include <ctype.h>

/**
 * @return true when the king of the current turn color is checked
 */
bool is_current_checked(game_t *game);

GAME_ACTION_RESULT move_was_made(game_t *game);

/**
 * Check if colored player has any available moves. Returns true if not (= checkmate).
 */
bool check_checkmate(game_t *game, bool checking_for_white);

void change_current_player(game_t *game);

void start_game(game_t *game);

GAME_ACTION_RESULT console_cmd_move(game_t *game, int r1, int c1, int r2, int c2);

#endif //UNI_CHESS_PROJECT_GAMELOGIC_H
