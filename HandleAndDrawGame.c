#include "HandleAndDrawGame.h"

/**
 * update the game gui board from the current game->board.
 */
void update_game_gui_board(game_gui_t *game_gui, Game *game);

/**
 * destroying the game_gui widget.
 * @param src pointer to a widget containing game_gui_t as its data.
 */
void destroy_game_gui(widget_t *src);

/**
 * handling all the game_gui events - making a move, choosing a piece.
 * @param src pointer to a widget containing game_gui_t as its data.
 */
void handle_game_gui_event(widget_t *src, SDL_Event *e);

/**
 * displaying message box to the user if the user got checked or the game has ended.
 */
void if_end_game_or_check_handle(Game *game, window_t *game_window);

/**
 * filling the relevant squares with the relevant textures according to possible_moves.
 */
void fill_highlighted_squares_from_possible_moves(game_gui_t *game_gui, PossibleMove possible_moves[32]);

/**
 * drawing the game_gui widget.
 * @param src pointer to a widget containing game_gui_t as its data.
 */
void draw_game_gui(widget_t *src);

/**
 *
 * @return true if the user is currently holding Ctrl.
 */
bool pressing_auto_move_key() {
    return SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RCTRL];
}

/**
 *
 * @return true if the user is currently holding Shift.
 */
bool pressing_double_auto_move_key() {
    return SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RSHIFT];
}

widget_t *create_game_gui(window_t *window, Game *game) {
    widget_t *res = 0;
    game_gui_t *data = 0;

    res = (widget_t *) malloc(sizeof(widget_t));
    if (res == NULL)
        goto HANDLE_ERROR;

    data = (game_gui_t *) calloc(1, sizeof(game_gui_t)); // every bool field is false (calloc put 0)
    if (data == NULL) {
        goto HANDLE_ERROR;
    }
    SDL_Surface *BG_surface = SDL_LoadBMP(DEFAULT_BG_PATH);
    if (BG_surface == NULL) {
        println_error("ERROR: %s", SDL_GetError());
        goto HANDLE_ERROR;
    }
    data->board_BG = SDL_CreateTextureFromSurface(window->renderer, BG_surface);
    if (data->board_BG == NULL) {
        SDL_FreeSurface(BG_surface);
        println_error("ERROR: %s", SDL_GetError());
        goto HANDLE_ERROR;
    }
    data->board_dst_rect.x = 0;
    data->board_dst_rect.y = 0;
    data->board_dst_rect.w = BOARD_W_H;
    data->board_dst_rect.h = BOARD_W_H;

    data->standard_square = create_texture_from_path(STANDARD_SQUARE_PATH, window->renderer);
    if (data->standard_square == NULL) {
        goto HANDLE_ERROR;
    }
    data->threatened_square = create_texture_from_path(THREATENED_SQUARE_PATH, window->renderer);
    if (data->threatened_square == NULL) {
        goto HANDLE_ERROR;
    }
    data->capture_square = create_texture_from_path(CAPTURE_SQUARE_PATH, window->renderer);
    if (data->capture_square == NULL) {
        goto HANDLE_ERROR;
    }
    data->threatened_capture_square = create_texture_from_path(THREATENED_CAPTURE_SQUARE_PATH, window->renderer);
    if (data->threatened_capture_square == NULL) {
        goto HANDLE_ERROR;
    }
    data->focused_square = create_texture_from_path(FOCUSED_SQUARE_PATH, window->renderer);
    if (data->focused_square == NULL) {
        goto HANDLE_ERROR;
    }
    data->focused_square_enemy = create_texture_from_path(FOCUSED_SQUARE_ENEMY_PATH, window->renderer);
    if (data->focused_square_enemy == NULL) {
        goto HANDLE_ERROR;
    }
    char *pieces_paths[NUM_COMBINED_PIECES_TYPES] = {BLACK_BISHOP_PATH, BLACK_KING_PATH,
                                                     BLACK_KNIGHT_PATH, BLACK_PAWN_PATH,
                                                     BLACK_QUEEN_PATH, BLACK_ROOK_PATH,
                                                     WHITE_BISHOP_PATH, WHITE_KING_PATH,
                                                     WHITE_KNIGHT_PATH, WHITE_PAWN_PATH,
                                                     WHITE_QUEEN_PATH, WHITE_ROOK_PATH};
    for (int i = 0; i < NUM_COMBINED_PIECES_TYPES; i++) {
        data->pieces[i] = create_texture_from_path(pieces_paths[i], window->renderer);
        if (data->pieces[i] == NULL) {
            goto HANDLE_ERROR;
        }
    }
    update_game_gui_board(data, game);
    SDL_Rect square_piece_rect;
    SDL_Rect square_rect;
    square_piece_rect.w = PIECE_DEST_W_H;
    square_piece_rect.h = PIECE_DEST_W_H;

    square_rect.w = (int) AVERAGE_SQUARE_WIDTH;
    square_rect.h = (int) AVERAGE_SQUARE_HEIGHT;
    for (int r = 0; r < 8; ++r) {
        square_piece_rect.y = FIRST_PIECE_X_Y + (int) ((7 - r) * AVERAGE_SQUARE_WIDTH);
        square_rect.y = FIRST_SQUARE_X_Y + (int) ((7 - r) * AVERAGE_SQUARE_WIDTH);
        for (int c = 0; c < 8; ++c) {
            square_piece_rect.x = FIRST_PIECE_X_Y + (int) (c * AVERAGE_SQUARE_HEIGHT);
            square_rect.x = FIRST_SQUARE_X_Y + (int) (c * AVERAGE_SQUARE_HEIGHT);
            data->board_square_rects[r][c] = square_rect;
            data->board_pieces_rects[r][c] = square_piece_rect;
        }
    }
    data->is_piece_focused = false;
    data->is_focused_piece_belonging_to_enemy = false;
    res->window = window;
    res->game = game;
    res->destroy = destroy_game_gui;
    res->draw = draw_game_gui;
    res->handle_event = handle_game_gui_event;
    res->data = data;
    return res;

    HANDLE_ERROR:
    println_error("ERROR: problem occurred when trying to create the graphical game board");
    if (data != NULL) {
        for (int i = NUM_COMBINED_PIECES_TYPES - 1; i >= 0; i--) {
            SDL_DestroyTexture(data->pieces[i]);
        }
        SDL_DestroyTexture(data->focused_square_enemy);
        SDL_DestroyTexture(data->focused_square);
        SDL_DestroyTexture(data->threatened_capture_square);
        SDL_DestroyTexture(data->capture_square);
        SDL_DestroyTexture(data->threatened_square);
        SDL_DestroyTexture(data->standard_square);
        SDL_DestroyTexture(data->board_BG);
    }
    free(data);
    free(res);
    return NULL;
}

