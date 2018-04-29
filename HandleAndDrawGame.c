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
    data->focused_square = create_texture_from_path(FOCUSED_SQUARE_PATH, window->renderer);
    if (data->focused_square == NULL) {
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
        square_piece_rect.y = FIRST_PEICE_X_Y + (int) ((7 - r) * AVERAGE_SQUARE_WIDTH);
        square_rect.y = FIRST_SQUARE_X_Y + (int) ((7 - r) * AVERAGE_SQUARE_WIDTH);
        for (int c = 0; c < 8; ++c) {
            square_piece_rect.x = FIRST_PEICE_X_Y + (int) (c * AVERAGE_SQUARE_HEIGHT);
            square_rect.x = FIRST_SQUARE_X_Y + (int) (c * AVERAGE_SQUARE_HEIGHT);
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

void reset_game_gui(game_gui_t *game_gui, game_t *game) {
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

    game_gui_t *game_gui = (game_gui_t *) src->data;
    SDL_Point mouse_pos = {.x = e->button.x, .y = e->button.y};
    switch (e->type) {
        case SDL_MOUSEBUTTONUP:
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (game_gui->highlighted_squares[i][j] != NULL &&
                        SDL_PointInRect(&mouse_pos, &game_gui->board_square_rects[i][j])) {
                        if (console_cmd_move(src->game, game_gui->focused_piece_row, game_gui->focused_piece_col, i,
                                             j) != SUCCESS) {
                            return;
                        }
                        move_was_made(src->game, game_gui->focused_piece_row, game_gui->focused_piece_col, i, j);
                        if (src->game->game_mode == GAME_MODE_SINGLEPLAYER) {
                            ComputerMove move = computer_move(src->game);
                            //move_was_made(src->game, move.r1, move.c1, move.r2, move.c2);
                        }
                        reset_game_gui(game_gui, src->game);
                        return;
                    }
                    if (game_gui->curr_pieces[i][j] != NULL &&
                        SDL_PointInRect(&mouse_pos, &game_gui->board_pieces_rects[i][j]) &&
                        (i != game_gui->focused_piece_row || j !=
                                                             game_gui->focused_piece_col)) { // if there is a piece and we clicked there and its not focused.
                        possible_move_t possible_moves[MOVES_ARRAY_SIZE] = {0};
                        if ((src->game->current_player == WHITE && is_white_piece(src->game->board->grid[i][j])) ||
                            (src->game->current_player == BLACK && !is_white_piece(src->game->board->grid[i][j]))) {
                            // if the piece is white and its white turn or black piece and its black turn.
                            if (get_possible_moves(src->game->board, i, j, possible_moves) != SUCCESS)
                                return; // not suppose to ever happan cause there is a piece there and its valid row and col
                            reset_game_gui(game_gui, src->game);
                            fill_highlighted_squares_from_possible_moves(game_gui, possible_moves);
                            game_gui->is_piece_focused = true;
                            game_gui->focused_piece_row = i;
                            game_gui->focused_piece_col = j;
                            return;
                        }
                    }
                }
            }
            reset_game_gui(game_gui, src->game);
            break;
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
        default:
            break;
    }
}

void
fill_highlighted_squares_from_possible_moves(game_gui_t *game_gui, possible_move_t possible_moves[MOVES_ARRAY_SIZE]) {
    for (int k = 0; k < MOVES_ARRAY_SIZE; ++k) {
        if (possible_moves[k].is_possible) {
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
        SDL_RenderCopy(src->window->renderer, game_gui->focused_square, NULL,
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
//    SDL_Rect slot_i_rect = slot_options->first_slot_location;
//    for (int i = 0; i < NUMBER_OF_DROWN_SLOTS; ++i) {
//        SDL_RenderCopy(src->window->renderer, slot_options->slots_textures[slot_options->current_top_slot + i], NULL,
//                       &slot_i_rect);
//        slot_i_rect.y += SLOT_HEIGHT;
//    }
//    SDL_RenderCopy(src->window->renderer, slot_options->arrow_up_tex, NULL, &slot_options->arrow_up_location);
//    SDL_RenderCopy(src->window->renderer, slot_options->arrow_down_tex, NULL, &slot_options->arrow_down_location);
}