#include "HandleAndDrawGame.h"

widget_t *create_slot_options(
        window_t *window,
        game_t *game,
        int number_of_slots,
        int x_pos,
        int y_pos,
        void (*action)(widget_t *src)) {
    widget_t *res = 0;
    slot_options_t *data = 0;
    SDL_Surface *blank_slot_surface = 0;
    SDL_Surface *digits_surfaces[10] = {0};
    SDL_Surface *numbered_slot_surface = 0;
    SDL_Surface *arrow_up_surface = 0;
    SDL_Texture *arrow_up_texture = 0;
    SDL_Surface *arrow_down_surface = 0;
    SDL_Texture *arrow_down_texture = 0;

    res = (widget_t *) malloc(sizeof(widget_t));
    if (res == NULL)
        goto FREE_ON_ERROR;

    data = (slot_options_t *) malloc(sizeof(slot_options_t));
    if (data == NULL) {
        goto FREE_ON_ERROR;
    }

    blank_slot_surface = SDL_LoadBMP(BLANK_SLOT_PATH);
    if (blank_slot_surface == NULL) {
        goto FREE_ON_ERROR;
    }

    char *digits_paths[10] = {ZERO_PATH, ONE_PATH, TWO_PATH, THREE_PATH, FOUR_PATH, FIVE_PATH, SIX_PATH, SEVEN_PATH,
                              EIGHT_PATH, NINE_PATH};
    for (int i = 0; i < 10; i++) {
        digits_surfaces[i] = SDL_LoadBMP(digits_paths[i]);
        if (digits_surfaces[i] == NULL) {
            goto FREE_ON_ERROR;
        }
    }

    data->slots_textures = (SDL_Texture **) malloc((number_of_slots + 1) * sizeof(SDL_Texture *));
    numbered_slot_surface = SDL_CreateRGBSurface(0, SLOT_WIDTH, SLOT_HEIGHT, 32, 0, 0, 0, 0);
    if (numbered_slot_surface == NULL) {
        goto FREE_ON_ERROR;
    }

    SDL_Rect slot_rect_dst;
    slot_rect_dst.x = 0;
    slot_rect_dst.y = 0;
    slot_rect_dst.w = SLOT_WIDTH;
    slot_rect_dst.h = SLOT_HEIGHT;
    for (int slot_index = 0; slot_index < number_of_slots; slot_index++) {
        if (SDL_BlitSurface(blank_slot_surface, NULL, numbered_slot_surface, &slot_rect_dst) < 0) {
            goto FREE_ON_ERROR;
        }
        int next_digit_x = NUMBERS_X_ON_SLOT;
        int slot_number = slot_index + 1;
        int num_of_digits = 0;
        while (slot_number != 0) {
            num_of_digits++;
            slot_number /= 10;
        }
        slot_number = slot_index + 1;
        int digit = slot_number / (int) pow(10, num_of_digits - 1);
        while (num_of_digits != 0) {
            SDL_Rect digit_dst_rect;
            digit_dst_rect.x = next_digit_x;
            digit_dst_rect.y = NUMBERS_Y_ON_SLOT;
            digit_dst_rect.w = digits_surfaces[digit]->w;
            digit_dst_rect.h = digits_surfaces[digit]->h;
            next_digit_x += digit_dst_rect.w;
            if (SDL_BlitSurface(digits_surfaces[digit], NULL, numbered_slot_surface, &digit_dst_rect) < 0) {
                goto FREE_ON_ERROR;
            }
            slot_number = slot_number - (int) (pow(10, num_of_digits - 1)) * digit;
            if (num_of_digits == 1)
                break;
            num_of_digits--;
            digit = slot_number / (int) pow(10, num_of_digits - 1);
        }
        data->slots_textures[slot_index] = SDL_CreateTextureFromSurface(window->renderer, numbered_slot_surface);
        if (data->slots_textures[slot_index] == NULL) {
            goto FREE_ON_ERROR;
        }
    }

    for (int j = 9; j >= 0; j--) {
        SDL_FreeSurface(digits_surfaces[j]);
    }
    SDL_FreeSurface(numbered_slot_surface);
    SDL_FreeSurface(blank_slot_surface);

    arrow_up_surface = SDL_LoadBMP(ARROW_UP_PATH);
    if (arrow_up_surface == NULL) {
        goto FREE_ON_ERROR;
    }
    arrow_up_texture = SDL_CreateTextureFromSurface(window->renderer, arrow_up_surface);
    if (arrow_up_texture == NULL) {
        goto FREE_ON_ERROR;
    }
    SDL_FreeSurface(arrow_up_surface);

    arrow_down_surface = SDL_LoadBMP(ARROW_DOWN_PATH);
    if (arrow_down_surface == NULL) {
        goto FREE_ON_ERROR;
    }
    arrow_down_texture = SDL_CreateTextureFromSurface(window->renderer, arrow_down_surface);
    if (arrow_down_texture == NULL) {
        goto FREE_ON_ERROR;
    }


    SDL_FreeSurface(arrow_down_surface);

    data->is_saved_slots = (bool *) calloc((size_t) number_of_slots + 1, sizeof(bool));
    if (data->is_saved_slots == NULL) {
        goto FREE_ON_ERROR;
    }
    data->current_slots_alpha_factor = (int *) calloc((size_t) number_of_slots + 1, sizeof(int));
    if (data->current_slots_alpha_factor == NULL) {
        goto FREE_ON_ERROR;
    }
    for (int k = 0; k < number_of_slots; k++) {
        data->current_slots_alpha_factor[k] = 255;
    }
    data->first_slot_location.x = x_pos;
    data->first_slot_location.y = y_pos;
    data->first_slot_location.w = SLOT_WIDTH;
    data->first_slot_location.h = SLOT_HEIGHT;

    data->arrow_up_location.x = x_pos + SLOT_WIDTH;
    data->arrow_up_location.y = y_pos;
    data->arrow_up_location.w = ARROWS_WIDTH;
    data->arrow_up_location.h = ARROWS_HEIGHT;

    data->arrow_down_location.x = data->arrow_up_location.x;
    data->arrow_down_location.y = y_pos + ARROWS_HEIGHT;
    data->arrow_down_location.w = ARROWS_WIDTH;
    data->arrow_down_location.h = ARROWS_HEIGHT;

    data->arrow_up_tex = arrow_up_texture;
    data->arrow_down_tex = arrow_down_texture;
    data->action = action;
    data->current_top_slot = 0;
    res->window = window;
    res->game = game;
    res->destroy = destroy_slot_options;
    res->draw = draw_slot_options;
    res->handle_event = handle_slot_options_event;
    res->data = data;
    return res;

    FREE_ON_ERROR:
    if (data != NULL) {
        free(data->current_slots_alpha_factor);
        free(data->is_saved_slots);
    }
    SDL_DestroyTexture(arrow_down_texture);
    SDL_FreeSurface(arrow_down_surface);
    SDL_DestroyTexture(arrow_up_texture);
    SDL_FreeSurface(arrow_up_surface);

    if (data != NULL && data->slots_textures != NULL) {
        for (int i = number_of_slots; i >= 0; i--) {
            SDL_DestroyTexture(data->slots_textures[i]);
        }
    }
    for (int j = 9; j >= 0; j--) {
        SDL_FreeSurface(digits_surfaces[j]);
    }

    SDL_FreeSurface(numbered_slot_surface);
    SDL_FreeSurface(blank_slot_surface);
    if (data != NULL)
        free(data->slots_textures);
    free(data);
    free(res);
    return NULL;
}

