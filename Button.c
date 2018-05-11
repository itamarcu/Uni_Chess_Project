#include "Button.h"


void handle_button_event(widget_t *src, SDL_Event *e);

widget_t *create_button(
        window_t *window,
        Game *game,
        const char *image,
        SDL_Rect location,
        window_t *next_window,
        void (*action)(widget_t *)) {
    widget_t *res = (widget_t *) malloc(sizeof(widget_t));
    if (res == NULL)
        goto PRINT_ERROR_RETURN_NULL;

    button_t *data = (button_t *) malloc(sizeof(button_t));
    if (data == NULL) {
        free(res);
        goto PRINT_ERROR_RETURN_NULL;
    }

    SDL_Surface *surface = SDL_LoadBMP(image);
    if (surface == NULL) {
        free(data);
        free(res);
        println_error("ERROR: %s", SDL_GetError());
        goto PRINT_ERROR_RETURN_NULL;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(window->renderer, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        free(data);
        free(res);
        println_error("ERROR: %s", SDL_GetError());
        goto PRINT_ERROR_RETURN_NULL;
    }

    // surface is not needed after texture is created
    SDL_FreeSurface(surface);

    // store button & widget details
    data->texture = texture;
    data->location = location;
    data->action = action;
    data->current_alpha_factor = 255;

    res->window = window;
    res->next_window = next_window;
    res->game = game;
    res->is_disabled = false;
    res->destroy = destroy_button;
    res->draw = draw_button;
    res->handle_event = handle_button_event;
    res->data = data;
    return res;

    PRINT_ERROR_RETURN_NULL:

    println_error("ERROR: problem occurred when trying to create a button");
    return NULL;
}

void destroy_button(widget_t *src) {
    button_t *button = (button_t *) src->data;
    SDL_DestroyTexture(button->texture);
    free(button);
    free(src);
}

void handle_button_event(widget_t *src, SDL_Event *e) {
    if (src->is_disabled)
        return;
    button_t *button = (button_t *) src->data;
    SDL_Point mouse_pos = {.x = e->button.x, .y = e->button.y};
    switch (e->type) {
        case SDL_MOUSEBUTTONUP:
            if (SDL_PointInRect(&mouse_pos, &button->location)) {
                button->action(src);
                button->current_alpha_factor = ALPHA_FACTOR_MOUSE_OVER;
            }
            break;
        default:
            if (SDL_PointInRect(&mouse_pos, &button->location)) {
                button->current_alpha_factor = ALPHA_FACTOR_MOUSE_OVER;
            } else
                button->current_alpha_factor = 255;

            break;
    }
}

void draw_button(widget_t *src) {
    button_t *button = (button_t *) src->data;
    if (src->is_disabled) {
        button->current_alpha_factor = 255;
        SDL_SetTextureAlphaMod(button->texture, 255);
        SDL_RenderCopy(src->window->renderer, button->texture, NULL, &button->location);
    } else {
        SDL_SetTextureAlphaMod(button->texture, button->current_alpha_factor);
        SDL_RenderCopy(src->window->renderer, button->texture, NULL, &button->location);
    }
}

void switch_window_and_change_prev_window_action(widget_t *widget) {
    widget->window->next_window_frame = widget->next_window->window_enum_val;
    widget->next_window->prev_window = widget->window->window_enum_val;
}

void switch_to_next_window_action(widget_t *widget) {
    if (widget->window->next_window != NULL) {
        widget->window->next_window_frame = widget->window->next_window->window_enum_val;
        widget->window->next_window->prev_window = widget->window->window_enum_val;
    } else
        widget->window->next_window_frame = QUIT;
}

void switch_to_prev_window_action(widget_t *widget) {
    widget->window->next_window_frame = widget->window->prev_window;
}
