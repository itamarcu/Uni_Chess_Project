
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Game.h"
#include "GameSettings.h"
#include "ConsoleUI.h"


void settings_case(Game *game) {
    Command *command = get_user_input_as_command();
    if (command == NULL) {
        println_error("Real error - malloc failed for command.");
    } else if (command->cmd == NULL) {
        println_error("Error: invalid command");
    } else if (strcmp(command->cmd, "game_mode") == 0) {
        cmd_game_mode(game, command);
    } else if (strcmp(command->cmd, "difficulty") == 0) {
        cmd_difficulty(game, command);
    } else if (strcmp(command->cmd, "user_color") == 0) {
        cmd_user_color(game, command);
    } else if (strcmp(command->cmd, "load") == 0) {
        cmd_load(game, command);
    } else if (strcmp(command->cmd, "default") == 0) {
        cmd_default(game, command);
    } else if (strcmp(command->cmd, "print_settings") == 0) {
        cmd_print_settings(game, command);
    } else if (strcmp(command->cmd, "quit") == 0) {
        cmd_quit(game, command);
    } else if (strcmp(command->cmd, "start") == 0) {
        cmd_start(game, command);
    } else {
        println_error("Error: invalid command");
    }

    free_command(command);
}

void game_case(Game *game) {
    if (game->game_mode == GAME_MODE_MULTIPLAYER || game->current_user == game->user_color) {
        print_board(game->board);
        char *player = game->current_user == USER_COLOR_WHITE ? "white" : "black";
        println_output("Enter your move (%s player):", player);

        Command *command = get_user_input_as_command();
        if (command == NULL) {
            println_error("Real error - malloc failed for command.");
        } else if (command->cmd == NULL) {
            println_error("Error: invalid command");
        } else if (strcmp(command->cmd, "move") == 0) {
            unabbreviated_cmd_move(game, command);
        } else if (strcmp(command->cmd, "mov") == 0 && DEBUG_MODE) {
            abbreviated_cmd_move(game, command);
        } else if (strcmp(command->cmd, "get_moves") == 0) {
            cmd_get_moves(game, command);
        } else if (strcmp(command->cmd, "save") == 0) {
            cmd_save(game, command);
        } else if (strcmp(command->cmd, "undo") == 0) {
            cmd_undo(game, command);
        } else if (strcmp(command->cmd, "reset") == 0) {
            cmd_reset(game, command);
        } else if (strcmp(command->cmd, "quit") == 0) {
            cmd_quit(game, command);
        } else {
            println_error("Error: invalid command");
        }

        free_command(command);
    } else //computer's turn
    {
        //TODO
        move_was_made(game);
    }
}

void move_was_made(Game *game) {
    //check for checkmate or draw
    //TODO
    int game_ended = 0;

    if (game_ended) {
        game->state = GAME_STATE_QUIT;
    } else {
        change_current_player(game);
    }
}

void change_current_player(Game *game) {
    if (game->current_user == USER_COLOR_WHITE) {
        game->current_user = USER_COLOR_BLACK;
    } else {
        game->current_user = USER_COLOR_WHITE;
    }
}

void set_game_state_to_settings(Game *game) {
    game->state = GAME_STATE_SETTINGS;
    println_output("Specify game settings or type 'start' to begin a game with the current settings:");
}

void cmd_quit(Game *game, Command *command) {
    println_output("Exiting...");
    game->state = GAME_STATE_QUIT;
}

void main_loop(Game *game) {
    while (game->state != GAME_STATE_QUIT) {
        switch (game->state) {
            case GAME_STATE_SETTINGS:
                settings_case(game);
                break;
            case GAME_STATE_GAME:
                game_case(game);
                break;
            case GAME_STATE_QUIT:
                return; //unnecessary but I want this
            default:
                println_debug("Weird state...fix this");
                break;
        }
    }
}

void free_game(Game *game) {
    free_board(game->board);
    free(game);
}

void free_command(Command *command) {
    if (command == NULL) {
        return;
    }

    free(command->cmd);
    for (int i = 0; i < 3; i++) {
        free(command->args[i]);
    }
    free(command->args);
    free(command);
}