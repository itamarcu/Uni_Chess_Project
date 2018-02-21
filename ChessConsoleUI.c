#include "ChessConsoleUI.h"


/**
 * example:
 *
 * 8| R _ _ n _ K _ _ |
 * 7| M M _ _ _ _ _ N |
 * 6| _ _ M _ _ _ M M |
 * 5| _ _ _ _ _ _ _ _ |
 * 4| _ _ _ q _ _ _ _ |
 * 3| _ b _ _ _ _ _ m |
 * 2| m _ m _ _ k m _ |
 * 1| _ _ _ _ r _ _ _ |
 *   -----------------
 *    A B C D E F G H
 */
void print_board(Board *board) {
    for (int row = 7; row >= 0; row--) {
        printf("%d|", (row + 1));
        for (int col = 0; col < 8; col++) {
            printf(" %c", board->grid[row][col]);
        }
        printf(" |\n");
    }
    printf("  -----------------\n"); // that's 2 spaces and 17 dashes
    printf("   A B C D E F G H\n");  // that's 3 spaces and A B ... H
}

//TO-DO: make it a switch-case over the different types of Commands, and check valid Settings_command first. also, add console respond prints.
void CUI_settings_case(Game *game) {
    Command *command = get_user_input_as_command();
    if (command == NULL) {
        println_error("Real error - malloc failed for command.");
    } else if (command->cmd == NULL) {
        println_error("Error: invalid command");
    } else if (strcmp(command->cmd, "game_mode") == 0) {
        cmd_game_mode(game, gameMode);
    } else if (strcmp(command->cmd, "difficulty") == 0) {
        cmd_difficulty(game, difficulty);
    } else if (strcmp(command->cmd, "user_color") == 0) {
        cmd_user_color(game, color);
    } else if (strcmp(command->cmd, "load") == 0) {
        cmd_load(game, path);
    } else if (strcmp(command->cmd, "default") == 0) {
        set_default_settings(game);
        println_output("All settings reset to default");
    } else if (strcmp(command->cmd, "print_settings") == 0) {
        print_settings(game);
    } else if (strcmp(command->cmd, "quit") == 0) {
        cmd_quit(game);
    } else if (strcmp(command->cmd, "start") == 0) {
        println_output("Starting game...");
        cmd_start(game);
    } else {
        println_error("Error: invalid command");
    }

    free(command);
}


//TO-DO: make it a switch-case over the different types of Commands, and check valid Game_command first. also, add console respond prints according to the relavent function GAME_RESPONSE.
void CUI_game_case(Game *game) {
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
            cmd_move(game, r1,c1,r2,c2);
        } else if (strcmp(command->cmd, "get_moves") == 0) {
            cmd_get_moves(game, r, c);
        } else if (strcmp(command->cmd, "save") == 0) {
            cmd_save(game);
        } else if (strcmp(command->cmd, "undo") == 0) {
            cmd_undo(game);
        } else if (strcmp(command->cmd, "reset") == 0) {
            println_output("Restarting...");
            println_output("Specify game settings or type 'start' to begin a game with the current settings:");
            cmd_reset(game);
        } else if (strcmp(command->cmd, "quit") == 0) {
            println_output("Exiting...");
            cmd_quit(game);
        } else {
            println_error("Error: invalid command");
        }

        free(command);
    } else //computer's turn
    {
        //TODO
        move_was_made(game);
    }
}


void CUI_main_loop(Game *game){
    println_output("Chess");
    println_output("-------");
    while (game->state != GAME_STATE_QUIT) {
        switch (game->state) {
            case GAME_STATE_SETTINGS:
                CUI_settings_case(game);
                break;
            case GAME_STATE_GAME:
                CUI_game_case(game);
                break;
            case GAME_STATE_QUIT:
                return; //unnecessary but I want this
            default:
                println_debug("Weird state...fix this");
                break;
        }
    }
}