void update_game_gui_board(game_gui_t *game_gui, Game *game) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            switch (game->board->grid[i][j]) {
                case EMPTY_SPACE:
                    game_gui->curr_pieces[i][j] = NULL;
                    break;
                case BLACK_BISHOP:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[0];
                    break;
                case BLACK_KING:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[1];
                    break;
                case BLACK_KNIGHT:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[2];
                    break;
                case BLACK_PAWN:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[3];
                    break;
                case BLACK_QUEEN:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[4];
                    break;
                case BLACK_ROOK:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[5];
                    break;
                case WHITE_BISHOP:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[6];
                    break;
                case WHITE_KING:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[7];
                    break;
                case WHITE_KNIGHT:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[8];
                    break;
                case WHITE_PAWN:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[9];
                    break;
                case WHITE_QUEEN:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[10];
                    break;
                case WHITE_ROOK:
                    game_gui->curr_pieces[i][j] = game_gui->pieces[11];
                    break;
                default:
                    break;
            }
        }
    }
}

void reset_game_gui(game_gui_t *game_gui, Game *game) {
    if (game->history->count != 1)
        game_gui->undo_button->is_disabled = false;
    update_game_gui_board(game_gui, game);
    game_gui->is_piece_focused = false;
    game_gui->focused_piece_row = -1;
    game_gui->focused_piece_col = -1;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            game_gui->highlighted_squares[i][j] = NULL;
            game_gui->is_possible_move[i][j] = false;
        }
    }
}

