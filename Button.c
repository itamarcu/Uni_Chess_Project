#include "Button.h"
#include <stdlib.h>

Widget *createButton(
        Window *window,
        const char *image,
        SDL_Rect location,
        GAME_WINDOW next_window,
        void (*action)(Widget *src)) {
    // allocate data
    Widget *res = (Widget *) malloc(sizeof(Widget));
    if (res == NULL)
        return NULL;

    Button *data = (Button *) malloc(sizeof(Button));
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
    res->destroy = destroyButton;
    res->draw = drawButton;
    res->handleEvent = handleButtonEvent;
    res->data = data;
    return res;
}

void destroyButton(Widget *src) {
    Button *button = (Button *) src->data;
    SDL_DestroyTexture(button->texture);
    free(button);
    free(src);
}

void handleButtonEvent(Widget *src, SDL_Event *e) {
    Button *button = (Button *) src->data;
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

void drawButton(Widget *src) {
    Button *button = (Button *) src->data;
    SDL_RenderCopy(src->window->renderer, button->texture, NULL, &button->location);
}

void switchWindowAction(Widget *widget) {
    widget->window->nextWindow = ((Button *) widget->data)->next_window;
}

Widget *createButtonSwitchBetweenWindows(
        Window *window,
        const char *image,
        SDL_Rect location,
        GAME_WINDOW next_window) {

    return createButton(window, image, location, next_window, switchWindowAction);
}