void destroy_slot_options(widget_t *src) {
    slot_options_t *data = (slot_options_t *) src->data;

    SDL_DestroyTexture(data->arrow_up_tex);
    SDL_DestroyTexture(data->arrow_down_tex);
    for (int i = data->num_of_slots; i >= 0; i--) {
        SDL_DestroyTexture(data->slots_textures[i]);
    }
    free(data->slots_textures);
    free(data);
    free(src);
}

void handle_slot_options_event(widget_t *src, SDL_Event *e) {
    slot_options_t *slot_options = (slot_options_t *) src->data;
    SDL_Point mouse_pos = {.x = e->button.x, .y = e->button.y};
    SDL_Rect slot_i_rect;
    switch (e->type) {
        case SDL_MOUSEBUTTONUP:
            slot_i_rect = slot_options->first_slot_location;
            for (int i = 0; i < NUMBER_OF_DROWN_SLOTS; ++i) {
                if (SDL_PointInRect(&mouse_pos, &slot_i_rect)) {
                    char slot_num_str[10];
                    char full_path[30];
                    itoa(slot_options->current_top_slot + i, slot_num_str, 10);
                    if (sprintf(full_path, "%s%s", GAME_SLOTS_PATH, slot_num_str) < 0) {
                        return; // fatal error to handle.
                    }
                    if (slot_options->is_loading_mode && slot_options->is_saved_slots[i]) {
                        if (load_game_from_slot(src->game, full_path) == false) {
                            if (show_error_message_box(src->window,
                                                       "There was an error trying to load the game, please try again") <
                                0) {
                                return; // fatal error to handle.
                            };
                        } else {
                            src->game->is_saved = true;
                            switch_to_next_window_action(src);
                        }
                    } else { // saving a game
                        // check if the slot is not disables because there is not saved game in the slot
                        if (save_game_to_slot(src->game, full_path) == false) {
                            if (show_error_message_box(src->window,
                                                       "There was an error trying to save the game, please try again") <
                                0) {
                                return; // fatal error to handle.
                            };
                        } else {
                            src->game->is_saved = true;
                            slot_options->is_saved_slots[i] = true;
                            switch_to_next_window_action(src);
                        }
                    }
                }
                slot_i_rect.y += SLOT_HEIGHT;
            }
            break;
        case SDL_MOUSEMOTION:
            slot_i_rect = slot_options->first_slot_location;
            for (int i = 0; i < NUMBER_OF_DROWN_SLOTS; ++i) {
                if (SDL_PointInRect(&mouse_pos, &slot_i_rect)) {

                }
            }
//            if (SDL_PointInRect(&mouse_pos, &button->location)) {
//                SDL_SetTextureAlphaMod(button->texture, ALPHA_FACTOR_MOUSE_OVER);
//            } else
//                SDL_SetTextureAlphaMod(button->texture, 255);
            break;
        default:
            break;
    }
}

void draw_slot_options(widget_t *src) {
    slot_options_t *slot_options = (slot_options_t *) src->data;
    SDL_Rect slot_i_rect = slot_options->first_slot_location;
    for (int i = 0; i < NUMBER_OF_DROWN_SLOTS; ++i) {
        SDL_RenderCopy(src->window->renderer, slot_options->slots_textures[slot_options->current_top_slot + i], NULL,
                       &slot_i_rect);
        slot_i_rect.y += SLOT_HEIGHT;
    }
    SDL_RenderCopy(src->window->renderer, slot_options->arrow_up_tex, NULL, &slot_options->arrow_up_location);
    SDL_RenderCopy(src->window->renderer, slot_options->arrow_down_tex, NULL, &slot_options->arrow_down_location);
}