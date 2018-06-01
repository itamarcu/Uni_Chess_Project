
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
bool check_if_king_is_threatened(Board *board, bool checking_for_white);

/**
 * Is called after a move is made. This function checks if the game has ended, and
 * if not, it simply switches to the next player (and the game progresses). This
 * also updates the game undo history.
 */
void move_was_made(Game *game, int r1, int c1, int r2, int c2);

/**
 * Check if colored player has any available moves. Returns false if not (= checkmate).
 */
bool check_if_player_can_move(Board *board, bool checking_for_white);

/**
 * Switches current player (BLACK ↔ WHITE)
 */
void change_current_player(Game *game);

void reset_game_board(Game *game);

int start_game(Game *game);

GAME_ACTION_RESULT console_cmd_move(Game *game, int r1, int c1, int r2, int c2);


// "Private" helper functions:


bool is_partially_legal_move(char grid[8][8], int r1, int c1, int r2, int c2);

/**
 * Helper function - checks for move legality with the assumptions that
 * all parameters are inside the grid and grid[r1][c1] has been emptied.
 */
bool is_partially_legal_move_without_start(char grid[8][8], int r1, int c1, int r2, int c2, char moving_piece,
                                           char target_piece);

#endif //UNI_CHESS_PROJECT_GAMELOGIC_H
