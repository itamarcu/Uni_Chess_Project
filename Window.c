#include "Window.h"
#include <stdio.h>
#include <stdlib.h>

void destroyWindow(Window *src) {
    int i;
    for (i = 0; i < src->numOfWidgets; i++) {
        destroyWidget(src->widgets[i]);
    }
    for (i = 0; i < src->numOfTex; i++) {
        free(src->texRects[i]);
    }
    for (i = 0; i < src->numOfTex; i++) {
        SDL_DestroyTexture(src->textures[i]);
    }
    free(src->widgets);
    free(src->texRects);
    free(src->textures);
    SDL_DestroyRenderer(src->renderer);
    SDL_DestroyWindow(src->window);
    free(src);
}

GAME_WINDOW handleWindowEvent(Window *src, SDL_Event *e) {
    int i;
    Widget *widget;
    for (i = 0; i < src->numOfWidgets; i++) {
        widget = src->widgets[i];
        widget->handleEvent(widget, e);
    }
    switch (e->type) {
        case SDL_QUIT:
            src->nextWindow = QUIT;
            break;
        case SDL_KEYDOWN:
            switch (e->key.keysym.sym) {
                case SDLK_ESCAPE:
                    src->nextWindow = QUIT;
                    break;
                default:
                    break;
            }
        default:
            break;
    }
    return src->nextWindow;
}

void drawWindow(Window *src) {
    int i;
    Widget *widget;
    SDL_Texture *texture;
    SDL_Rect *texRect;
    SDL_Renderer *renderer = src->renderer;
    SDL_RenderClear(renderer);
    for (i = 0; i < src->numOfTex; i++) {
        texture = src->textures[i];
        texRect = src->texRects[i];
        SDL_RenderCopy(renderer, texture, NULL, texRect);
    }
    for (i = 0; i < src->numOfWidgets; i++) {
        widget = src->widgets[i];
        widget->draw(widget);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

void addTextureToWindow(Window *src, SDL_Texture *texture, SDL_Rect *rect) {
    src->textures[src->numOfTex] = texture;
    src->texRects[src->numOfTex] = rect;
    src->numOfTex++;
}

void addWidgetToWindow(Window *src, Widget *widget) {
    src->widgets[src->numOfWidgets] = widget;
    src->numOfWidgets++;
}

void addChessBGAndTitle(Window *src, char *title_path) {
    SDL_Renderer *rend = src->renderer;
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    SDL_Surface *background_surface = SDL_LoadBMP(DEFAULT_BG_PATH);
    if (background_surface == NULL) {
        printf("ERROR: unable to load image: %s\n", SDL_GetError());
        destroyWindow(src);
        return;
    }
    SDL_Texture *background_texture = SDL_CreateTextureFromSurface(rend, background_surface);
    if (background_texture == NULL) {
        printf("ERROR: unable to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(background_surface);
        destroyWindow(src);
        return;
    }
    SDL_FreeSurface(background_surface);


    SDL_Surface *title_surface = SDL_LoadBMP(title_path);
    if (title_surface == NULL) {
        printf("ERROR: unable to load image: %s\n", SDL_GetError());
        SDL_DestroyTexture(background_texture);
        destroyWindow(src);
        return;
    }
    SDL_SetColorKey(title_surface, SDL_TRUE, SDL_MapRGB(title_surface->format, 255, 255, 255));
    SDL_Texture *title_texture = SDL_CreateTextureFromSurface(rend, title_surface);
    if (title_texture == NULL) {
        printf("ERROR: unable to create texture: %s\n", SDL_GetError());
        SDL_DestroyTexture(background_texture);
        SDL_FreeSurface(title_surface);
        destroyWindow(src);
        return;
    }
    SDL_FreeSurface(title_surface);

    SDL_Rect *title_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    int title_width, title_height = 0;
    SDL_QueryTexture(title_texture, NULL, NULL, &title_width, &title_height);
    title_rect->x = (src->width - title_width) / 2;
    title_rect->y = (src->height - title_height) / 10;
    title_rect->w = DEFAULT_TITLE_WIDTH;
    title_rect->h = DEFAULT_TITLE_HEIGHT;

    addTextureToWindow(src, background_texture, NULL);
    addTextureToWindow(src, title_texture, title_rect);
}

Window *createEmptyCenteredWindow(int window_width, int window_height, int num_of_textures, int num_of_widgets,
                                  GAME_WINDOW game_window) {
    Window *window = (Window *) malloc(sizeof(Window));
    window->window = SDL_CreateWindow(
            "Main_Menu",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width,
            window_height,
            SDL_WINDOW_OPENGL);
    if (window->window == NULL) {
        printf("ERROR: unable to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }
    SDL_HideWindow(window->window);
    window->height = window_height;
    window->width = window_width;
    window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (window->renderer == NULL) {
        printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window->window);
        SDL_Quit();
        return NULL;
    }
    window->textures = (SDL_Texture **) malloc((num_of_textures + 1) * sizeof(SDL_Texture *));
    window->texRects = (SDL_Rect **) malloc((num_of_textures + 1) * sizeof(SDL_Rect *));
    window->widgets = (Widget **) malloc((num_of_widgets + 1) * sizeof(Widget *));

    window->numOfTex = 0;
    window->numOfWidgets = 0;
    window->isShown = true;
    window->nextWindow = game_window;
    return window;
}

void addBackButtonToWindow(Window *window, GAME_WINDOW back_window) {
    SDL_Rect new_game_button_rect;
    new_game_button_rect.x = 0;
    new_game_button_rect.y = window->height - DEFAULT_BACK_BUTTON_HEIGHT;
    new_game_button_rect.w = DEFAULT_BACK_BUTTON_WIDTH;
    new_game_button_rect.h = DEFAULT_BACK_BUTTON_HEIGHT;
    Widget *back_button = createButtonSwitchBetweenWindows(window, DEFAULT_BACK_BUTTON_PATH, new_game_button_rect,
                                                           back_window);
    addWidgetToWindow(window, back_button);
}
