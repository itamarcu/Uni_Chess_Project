
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
bool check_if_king_is_threatened(board_t *board, bool checking_for_white);

/**
 * Is called after a move is made. This function checks if the game has ended, and
 * if not, it simply switches to the next player (and the game progresses). This
 * also updates the game undo history.
 */
void move_was_made(game_t *game, int r1, int c1, int r2, int c2);

/**
 * Check if colored player has any available moves. Returns false if not (= checkmate).
 */
bool check_if_player_can_move(board_t *board, bool checking_for_white);

/**
 * Switches current player (BLACK ↔ WHITE)
 */
void change_current_player(game_t *game);

void start_game(game_t *game);

GAME_ACTION_RESULT console_cmd_move(game_t *game, int r1, int c1, int r2, int c2);

#endif //UNI_CHESS_PROJECT_GAMELOGIC_H
