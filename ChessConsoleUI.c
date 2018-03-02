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
        return;
    }

    if (!command->isValidCommand) {
        println_error("Error: invalid command");
    } else
        switch (command->settingsCommand) {
            case CMD_GAME_MODE:
                if (command->args[0] == 1) {
                    game->game_mode = GAME_MODE_SINGLEPLAYER;
                    println_output("Game mode is set to 1-player");
                } else if (command->args[0] == 2) {
                    game->game_mode = GAME_MODE_MULTIPLAYER;
                    println_output("Game mode is set to 2-player");
                } else {
                    println_error("Wrong game mode");
                }
                break;
            case CMD_DIFFICULTY:
                if (game->game_mode == GAME_MODE_MULTIPLAYER) {
                    println_error("Error: invalid command");
                    break;
                }
                if (command->args[0] < 1 || command->args[0] > 5) {
                    println_error("Wrong difficulty level. The value should be between 1 to 5");
                    break;
                }
                game->difficulty = command->args[0];
                println_output("Difficulty level is set to %d", difficulty_string(game->difficulty));
                break;
            case CMD_USER_COLOR:
                if (game->game_mode == GAME_MODE_MULTIPLAYER) {
                    println_error("Error: invalid command");
                    break;
                }
                game->user_color = command->args[0];
                println_output("User color is set to %s", color_string(game->user_color));
                break;
            case CMD_LOAD:
                //TODO load
                println_debug("---The load command is not yet developed---");
                break;
            case CMD_DEFAULT:
                reset_default_settings(game);
                println_output("All settings reset to default");
                break;
            case CMD_PRINT_SETTINGS:
                println_output("SETTINGS:");
                if (game->game_mode == GAME_MODE_MULTIPLAYER) {
                    println_output("GAME_MODE: 2-player");
                } else {
                    println_output("GAME_MODE: 1-player");
                    println_output("DIFFICULTY: %s", difficulty_string(game->difficulty));
                    println_output("USER_COLOR: %s", color_string(game->user_color));
                }
                break;
            case CMD_QUIT_SETTINGS:
                println_output("Exiting...");
                game->state = GAME_STATE_QUIT;
                break;
            case CMD_START:
                println_output("Starting game...");
                start_game(game);
                break;
            case CMD_NONE_SETTINGS:
                println_error("ERROR: invalid command");
                break;
            default:
                println_error("ERROR: unhandled settings command in switch-case:    %d", command->settingsCommand);
                break;
        }

    free_command(command);
}


//TO-DO: make it a switch-case over the different types of Commands, and check valid Game_command first. also, add console respond prints according to the relavent function GAME_RESPONSE.
void CUI_game_case(Game *game) {
    if (game->game_mode == GAME_MODE_MULTIPLAYER || game->current_user == game->user_color) {
        print_board(game->board);
        println_output("Enter your move (%s player):", color_string(game->user_color));

        Command *command = get_user_input_as_command();
        if (command == NULL) {
            println_error("Real error - malloc failed for command.");
            return;
        }

        if (!command->isValidCommand) {
            println_error("ERROR: invalid command");
        } else
            switch (command->gameCommand) {
                case CMD_MOVE:
                    switch (console_cmd_move(game, command->args[0], command->args[1], command->args[2],
                                             command->args[3])) {
                        case SUCCESS:
                            break;
                        case INVALID_POS:
                            break;
                        case NO_PIECE_IN_LOCATION:
                            break;
                        case ILLEGAL_MOVE:
                            break;
                        case KING_STILL_THREATENED:
                            break;
                        case KING_WILL_BE_THREATENED:
                            break;
                        case CANT_SAVE_FILE:
                            break;
                        case EMPTY_HISTORY:
                            break;
                    }
                    break;
                case CMD_GET_MOVES:
                    //TODO get_moves command
                    //use console_cmd_get_moves()
                    println_debug("---The get_moves command is not yet developed---");
                    break;
                case CMD_SAVE:
                    //TODO save command
                    println_debug("---The save command is not yet developed---");
                    break;
                case CMD_UNDO:
                    //TODO undo command
                    println_debug("---The undo command is not yet developed---");
                    break;
                case CMD_RESET:
                    println_output("Restarting...");
                    println_output("Specify game settings or type 'start' to begin a game with the current settings:");
                    game->state = GAME_STATE_SETTINGS;
                    break;
                case CMD_QUIT_GAME:
                    println_output("Exiting...");
                    game->state = GAME_STATE_QUIT;
                    break;
                case CMD_NONE_GAME:
                    println_error("ERROR: invalid command");
                    break;
                default:
                    println_error("ERROR: unhandled game command in switch-case:    %d", command->gameCommand);
                    break;
            }

        free_command(command);
    } else //computer's turn
    {
        //TODO computer turn
        move_was_made(game);
    }
}


void CUI_main_loop(Game *game) {
    println_output("Chess");
    println_output("-------");
    println_output("Specify game settings or type 'start' to begin a game with the current settings:");
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