#include "ChessConsoleUI.h"
#include "Undo.h"


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

    if (!command->valid_command) {
        println_error_weak("Error: invalid command");
    } else
        switch (command->settings_command) {
            case CMD_GAME_MODE:
                if (command->args[0] == 1) {
                    game->game_mode = GAME_MODE_SINGLEPLAYER;
                    println_output("Game mode is set to 1-player");
                } else if (command->args[0] == 2) {
                    game->game_mode = GAME_MODE_MULTIPLAYER;
                    println_output("Game mode is set to 2-player");
                } else {
                    println_error_weak("Wrong game mode");
                }
                break;
            case CMD_DIFFICULTY:
                if (game->game_mode == GAME_MODE_MULTIPLAYER) {
                    println_error_weak("Error: invalid command");
                    break;
                }
                if (command->args[0] < 1 || command->args[0] > 5) {
                    println_error_weak("Wrong difficulty level. The value should be between 1 to 5");
                    break;
                }
                game->difficulty = command->args[0];
                println_output("Difficulty level is set to %s", difficulty_string(game->difficulty));
                break;
            case CMD_USER_COLOR:
                if (game->game_mode == GAME_MODE_MULTIPLAYER) {
                    println_error_weak("Error: invalid command");
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
                if (game->user_color != WHITE && game->game_mode == GAME_MODE_SINGLEPLAYER) // special case
                    print_board(game->board);
                break;
            case CMD_NONE_SETTINGS:
                println_error_weak("ERROR: invalid command");
                break;
            default:
                println_error("ERROR: unhandled settings command in switch-case:    %d", command->settings_command);
                break;
        }

    free_command(command);
}


void CUI_game_case(Game *game) {
    if (game->game_mode == GAME_MODE_MULTIPLAYER || game->current_player == game->user_color) {
        print_board(game->board);
//        if (DEBUG_MODE)
//            println_debug("[BOARD SCORE: %d]", calculate_simple_board_score(game->board));
        println_output("Enter your move (%s player):", color_string(game->current_player));

        bool turn_was_made = false;

        while (!turn_was_made) {
            Command *command = get_user_input_as_command();
            if (command == NULL) {
                println_error("Real error - malloc failed for command.");
                return;
            }

            PossibleMove moves[MOVES_ARRAY_SIZE];
            ComputerMove auto_move;

            if (!command->valid_command) {
                println_error_weak("ERROR: invalid command");
            } else
                switch (command->game_command) {
                    case CMD_MOVE:
                        switch (console_cmd_move(game, command->args[0], command->args[1], command->args[2],
                                                 command->args[3])) {
                            case SUCCESS:
                                move_was_made(game, command->args[0], command->args[1], command->args[2],
                                              command->args[3]);
                                turn_was_made = true;
                                break;
                            case INVALID_POS:
                                println_error_weak("Invalid position on the board");
                                break;
                            case NO_PIECE_IN_LOCATION:
                                println_error_weak("The specified position does not contain your piece");
                                break;
                            case ILLEGAL_MOVE:
                                println_error_weak("Illegal move");
                                break;
                            case KING_STILL_THREATENED:
                                println_error_weak("Illegal move: king is still threatened");
                                break;
                            case KING_WILL_BE_THREATENED:
                                println_error_weak("Illegal move: king will be threatened");
                                break;
                            case CANT_SAVE_FILE:
                            case EMPTY_HISTORY:
                                println_error("BUG 4698723154");
                                break;
                        }
                        break;
                    case CMD_GET_MOVES:
                        switch (get_possible_moves(game->board, command->args[0], command->args[1], moves)) {
                            case SUCCESS:
                                for (int i = 0; i < MOVES_ARRAY_SIZE; i++) {
                                    if (!moves[i].is_legal)
                                        break; //all moves from now on are guaranteed to be not possible
                                    printf(("<%c,%c>"), moves[i].row + '1', moves[i].col + 'A');
                                    if (moves[i].is_threatened_by_opponent)
                                        printf("*");
                                    if (moves[i].is_capturing)
                                        printf("^");
                                    printf("\n");
                                }
                                break;
                            case INVALID_POS:
                                println_error_weak("Invalid position on the board");
                                break;
                            case NO_PIECE_IN_LOCATION:
                                println_error_weak("The specified position does not contain a player piece");
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
                        switch (undo_move(game)) {
                            case SUCCESS:
                                // Print happens inside the function, if game mode is console :/
                                turn_was_made = true;
                                break;
                            case EMPTY_HISTORY:
                                println_error_weak("Empty history, no move to undo");
                                break;
                            default:
                                println_error("Unexpected enum value after using cmd_undo");
                                break;
                        }
                        break;
                    case CMD_RESET:
                        game->state = GAME_STATE_SETTINGS;
                        println_output("Restarting...");
                        println_output(
                                "Specify game settings or type 'start' to begin a game with the current settings:");
                        turn_was_made = true;
                        break;
                    case CMD_QUIT_GAME:
                        game->state = GAME_STATE_QUIT;
                        println_output("Exiting...");
                        turn_was_made = true;
                        break;
                    case CMD_NONE_GAME:
                        println_error_weak("ERROR: invalid command");
                        break;
                    case CMD_MAKE_MY_MOVE:
                        auto_move = computer_move(game);
                        println_output("Auto-Player: move %s at <%c,%c> to <%c,%c>",
                                       name_of_piece(game->board->grid[auto_move.r1][auto_move.r2]),
                                       auto_move.r1 + '1', auto_move.c1 + 'A',
                                       auto_move.r2 + '1', auto_move.c2 + 'A');
                        move_was_made(game, auto_move.r1, auto_move.c1, auto_move.r2, auto_move.c2);
                        turn_was_made = true;
                        break;
                    default:
                        println_error("ERROR: unhandled game command in switch-case:    %d", command->game_command);
                        break;
                }
            free_command(command);
        }

    } else //computer's turn
    {
        if (DEBUG_MODE) {
            print_board(game->board);
            println_debug("Calculating computer move... (current board score: %d)",
                          calculate_simple_board_score(game->board));
        }

        ComputerMove move = computer_move(game);
        println_output("Computer: move %s at <%c,%c> to <%c,%c>", name_of_piece(game->board->grid[move.r1][move.r2]),
                       move.r1 + '1', move.c1 + 'A',
                       move.r2 + '1', move.c2 + 'A');
        move_was_made(game, move.r1, move.c1, move.r2, move.c2);
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
                break;
            default:
                println_error("Weird state...fix this");
                break;
        }
    }
    if (game->winner == GAME_WINNER_NONE) {
        //No text, just immediately terminate
        return;
    }

    if (game->winner == GAME_WINNER_WHITE || game->winner == GAME_WINNER_BLACK) {
        println_output("Checkmate! %s player wins the game", game->winner == GAME_WINNER_WHITE ? "white" : "black");
    } else if (game->winner == GAME_WINNER_DRAW) {
        println_output("The game ends in a draw");
    } else {
        println_error("Winner is %d... This is a bug.", game->winner);
    }


    // The project instructions were unclear about printing at endgame - I'm doing it anyways
    print_board(game->board);
}