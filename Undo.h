#ifndef UNI_CHESS_PROJECT_UNDO_H
#define UNI_CHESS_PROJECT_UNDO_H

#include "ChessGameSettings.h"
#include "Auxiliary.h"

/**
 * Undoes the 1 or 2 most recent moves in the game.
 *
 *
 * Will undo 2 moves unless:
 *
 * * There is only 1 move in history
 *
 * * The game is singleplayer and the user has won
 *
 * in which case it undoes 1 move.
 *
 * @return result of undo. SUCCESS if it worked correctly, EMPTY_HISTORY if the history
 * was empty.
 */
GAME_ACTION_RESULT undo_move(Game *game);

/**
 * Is called after a player makes a move. This function adds the move (<r1,c1> to <r2,c2>,
 * in zero-based coordinates) to the game's history. If the history is full, it will
 * delete the oldest record from it.
 */
void push_move_to_history(Game *game, int r1, int c1, int r2, int c2);

#endif //UNI_CHESS_PROJECT_UNDO_H
