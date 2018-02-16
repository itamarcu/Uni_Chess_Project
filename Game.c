
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Game.h"
#include "Auxiliary.h"


void cmd_game_mode(Game *game, Command *command) {
    if (command->args[0] == NULL) {
        println_error("Wrong game mode");
        return;
    }

    int arg0;
    CAST_STR_TO_INT_OR_ERROR_AND_RETURN(arg0, command->args[0])

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
    CAST_STR_TO_INT_OR_ERROR_AND_RETURN(arg0, command->args[0])
    if (1 <= arg0 && arg0 <= 5) {
        game->difficulty = arg0;
        println_output("Game mode is set to %d", game->difficulty);
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

}

void cmd_quit(Game *game, Command *command) {
    println_output("Exiting...");
    game->state = GAME_STATE_QUIT;
}

void cmd_start(Game *game, Command *command) {
    println_output("Starting game...");
    game->state = GAME_STATE_GAME;
    game->current_user = USER_COLOR_WHITE;
    game->winner = GAME_WINNER_NONE;
    free_board(game->board);
    game->board = make_starting_board();
}

void cmd_move(Game *game, Command *command) {
    // format should be: "move <r1,c1> to <r2,c2>"
    // for example, "move <1,A> to <8,H>"
    // here, command args should be "<1,A>", "to", "<8,H>"
    if (command->args[2] == NULL || strcmp(command->args[1], "to") != 0
        || strlen(command->args[0]) != 5 || strlen(command->args[2]) != 5
        || command->args[0][0] != '<' || command->args[0][2] != ',' || command->args[0][4] != '>'
        || command->args[2][0] != '<' || command->args[2][2] != ',' || command->args[2][4] != '>') {
        println_error("Error: invalid command");
        return;
    }


    int r1 = command->args[0][1] - '1';
    int c1 = command->args[0][3] - 'A';
    int r2 = command->args[2][1] - '1';
    int c2 = command->args[2][3] - 'A';

    if (0 > r1 || r1 >= 8 || 0 > c1 || c1 >= 8 || 0 > r2 || r2 >= 8 || 0 > c2 || c2 >= 8) {
        println_error("Invalid position on the board");
        return;
    }

    char moving_piece = game->board->grid[r1][c1];
    char target_piece = game->board->grid[r2][c2];

    if (is_empty_space(moving_piece) || (is_white_piece(moving_piece) != (game->current_user == USER_COLOR_WHITE))) {
        println_error("The specified position does not contain your piece");
        return;
    }

    switch (toupper(moving_piece))
    {
        case 'M': //pawn
            if (r2 == r1 + 2 && c1==c2 && is_empty_space(game->board->grid[r1+1][c1]) && is_empty_space())
            break;
        default:
            println_debug("unknown piece! %c", moving_piece);
            return;
    }
}

void cmd_get_moves(Game *game, Command *command) {

}

void cmd_save(Game *game, Command *command) {

}

void cmd_undo(Game *game, Command *command) {

}

void cmd_reset(Game *game, Command *command) {
    println_output("Restarting...");
    set_game_state_to_settings(game);
}

void set_game_state_to_settings(Game *game) {
    game->state = GAME_STATE_SETTINGS;
    println_output("Specify game settings or type 'start' to begin a game with the current settings:");
}

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
            cmd_move(game, command);
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
    int game_ended = 1;

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