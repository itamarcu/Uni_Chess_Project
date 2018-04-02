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
void print_board(board_t *board) {
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
void CUI_settings_case(game_t *game) {
    command_t *command = get_user_input_as_command();
    if (command == NULL) {
        println_error("Real error - malloc failed for command.");
        return;
    }

    if (!command->valid_command) {
        println_error("Error: invalid command");
    } else
        switch (command->settings_command) {
            case CMD_GAME_MODE:
                if (command->args[0] == 1) {
                    game->game_mode = GAME_MODE_SINGLEPLAYER;
                    println_output("game_t mode is set to 1-player");
                } else if (command->args[0] == 2) {
                    game->game_mode = GAME_MODE_MULTIPLAYER;
                    println_output("game_t mode is set to 2-player");
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
                println_error("ERROR: unhandled settings command in switch-case:    %d", command->settings_command);
                break;
        }

    free_command(command);
}


void CUI_game_case(game_t *game) {
    if (game->game_mode == GAME_MODE_MULTIPLAYER || game->current_user == game->user_color) {
        print_board(game->board);
        println_output("Enter your move (%s player):", color_string(game->current_user));

        command_t *command = get_user_input_as_command();
        if (command == NULL) {
            println_error("Real error - malloc failed for command.");
            return;
        }

        possible_move_t moves[MOVES_ARRAY_SIZE];

        if (!command->valid_command) {
            println_error("ERROR: invalid command");
        } else
            switch (command->game_command) {
                case CMD_MOVE:
                    switch (console_cmd_move(game, command->args[0], command->args[1], command->args[2],
                                             command->args[3])) {
                        case SUCCESS:
                            move_was_made(game);
                            break;
                        case INVALID_POS:
                            println_output("Invalid position on the board");
                            break;
                        case NO_PIECE_IN_LOCATION:
                            println_output("The specified position does not contain your piece");
                            break;
                        case ILLEGAL_MOVE:
                            println_output("Illegal move");
                            break;
                        case KING_STILL_THREATENED:
                            println_output("Illegal move: king is still threatened");
                            break;
                        case KING_WILL_BE_THREATENED:
                            println_output("Illegal move: king will be threatened");
                            break;
                        case CANT_SAVE_FILE:
                        case EMPTY_HISTORY:
                            println_error("BUG 4698723154");
                            break;
                    }
                    break;
                case CMD_GET_MOVES:
                    switch (get_possible_moves(game, command->args[0], command->args[1], moves)) {
                        case SUCCESS:
                            for (int i = 0; i < MOVES_ARRAY_SIZE; i++) {
                                if (!moves[i].is_possible)
                                    break; //all moves from now on are guaranteed to be not possible
                                printf(("<%c,%c>"), moves[i].row + '1', moves[i].col + 'A');
                                if (moves[i].is_threatened_by_opponent)
                                    printf("*");
                                if (moves[i].is_capturing)
                                    printf("^");
                                printf("\n");
                            }

                            //free the moves array? //TODO check if needed
                            break;
                        case INVALID_POS:
                            println_output("Invalid position on the board");
                            break;
                        case NO_PIECE_IN_LOCATION:
                            println_output("The specified position does not contain a player piece");
                            break;
                        case ILLEGAL_MOVE:
                        case KING_STILL_THREATENED:
                        case KING_WILL_BE_THREATENED:
                        case CANT_SAVE_FILE:
                        case EMPTY_HISTORY:
                            println_error("BUG 897213857");
                            break;
                    }
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
                    game->state = GAME_STATE_SETTINGS;
                    println_output("Restarting...");
                    println_output("Specify game settings or type 'start' to begin a game with the current settings:");
                    break;
                case CMD_QUIT_GAME:
                    game->state = GAME_STATE_QUIT;
                    println_output("Exiting...");
                    break;
                case CMD_NONE_GAME:
                    println_error("ERROR: invalid command");
                    break;
                default:
                    println_error("ERROR: unhandled game command in switch-case:    %d", command->game_command);
                    break;
            }

        free_command(command);
    } else //computer's turn
    {
        //TODO computer turn
        println_debug("---The computer turn is not yet developed---");
        move_was_made(game);
    }
}


void CUI_main_loop(game_t *game) {
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
                println_error("Weird state...fix this");
                break;
        }
    }
}