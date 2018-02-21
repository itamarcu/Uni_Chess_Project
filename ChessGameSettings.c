#include "ChessGameSettings.h"


void cmd_game_mode(Game *game, int gameMode) {
    game -> game_mode = gameMode;
    /* move check to parser. if invalid arg - "wrong game mode".
     * move prints to consoleUI
    if (command->args[0] == NULL) {
        println_error("Wrong game mode");
        return;
    }

    int arg0;
    if (!cast_str_to_int(command->args[0], &arg0)) {
        println_error("Error: invalid command");
        return;
    }

    if (arg0 == 1) {
        game->game_mode = GAME_MODE_SINGLEPLAYER;
        println_output("Game mode is set to 1-player");
    } else if (arg0 == 2) {
        game->game_mode = GAME_MODE_MULTIPLAYER;
        println_output("Game mode is set to 2-player");
    } else {
        println_error("Wrong game mode");
        return;
    }
     */
}

void cmd_difficulty(Game *game, int difficulty) {
    game->difficulty = difficulty;
    /* should move this check into parser - can be valid difficulty command with invalid arg (in this case "wrong diff level"...)
    if (game->game_mode == GAME_MODE_MULTIPLAYER) {
        println_error("Error: invalid command");
        return;
    }
    if (command->args[0] == NULL) {
        println_error("Wrong difficulty level. The value should be between 1 to 5");
        return;
    }

    int arg0;
    if (!cast_str_to_int(command->args[0], &arg0)) {
        println_error("Error: invalid command");
        return;
    }
    if (1 <= arg0 && arg0 <= 5) {
        game->difficulty = arg0;
        println_output("Difficulty level is set to %d", difficulty_int_to_string(game->difficulty));
    } else {
        println_error("Wrong difficulty level. The value should be between 1 to 5");
        return;
    }
     */
}

void cmd_user_color(Game *game, int color) {
    game->user_color = color;
}

bool cmd_load(Game *game, char* path) {

}

void set_default_settings(Game *game) {
    game->difficulty = 2;
    game->game_mode = GAME_MODE_SINGLEPLAYER;
    game->user_color = USER_COLOR_WHITE;
    // println_output("All settings reset to default");
}

void print_settings(Game *game) {
    println_output("SETTINGS:");
    if (game->game_mode == GAME_MODE_MULTIPLAYER) {
        println_output("GAME_MODE: 2-player");
    } else {
        println_output("GAME_MODE: 1-player");
        println_output("DIFFICULTY: %s", difficulty_int_to_string(game->difficulty));
        println_output("USER_COLOR: %s", game->user_color == USER_COLOR_WHITE ? "white" : "black");
    }
}

void free_game(Game *game) {
    free_board(game->board);
    free(game);
}

char *difficulty_int_to_string(int difficulty) {
    switch (difficulty) {
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
            return "what the fuck is going on??? LOOOOOOOOOOL ARE YOU HIGH? XD";
    }
}