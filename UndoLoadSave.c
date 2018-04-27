#include "UndoLoadSave.h"
#include "ChessGameLogic.h"

GAME_ACTION_RESULT undo_move(game_t *game) {
    if (game->history->count == 0) {
        return EMPTY_HISTORY;
    }

    if (game->history->count == 1) {
        // Go back 1 move
        game->history->count = 0;
        change_current_player(game);
        free_board(game->board);
        game->board = game->history->prev_boards[0];
    } else {
        // Go back 2 moves
        free_board(game->board);
        free_board(game->history->prev_boards[0]);
        game->board = game->history->prev_boards[1];
        // push forward all other records
        game->history->count -= 2;
        for (int i = 0; i < game->history->count; i++) {
            game->history->prev_boards[i] = game->history->prev_boards[i + 2];
        }
    }
    return SUCCESS;
}

void push_current_board_to_history(game_t *game) {
    if (game->history->count < HISTORY_SIZE) {
        // Enough space left to simply add to history
        game->history->prev_boards[game->history->count] = copy_board(game->board);
        game->history->count += 1;
    } else {
        // History is full (count == HISTORY_SIZE)
        // free the oldest record
        free(game->history->prev_boards[HISTORY_SIZE - 1]);
        //push back all other records
        for (int i = HISTORY_SIZE - 1; i > 0; i--) {
            game->history->prev_boards[i] = game->history->prev_boards[i - 1];
        }
        //add current board to front
        game->history->prev_boards[0] = copy_board(game->board);
    }
}