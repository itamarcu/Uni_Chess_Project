#include "Button.h"

widget_t *create_button(
        window_t *window,
        game_t *game,
        const char *image,
        SDL_Rect location,
        GAME_WINDOW next_window,
        void (*action)(widget_t *src)) {
    // allocate data
    widget_t *res = (widget_t *) malloc(sizeof(widget_t));
    if (res == NULL)
        return NULL;

    button_t *data = (button_t *) malloc(sizeof(button_t));
    if (data == NULL) {
        free(res);
        return NULL;
    }

    // we use the surface as a temp var
    SDL_Surface *surface = SDL_LoadBMP(image);
    if (surface == NULL) {
        free(res);
        free(data);
        return NULL;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(window->renderer, surface);
    if (texture == NULL) {
        free(res);
        free(data);
        SDL_FreeSurface(surface);
        return NULL;
    }

    // surface is not needed after texture is created
    SDL_FreeSurface(surface);

    // store button & widget details
    data->texture = texture;
    data->location = location;
    data->next_window = next_window;
    data->action = action;
    res->window = window;
    res->game = game;
    res->destroy = destroy_button;
    res->draw = draw_button;
    res->handle_event = handle_button_event;
    res->data = data;
    return res;
}

void destroy_button(widget_t *src) {
    button_t *button = (button_t *) src->data;
    SDL_DestroyTexture(button->texture);
    free(button);
    free(src);
}

void handle_button_event(widget_t *src, SDL_Event *e) {
    button_t *button = (button_t *) src->data;
    SDL_Point mouse_pos = {.x = e->button.x, .y = e->button.y};
    switch (e->type) {
        case SDL_MOUSEBUTTONUP:
            if (SDL_PointInRect(&mouse_pos, &button->location)) {
                button->action(src);
            }
            break;
        case SDL_MOUSEMOTION:
            if (SDL_PointInRect(&mouse_pos, &button->location)) {
                SDL_SetTextureAlphaMod(button->texture, ALPHA_FACTOR_MOUSE_OVER);
            } else
                SDL_SetTextureAlphaMod(button->texture, 255);
            break;
        default:
            break;
    }
}

void draw_button(widget_t *src) {
    button_t *button = (button_t *) src->data;
    SDL_RenderCopy(src->window->renderer, button->texture, NULL, &button->location);
}

void switch_window_action(widget_t *widget) {
    widget->window->next_window = ((button_t *) widget->data)->next_window;
}

widget_t *create_button_switch_between_windows(
        window_t *window,
        game_t *game,
        const char *image,
        SDL_Rect location,
        GAME_WINDOW next_window) {

    return create_button(window, game, image, location, next_window, switch_window_action);
}