void maybe_make_first_computer_turn(Game *game, game_gui_t *game_gui, window_t *window) {
    // Possibly make computer's move, if game just started and computer is white
    if (game->state == GAME_STATE_GAME && game->game_mode == GAME_MODE_SINGLEPLAYER &&
        ((game->current_player == WHITE) != (game->user_color == WHITE))) {
        ComputerMove move = computer_move(game);
        move_was_made(game, move.r1, move.c1, move.r2, move.c2);

//        reset_game_gui(game_gui, game);
        if (game_gui == NULL)
            println_debug("This message is only here to keep the compiler happy while debugging");

        if_end_game_or_check_handle(game, window);
    }
}

void destroy_game_gui(widget_t *src) {
    game_gui_t *data = (game_gui_t *) src->data;
    for (int i = NUM_COMBINED_PIECES_TYPES - 1; i >= 0; i--) {
        SDL_DestroyTexture(data->pieces[i]);
    }
    SDL_DestroyTexture(data->focused_square_enemy);
    SDL_DestroyTexture(data->focused_square);
    SDL_DestroyTexture(data->threatened_capture_square);
    SDL_DestroyTexture(data->capture_square);
    SDL_DestroyTexture(data->threatened_square);
    SDL_DestroyTexture(data->standard_square);
    SDL_DestroyTexture(data->board_BG);
    free(data);
    free(src);
}

void handle_game_gui_event(widget_t *src, SDL_Event *e) {

    game_gui_t *game_gui = (game_gui_t *) src->data;
    SDL_Point mouse_pos = {.x = e->button.x, .y = e->button.y};
    if (src->game->state == GAME_STATE_GAME)
        switch (e->type) {
            case SDL_MOUSEBUTTONUP:
                if (pressing_double_auto_move_key()) {
                    ComputerMove move1 = computer_move(src->game);
                    move_was_made(src->game, move1.r1, move1.c1, move1.r2, move1.c2);
                    ComputerMove move2 = computer_move(src->game);
                    move_was_made(src->game, move2.r1, move2.c1, move2.r2, move2.c2);
                    reset_game_gui(game_gui, src->game);
                    if_end_game_or_check_handle(src->game, src->window);
                    src->game->is_saved = false;
                    break;
                }
                if (pressing_auto_move_key()) {
                    ComputerMove move = computer_move(src->game);
                    move_was_made(src->game, move.r1, move.c1, move.r2, move.c2);
                    reset_game_gui(game_gui, src->game);
                    if_end_game_or_check_handle(src->game, src->window);
                    src->game->is_saved = false;
                    break;
                }
                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        // if we clicked on a highlighted square and the current focused piece belongs to the current player, then make a move.
                        if (game_gui->highlighted_squares[i][j] != NULL &&
                            SDL_PointInRect(&mouse_pos, &game_gui->board_square_rects[i][j]) &&
                            ((is_white_piece(
                                    src->game->board->grid[game_gui->focused_piece_row][game_gui->focused_piece_col]) &&
                              src->game->current_player == WHITE) ||
                             (!is_white_piece(
                                     src->game->board->grid[game_gui->focused_piece_row][game_gui->focused_piece_col]) &&
                              src->game->current_player == BLACK))) {

                            if (console_cmd_move(src->game, game_gui->focused_piece_row, game_gui->focused_piece_col, i,
                                                 j) != SUCCESS) {
                                println_error("Programmer error 16290858162348: %d",
                                              console_cmd_move(src->game, game_gui->focused_piece_row,
                                                               game_gui->focused_piece_col, i,
                                                               j));
                                return;
                            }
                            move_was_made(src->game, game_gui->focused_piece_row, game_gui->focused_piece_col, i, j);
                            reset_game_gui(game_gui, src->game);
                            if_end_game_or_check_handle(src->game, src->window);
                            if (src->game->state == GAME_STATE_GAME && src->game->game_mode == GAME_MODE_SINGLEPLAYER) {
                                ComputerMove move = computer_move(src->game);
                                move_was_made(src->game, move.r1, move.c1, move.r2, move.c2);
                                reset_game_gui(game_gui, src->game);
                                if_end_game_or_check_handle(src->game, src->window);
                            }

                            src->game->is_saved = false;
                            return;
                        }
                        if (game_gui->curr_pieces[i][j] != NULL &&
                            SDL_PointInRect(&mouse_pos, &game_gui->board_pieces_rects[i][j]) &&
                            (i != game_gui->focused_piece_row || j !=
                                                                 game_gui->focused_piece_col)) { // if there is a piece and we clicked there and its not focused.
                            PossibleMove possible_moves[MOVES_ARRAY_SIZE] = {{0}};
                            if (get_possible_moves(src->game->board, i, j, possible_moves) != SUCCESS)
                                return; // not suppose to ever happen cause there is a piece there and its valid row and col
                            reset_game_gui(game_gui, src->game);
                            fill_highlighted_squares_from_possible_moves(game_gui, possible_moves);
                            game_gui->is_piece_focused = true;
                            game_gui->focused_piece_row = i;
                            game_gui->focused_piece_col = j;
                            game_gui->is_focused_piece_belonging_to_enemy = (
                                    is_white_piece(src->game->board->grid[i][j]) !=
                                    (src->game->current_player == WHITE));
                            return;
                        }
                    }
                }
                reset_game_gui(game_gui, src->game);
                break;
            default:
                break;
        }
}

