#include "ChessGameSettings.h"
#include "Auxiliary.h"

void reset_default_settings(Game *game) {
    game->difficulty = 2;
    game->game_mode = GAME_MODE_SINGLEPLAYER;
    game->user_color = WHITE;
    game->state = GAME_STATE_SETTINGS;
    game->is_saved = false;
}

void free_history(History *history) {
    if (history == NULL)
        return;
    for (int i = 0; i < history->count; i++) {
        free_board(history->prev_boards[i]);
    }
    free(history);
}

void free_game(Game *game) {
    free_board(game->board);
    free_history(game->history);
    free(game);
}

char *difficulty_string(int difficulty_int) {
    switch (difficulty_int) {
        case AMATEUR_DIFFICULTY:
            return "amateur";
        case EASY_DIFFICULTY:
            return "easy";
        case MODERATE_DIFFICULTY:
            return "moderate";
        case HARD_DIFFICULTY:
            return "hard";
        case EXPERT_DIFFICULTY:
            return "expert";
        default:
            println_error("Difficulty is not in range 1 to 5");
            return "plz fix ur input";
    }
}

char *color_string(int color_int) {
    if (color_int == WHITE) {
        return "white";
    } else if (color_int == BLACK) {
        return "black";
    }
    println_error("wrong color int was input into color_string(), should be WHITE or BLACK");
    return "asian";
}