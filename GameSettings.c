#include "GameSettings.h"


void cmd_game_mode(Game *game, Command *command) {
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
}

void cmd_difficulty(Game *game, Command *command) {
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
}

void cmd_user_color(Game *game, Command *command) {

}

void cmd_load(Game *game, Command *command) {

}

void cmd_default(Game *game, Command *command) {
    game->difficulty = 2;
    game->game_mode = GAME_MODE_SINGLEPLAYER;
    game->user_color = USER_COLOR_WHITE;
    println_output("All settings reset to default");
}

void cmd_print_settings(Game *game, Command *command) {
    println_output("SETTINGS:");
    if (game->game_mode == GAME_MODE_MULTIPLAYER) {
        println_output("GAME_MODE: 2-player");
    } else {
        println_output("GAME_MODE: 1-player");
        println_output("DIFFICULTY: %s", difficulty_int_to_string(game->difficulty));
        println_output("USER_COLOR: %s", game->user_color == USER_COLOR_WHITE ? "white" : "black");
    }
}
