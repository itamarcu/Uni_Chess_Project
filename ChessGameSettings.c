#include "ChessGameSettings.h"


void reset_default_settings(Game *game) {
    game->difficulty = 2;
    game->game_mode = GAME_MODE_SINGLEPLAYER;
    game->user_color = WHITE;
    game->state = GAME_STATE_SETTINGS;
}

void free_game(Game *game) {
    free_board(game->board);
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
            println_debug("Difficulty is not in range 1 to 5");
            return "plz fix ur input";
    }
}

char *color_string(int color_int) {
    if (color_int == WHITE) {
        return "white";
    } else if (color_int == BLACK) {
        return "black";
    }
    println_debug("wrong color int was input into color_string(), should be USER_COLOR_xxxxx");
    return "asian";
}