void if_end_game_or_check_handle(Game *game, window_t *game_window) {
    SDL_MessageBoxButtonData buttons[] = {
            {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Ok"}
    };
    if (!pressing_auto_move_key() && !pressing_double_auto_move_key() &&
        check_if_king_is_threatened(game->board, true) &&
        game->current_player == WHITE) {
        show_message_box(game_window, buttons, 1, "Pay Attention!", "White Player - you just got checked!", 0);
    }
    if (!pressing_auto_move_key() && !pressing_double_auto_move_key() &&
        check_if_king_is_threatened(game->board, false) &&
        game->current_player == BLACK) {
        show_message_box(game_window, buttons, 1, "Pay Attention!", "Black Player - you just got checked!", 0);
    }
    if (game->state != GAME_STATE_QUIT)
        return;
    if (game->winner == GAME_WINNER_WHITE) {
        show_message_box(game_window, buttons, 1, "Game Ended", "WHITE WON ! ! !", 0);
    }
    if (game->winner == GAME_WINNER_BLACK) {
        show_message_box(game_window, buttons, 1, "Game Ended", "BLACK WON ! ! !", 0);
    }
    if (game->winner == GAME_WINNER_DRAW) {
        show_message_box(game_window, buttons, 1, "Game Ended", "The game has ended with a draw", 0);
    }
}

void
fill_highlighted_squares_from_possible_moves(game_gui_t *game_gui, PossibleMove possible_moves[MOVES_ARRAY_SIZE]) {
    for (int k = 0; k < MOVES_ARRAY_SIZE; ++k) {
        if (possible_moves[k].is_legal) {
            if (possible_moves[k].is_threatened_by_opponent && possible_moves[k].is_capturing) {
                game_gui->highlighted_squares[possible_moves[k].row][possible_moves[k].col] = game_gui->threatened_capture_square;
                continue;
            }
            if (possible_moves[k].is_threatened_by_opponent) {
                game_gui->highlighted_squares[possible_moves[k].row][possible_moves[k].col] = game_gui->threatened_square;
                continue;
            }
            if (possible_moves[k].is_capturing) {
                game_gui->highlighted_squares[possible_moves[k].row][possible_moves[k].col] = game_gui->capture_square;
                continue;
            }
            game_gui->highlighted_squares[possible_moves[k].row][possible_moves[k].col] = game_gui->standard_square;
        }
    }
}

void draw_game_gui(widget_t *src) {
    game_gui_t *game_gui = (game_gui_t *) src->data;
    SDL_RenderCopy(src->window->renderer, game_gui->board_BG, NULL, &game_gui->board_dst_rect);
    if (game_gui->is_piece_focused) {
        SDL_Texture *texture = game_gui->is_focused_piece_belonging_to_enemy ? game_gui->focused_square_enemy
                                                                             : game_gui->focused_square;
        SDL_RenderCopy(src->window->renderer, texture, NULL,
                       &game_gui->board_square_rects[game_gui->focused_piece_row][game_gui->focused_piece_col]);
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (game_gui->highlighted_squares[i][j] != NULL) {
                SDL_RenderCopy(src->window->renderer, game_gui->highlighted_squares[i][j], NULL,
                               &game_gui->board_square_rects[i][j]);
            }
            if (game_gui->curr_pieces[i][j] != NULL) {
                SDL_RenderCopy(src->window->renderer, game_gui->curr_pieces[i][j], NULL,
                               &game_gui->board_pieces_rects[i][j]);
            }
        }
    }
}