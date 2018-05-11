#include "SlotOptions.h"

/**
 * Like pow() from math.h
 */
int pow_2(int base, int exp);

/**
 * destroying all the relevant data in src->data assuming its data is slot_options_t *.
 * @param src a pointer to a widget containing slot_options_t * as its data.
 */
void destroy_slot_options(widget_t *src);

/**
 * handling slot_options widget event when assuming src->data its data is slot_options_t *.
 * @param src a pointer to a widget containing slot_options_t * as its data.
 */
void handle_slot_options_event(widget_t *src, SDL_Event *e);

/**
 * drawing slot_options widget when assuming src->data its data is slot_options_t *.
 * @param src a pointer to a widget containing slot_options_t * as its data.
 */
void draw_slot_options(widget_t *src);

widget_t *create_slot_options(
        window_t *window,
        Game *game,
        int number_of_slots,
        int x_pos,
        int y_pos,
        void (*action)(widget_t *, int)) {
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
    if (res == NULL) {
        println_error("ERROR: Could not allocate memory when creating the slots widget");
        return NULL;
    }

    data = (slot_options_t *) malloc(sizeof(slot_options_t));
    if (data == NULL) {
        free(res);
        println_error("ERROR: Could not allocate memory when creating the slots widget");
        return NULL;
    }
    data->slots_textures = (SDL_Texture **) malloc((number_of_slots + 1) * sizeof(SDL_Texture *));
    if (data->slots_textures == NULL) {
        free(data);
        free(res);
        println_error("ERROR: Could not allocate memory when creating the slots widget");
        return NULL;
    }
    // a blank slot without numbering on it.
    blank_slot_surface = SDL_LoadBMP(BLANK_SLOT_PATH);
    if (blank_slot_surface == NULL) {
        goto HANDLE_ERROR;
    }
    // creating all the digits surfaces.
    char *digits_paths[10] = {ZERO_PATH, ONE_PATH, TWO_PATH, THREE_PATH, FOUR_PATH, FIVE_PATH, SIX_PATH, SEVEN_PATH,
                              EIGHT_PATH, NINE_PATH};
    for (int i = 0; i < 10; i++) {
        digits_surfaces[i] = SDL_LoadBMP(digits_paths[i]);
        if (digits_surfaces[i] == NULL) {
            goto HANDLE_ERROR;
        }
    }

    //this is the surface we copy the blank surface into and then the relevant digits surfaces depend on the index.
    numbered_slot_surface = SDL_CreateRGBSurface(0, SLOT_WIDTH, SLOT_HEIGHT, 32, 0, 0, 0, 0);
    if (numbered_slot_surface == NULL) {
        goto HANDLE_ERROR;
    }

    Uint32 map_rgb = SDL_MapRGB(blank_slot_surface->format, 250, 250, 250); //same format for all surfaces
    SDL_Rect slot_rect_dst;
    slot_rect_dst.x = 0;
    slot_rect_dst.y = 0;
    slot_rect_dst.w = SLOT_WIDTH;
    slot_rect_dst.h = SLOT_HEIGHT;
    //this loop will fill the data->slots_textures with numbered textures.
    for (int slot_index = 0; slot_index < number_of_slots; slot_index++) {
        if (SDL_BlitSurface(blank_slot_surface, NULL, numbered_slot_surface, &slot_rect_dst) < 0) {
            goto HANDLE_ERROR;
        }
        int next_digit_x = NUMBERS_X_ON_SLOT;
        int slot_number = slot_index + 1;
        int num_of_digits = 0;
        while (slot_number != 0) {
            num_of_digits++;
            slot_number /= 10;
        }
        slot_number = slot_index + 1;
        int digit = slot_number / pow_2(10, num_of_digits - 1);
        while (num_of_digits != 0) {
            SDL_Rect digit_dst_rect;
            digit_dst_rect.x = next_digit_x;
            digit_dst_rect.y = NUMBERS_Y_ON_SLOT;
            digit_dst_rect.w = digits_surfaces[digit]->w;
            digit_dst_rect.h = digits_surfaces[digit]->h;
            next_digit_x += digit_dst_rect.w;
            if (SDL_BlitSurface(digits_surfaces[digit], NULL, numbered_slot_surface, &digit_dst_rect) < 0) {
                goto HANDLE_ERROR;
            }
            slot_number = slot_number - (pow_2(10, num_of_digits - 1)) * digit;
            if (num_of_digits == 1)
                break;
            num_of_digits--;
            digit = slot_number / pow_2(10, num_of_digits - 1);
        }
        SDL_SetColorKey(numbered_slot_surface, SDL_TRUE, map_rgb);
        data->slots_textures[slot_index] = SDL_CreateTextureFromSurface(window->renderer, numbered_slot_surface);
        if (data->slots_textures[slot_index] == NULL) {
            goto HANDLE_ERROR;
        }
    }

    for (int j = 9; j >= 0; j--) {
        SDL_FreeSurface(digits_surfaces[j]);
    }
    SDL_FreeSurface(numbered_slot_surface);
    SDL_FreeSurface(blank_slot_surface);

    arrow_up_surface = SDL_LoadBMP(ARROW_UP_PATH);
    if (arrow_up_surface == NULL) {
        goto HANDLE_ERROR;
    }
    SDL_SetColorKey(arrow_up_surface, SDL_TRUE, map_rgb);
    arrow_up_texture = SDL_CreateTextureFromSurface(window->renderer, arrow_up_surface);
    if (arrow_up_texture == NULL) {
        goto HANDLE_ERROR;
    }
    SDL_FreeSurface(arrow_up_surface);

    arrow_down_surface = SDL_LoadBMP(ARROW_DOWN_PATH);
    if (arrow_down_surface == NULL) {
        goto HANDLE_ERROR;
    }
    SDL_SetColorKey(arrow_down_surface, SDL_TRUE, map_rgb);
    arrow_down_texture = SDL_CreateTextureFromSurface(window->renderer, arrow_down_surface);
    if (arrow_down_texture == NULL) {
        goto HANDLE_ERROR;
    }
    SDL_FreeSurface(arrow_down_surface);

    data->is_saved_slots = (bool *) calloc((size_t) number_of_slots + 1, sizeof(bool));
    if (data->is_saved_slots == NULL) {
        goto HANDLE_ERROR;
    }
    char slot_num_str[10];
    char full_path[30];
    for (int l = 0; l < number_of_slots; ++l) {
        if (sprintf(slot_num_str, "%d", l + 1) < 0) {
            goto HANDLE_ERROR;
        }
        if (sprintf(full_path, "%s" SEP "%s.save", SAVED_GAMES_DIRECTORY, slot_num_str) < 0) {
            goto HANDLE_ERROR;
        }
        if (does_file_exist(full_path) && !is_file_empty(full_path))
            data->is_saved_slots[l] = true;
        else {
            FILE *f = fopen(full_path, "w");
            if (f == NULL) {
                goto HANDLE_ERROR;
            }
        }
    }
    data->current_slots_alpha_factor = (Uint8 *) calloc((size_t) number_of_slots + 1, sizeof(int));
    if (data->current_slots_alpha_factor == NULL) {
        goto HANDLE_ERROR;
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

    data->num_of_slots = number_of_slots;
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

    HANDLE_ERROR:
    println_error("ERROR: problem occurred when trying to create the slots widget - ,%s", SDL_GetError());
    free(data->current_slots_alpha_factor);
    free(data->is_saved_slots);
    SDL_DestroyTexture(arrow_down_texture);
    SDL_FreeSurface(arrow_down_surface);
    SDL_DestroyTexture(arrow_up_texture);
    SDL_FreeSurface(arrow_up_surface);

    if (data->slots_textures != NULL) {
        for (int i = number_of_slots; i >= 0; i--) {
            SDL_DestroyTexture(data->slots_textures[i]);
        }
    }
    for (int j = 9; j >= 0; j--) {
        SDL_FreeSurface(digits_surfaces[j]);
    }

    SDL_FreeSurface(numbered_slot_surface);
    SDL_FreeSurface(blank_slot_surface);
    free(data->slots_textures);
    free(data);
    free(res);
    return NULL;
}

int pow_2(int base, int exp) {
    int result = 1;
    while (true) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

void destroy_slot_options(widget_t *src) {
    slot_options_t *data = (slot_options_t *) src->data;
    free(data->current_slots_alpha_factor);
    free(data->is_saved_slots);
    SDL_DestroyTexture(data->arrow_up_tex);
    SDL_DestroyTexture(data->arrow_down_tex);
    for (int i = data->num_of_slots - 1; i >= 0; i--) {
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
            for (int i = 0; i < NUMBER_OF_DRAWN_SLOTS; ++i) {
                if (SDL_PointInRect(&mouse_pos, &slot_i_rect)) {
                    slot_options->action(src, i + slot_options->current_top_slot);
                }
                slot_i_rect.y += SLOT_HEIGHT;
            }
            if (slot_options->current_top_slot != 0 &&
                SDL_PointInRect(&mouse_pos, &slot_options->arrow_up_location)) {
                slot_options->current_top_slot--;
            } else {
                if (slot_options->current_top_slot != 0) {
                    slot_options->arrow_up_alpha_factor = 255; // for active button view
                }
            }
            if (slot_options->current_top_slot != slot_options->num_of_slots - NUMBER_OF_DRAWN_OPTIONS &&
                SDL_PointInRect(&mouse_pos, &slot_options->arrow_down_location)) {
                slot_options->current_top_slot++;
            } else {
                if (slot_options->current_top_slot != slot_options->num_of_slots - NUMBER_OF_DRAWN_OPTIONS) {
                    slot_options->arrow_down_alpha_factor = 255; // for active button view
                }
            }
            break;
        case SDL_MOUSEMOTION:
            slot_i_rect = slot_options->first_slot_location;
            for (int i = 0; i < NUMBER_OF_DRAWN_SLOTS; ++i) {
                if (SDL_PointInRect(&mouse_pos, &slot_i_rect)) {
                    slot_options->current_slots_alpha_factor[slot_options->current_top_slot +
                                                             i] = ALPHA_FACTOR_MOUSE_OVER;
                } else
                    slot_options->current_slots_alpha_factor[slot_options->current_top_slot + i] = 255;
                slot_i_rect.y += SLOT_HEIGHT;
            }
            if (SDL_PointInRect(&mouse_pos, &slot_options->arrow_up_location)) {
                slot_options->arrow_up_alpha_factor = ALPHA_FACTOR_MOUSE_OVER;
            } else
                slot_options->arrow_up_alpha_factor = 255;
            if (SDL_PointInRect(&mouse_pos, &slot_options->arrow_down_location)) {
                slot_options->arrow_down_alpha_factor = ALPHA_FACTOR_MOUSE_OVER;
            } else
                slot_options->arrow_down_alpha_factor = 255;
            break;
        default:
            break;
    }
}

void draw_slot_options(widget_t *src) {
    slot_options_t *slot_options = (slot_options_t *) src->data;
    SDL_Rect slot_i_rect = slot_options->first_slot_location;
    int top_slot = slot_options->current_top_slot;
    int top_slot_i;
    for (int i = 0; i < NUMBER_OF_DRAWN_SLOTS; ++i) {
        top_slot_i = top_slot + i;
        if (!slot_options->is_saved_slots[top_slot_i] && slot_options->is_loading_mode)
            slot_options->current_slots_alpha_factor[top_slot_i] = ALPHA_FACTOR_MOUSE_OVER;
        SDL_SetTextureAlphaMod(slot_options->slots_textures[top_slot_i],
                               slot_options->current_slots_alpha_factor[top_slot_i]);
        SDL_RenderCopy(src->window->renderer, slot_options->slots_textures[top_slot_i], NULL,
                       &slot_i_rect);
        slot_i_rect.y += SLOT_HEIGHT;
    }
    if (top_slot == 0) {
        slot_options->arrow_up_alpha_factor = ALPHA_FACTOR_MOUSE_OVER; // for disabled button view.
    }
    if (top_slot == slot_options->num_of_slots - NUMBER_OF_DRAWN_OPTIONS) {
        slot_options->arrow_down_alpha_factor = ALPHA_FACTOR_MOUSE_OVER; // for disabled button view.
    }
    SDL_SetTextureAlphaMod(slot_options->arrow_up_tex, slot_options->arrow_up_alpha_factor);
    SDL_SetTextureAlphaMod(slot_options->arrow_down_tex, slot_options->arrow_down_alpha_factor);
    SDL_RenderCopy(src->window->renderer, slot_options->arrow_up_tex, NULL, &slot_options->arrow_up_location);
    SDL_RenderCopy(src->window->renderer, slot_options->arrow_down_tex, NULL, &slot_options->arrow_down_location);
}