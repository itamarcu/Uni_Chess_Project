#include "Undo.h"
#include "ChessGameLogic.h"

/**
 * Helper function for undoing. This also prints the undo, if the program mode is PROGRAM_MODE_CONSOLE,
 * which isn't very nicely-designed code structure but I was in a hurry and the alternative was frustrating.
 * This also changes current player because it fits every time.
 */
void undo_one_move(Game *game) {
    change_current_player(game);

    if (game->program_mode == PROGRAM_MODE_CONSOLE) {
        // sorry!
        unsigned int m = game->history->prev_moves[0];
        char r1 = (char) ((m >> 24) + '1');
        char c1 = (char) ((m >> 16) + 'A');
        char r2 = (char) ((m >> 8) + '1');
        char c2 = (char) ((m >> 0) + 'A');
        char *player = color_string(game->current_player);
        println_output("Undo move for %s player: <%c,%c> -> <%c,%c>", player, r1, c1, r2, c2); // (CORRECT ORDER!)
    }

    free_board(game->history->prev_boards[0]);
    // push forward all records
    for (int i = 0; i < game->history->count - 1; i++) {
        game->history->prev_boards[i] = game->history->prev_boards[i + 1];
        game->history->prev_moves[i] = game->history->prev_moves[i + 1];
    }
    game->history->count -= 1;
}

GAME_ACTION_RESULT undo_move(Game *game) {
    // (game->history->count != 0) !!!
    if (game->history->count == 1) {
        return EMPTY_HISTORY;
    }

    if (game->history->count == 2) {
        // Go back 1 move
        undo_one_move(game);
        free_board(game->board);
        game->board = copy_board(game->history->prev_boards[0]);
    } else {
        // Go back 2 moves
        undo_one_move(game);
        undo_one_move(game);
        free_board(game->board);
        game->board = copy_board(game->history->prev_boards[0]);
    }

    return SUCCESS;
}

void push_move_to_history(Game *game, int r1, int c1, int r2, int c2) {
    if (game->history->count == HISTORY_SIZE) {
        // free the oldest record
        free_board(game->history->prev_boards[HISTORY_SIZE - 1]);
    } else {
        game->history->count += 1;
    }
    // push back all other records
    for (int i = game->history->count - 1; i > 0; i--) {
        game->history->prev_boards[i] = game->history->prev_boards[i - 1];
        game->history->prev_moves[i] = game->history->prev_moves[i - 1];
    }

    // add current board to front
    game->history->prev_boards[0] = copy_board(game->board);
    unsigned int r1_ = (unsigned int) r1;
    unsigned int c1_ = (unsigned int) c1;
    unsigned int r2_ = (unsigned int) r2;
    unsigned int c2_ = (unsigned int) c2;
    game->history->prev_moves[0] = ((r1_ << 24) + (c1_ << 16) + (r2_ << 8) + (c2_ << 0));
}