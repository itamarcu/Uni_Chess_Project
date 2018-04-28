#include "HandleAndDrawGame.h"

widget_t *create_game_gui(
        window_t *window,
        game_t *game) {
    widget_t *res = 0;
    game_gui_t *data = 0;


    res = (widget_t *) malloc(sizeof(widget_t));
    if (res == NULL)
        goto FREE_ON_ERROR;

    data = (game_gui_t *) calloc(1, sizeof(game_gui_t)); // every bool field is false (calloc put 0)
    if (data == NULL) {
        goto FREE_ON_ERROR;
    }

    data->board_BG = create_texture_from_path(DEFAULT_BG_PATH, window->renderer);
    if (data->board_BG == NULL) {
        goto FREE_ON_ERROR;
    }
    data->board_dst_rect.x = 0;
    data->board_dst_rect.y = 0;
    data->board_dst_rect.w = BOARD_W_H;
    data->board_dst_rect.h = BOARD_W_H;

    data->standard_square = create_texture_from_path(STANDARD_SQUARE_PATH, window->renderer);
    if (data->standard_square == NULL) {
        goto FREE_ON_ERROR;
    }
    data->threatened_square = create_texture_from_path(THREATENED_SQUARE_PATH, window->renderer);
    if (data->threatened_square == NULL) {
        goto FREE_ON_ERROR;
    }
    data->capture_square = create_texture_from_path(CAPTURE_SQUARE_PATH, window->renderer);
    if (data->capture_square == NULL) {
        goto FREE_ON_ERROR;
    }
    data->threatened_capture_square = create_texture_from_path(THREATENED_CAPTURE_SQUARE_PATH, window->renderer);
    if (data->threatened_capture_square == NULL) {
        goto FREE_ON_ERROR;
    }
    char *pieces_paths[12] = {BLACK_BISHOP_PATH, BLACK_KING_PATH,
                              BLACK_KNIGHT_PATH, BLACK_PAWN_PATH,
                              BLACK_QUEEN_PATH, BLACK_ROOK_PATH,
                              WHITE_BISHOP_PATH, WHITE_KING_PATH,
                              WHITE_KNIGHT_PATH, WHITE_PAWN_PATH,
                              WHITE_QUEEN_PATH, WHITE_ROOK_PATH};
    for (int i = 0; i < 12; i++) {
        data->pieces[i] = create_texture_from_path(pieces_paths[i], window->renderer);
        if (data->pieces[i] == NULL) {
            goto FREE_ON_ERROR;
        }
    }
    update_game_gui_board(data, game);
    SDL_Rect square_piece_rect;
    SDL_Rect square_rect;
    square_piece_rect.w = PEICE_DEST_W_H;
    square_piece_rect.h = PEICE_DEST_W_H;

    square_rect.w = (int) AVERAGE_SQUARE_WIDTH;
    square_rect.h = (int) AVERAGE_SQUARE_HEIGHT;
    for (int r = 0; r < 8; ++r) {
        square_piece_rect.x = FIRST_PEICE_X_Y + (int) (r * AVERAGE_SQUARE_WIDTH);
        square_rect.x = FIRST_SQUARE_X_Y + (int) (r * AVERAGE_SQUARE_WIDTH);
        for (int c = 0; c < 8; ++c) {
            square_piece_rect.y = FIRST_PEICE_X_Y + (int) (c * AVERAGE_SQUARE_HEIGHT);
            square_rect.y = FIRST_SQUARE_X_Y + (int) (c * AVERAGE_SQUARE_HEIGHT);
            data->board_square_rects[r][c] = square_rect;
            data->board_pieces_rects[r][c] = square_piece_rect;
        }
    }
    data->is_currently_saved = false;
    data->is_piece_focused = false;
    res->window = window;
    res->game = game;
    res->destroy = destroy_game_gui;
    res->draw = draw_game_gui;
    res->handle_event = handle_game_gui_event;
    res->data = data;
    return res;

    FREE_ON_ERROR:
    if (data != NULL) {
        for (int i = 11; i >= 0; i--) {
            SDL_DestroyTexture(data->pieces[i]);
        }
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

void update_game_gui_board(game_gui_t *game_gui, game_t *game) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            switch (game->board->grid[i][j]) {
                case EMPTY_SPACE:
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

void reset_game_gui(game_gui_t *game_gui, game_t *game) {
    update_game_gui_board(game_gui, game);
    game_gui->is_piece_focused = false;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            game_gui->highlighted_squares[i][j] = NULL;
            game_gui->is_possible_move[i][j] = false;
        }
    }
}

void destroy_game_gui(widget_t *src) {
    game_gui_t *data = (game_gui_t *) src->data;
    for (int i = 11; i >= 0; i--) {
        SDL_DestroyTexture(data->pieces[i]);
    }
    SDL_DestroyTexture(data->threatened_capture_square);
    SDL_DestroyTexture(data->capture_square);
    SDL_DestroyTexture(data->threatened_square);
    SDL_DestroyTexture(data->standard_square);
    SDL_DestroyTexture(data->board_BG);
    free(data);
    free(src);
}

void handle_game_gui_event(widget_t *src, SDL_Event *e) {
//    slot_options_t *slot_options = (slot_options_t *) src->data;
//    SDL_Point mouse_pos = {.x = e->button.x, .y = e->button.y};
//    SDL_Rect slot_i_rect;
//    switch (e->type) {
//        case SDL_MOUSEBUTTONUP:
//            slot_i_rect = slot_options->first_slot_location;
//            for (int i = 0; i < NUMBER_OF_DROWN_SLOTS; ++i) {
//                if (SDL_PointInRect(&mouse_pos, &slot_i_rect)) {
//                    char slot_num_str[10];
//                    char full_path[30];
//                    itoa(slot_options->current_top_slot + i, slot_num_str, 10);
//                    if (sprintf(full_path, "%s%s", GAME_SLOTS_PATH, slot_num_str) < 0) {
//                        return; // fatal error to handle.
//                    }
//                    if (slot_options->is_loading_mode && slot_options->is_saved_slots[i]) {
//                        if (load_game_from_slot(src->game, full_path) == false) {
//                            if (show_error_message_box(src->window,
//                                                       "There was an error trying to load the game, please try again") <
//                                0) {
//                                return; // fatal error to handle.
//                            };
//                        } else {
//                            src->game->is_saved = true;
//                            switch_to_next_window_action(src);
//                        }
//                    } else { // saving a game
//                        // check if the slot is not disables because there is not saved game in the slot
//                        if (save_game_to_slot(src->game, full_path) == false) {
//                            if (show_error_message_box(src->window,
//                                                       "There was an error trying to save the game, please try again") <
//                                0) {
//                                return; // fatal error to handle.
//                            };
//                        } else {
//                            src->game->is_saved = true;
//                            slot_options->is_saved_slots[i] = true;
//                            switch_to_next_window_action(src);
//                        }
//                    }
//                }
//                slot_i_rect.y += SLOT_HEIGHT;
//            }
//            break;
//        case SDL_MOUSEMOTION:
//            slot_i_rect = slot_options->first_slot_location;
//            for (int i = 0; i < NUMBER_OF_DROWN_SLOTS; ++i) {
//                if (SDL_PointInRect(&mouse_pos, &slot_i_rect)) {
//
//                }
//            }
////            if (SDL_PointInRect(&mouse_pos, &button->location)) {
////                SDL_SetTextureAlphaMod(button->texture, ALPHA_FACTOR_MOUSE_OVER);
////            } else
////                SDL_SetTextureAlphaMod(button->texture, 255);
//            break;
//        default:
//            break;
//    }
}

void draw_game_gui(widget_t *src) {
    game_gui_t *game_gui = (game_gui_t *) src->data;
    SDL_RenderCopy(src->window->renderer, game_gui->board_BG, NULL, &game_gui->board_dst_rect);

//    struct game_gui_t {
//        SDL_Texture *board_BG;
//        SDL_Texture *standard_square;
//        SDL_Texture *threatened_square;
//        SDL_Texture *capture_square;
//        SDL_Texture *threatened_capture_square;
//        SDL_Texture *pieces[NUM_COMBINED_PIECES_TYPES];
//        SDL_Texture *highlighted_squares[8][8];
//        SDL_Texture *curr_pieces[8][8];
//        SDL_Rect board_square_rects[8][8];
//        SDL_Rect board_pieces_rects[8][8];
//        SDL_Rect board_dst_rect;
//        bool is_piece_focused;
//        int focused_piece_row;
//        int focused_piece_col;
//        bool is_possible_move[8][8];
//        bool is_currently_saved;
//    };

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (game_gui->curr_pieces[i][j] != NULL) {
                SDL_RenderCopy(src->window->renderer, game_gui->curr_pieces[i][j], NULL,
                               &game_gui->board_pieces_rects[i][j]);
            }
        }
    }
//    SDL_Rect slot_i_rect = slot_options->first_slot_location;
//    for (int i = 0; i < NUMBER_OF_DROWN_SLOTS; ++i) {
//        SDL_RenderCopy(src->window->renderer, slot_options->slots_textures[slot_options->current_top_slot + i], NULL,
//                       &slot_i_rect);
//        slot_i_rect.y += SLOT_HEIGHT;
//    }
//    SDL_RenderCopy(src->window->renderer, slot_options->arrow_up_tex, NULL, &slot_options->arrow_up_location);
//    SDL_RenderCopy(src->window->renderer, slot_options->arrow_down_tex, NULL, &slot_options->arrow_down_location);
}