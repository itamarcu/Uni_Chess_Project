#ifndef GUI_STRUCTS_H_
#define GUI_STRUCTS_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "ChessGameSettings.h"

typedef struct widget_t Widget;
typedef struct button_t Button;
typedef struct window_t Window;

typedef enum _GAME_WINDOW {
    MAIN_MENU,
    GAME_MODE,
    ONE_PLAYER_OPTIONS,
    LOAD_GAME,
    GAME,
    QUIT
} GAME_WINDOW;

struct widget_t {
    Window *window;
    Game *game;
    void (*draw)(struct widget_t *);
    void (*handleEvent)(struct widget_t *, SDL_Event *);
    void (*destroy)(struct widget_t *);
    void *data;
};

struct button_t {
    SDL_Texture *texture;
    SDL_Rect location;
    GAME_WINDOW next_window;

    void (*action)(Widget *src);
};

struct window_t {
    SDL_Window *window;
    int height;
    int width;
    SDL_Renderer *renderer;
    SDL_Texture **textures;
    SDL_Rect **texRects;
    struct widget_t **widgets;
    int numOfTex;
    int numOfWidgets;
    bool isShown;
    GAME_WINDOW nextWindow;
};

typedef struct windows {
    Window *main_menu;
    Window *game_mode;
    Window *one_player_options;
    Window *load_game;
    Window *game_window;
} Windows;
#endif /* GUI_STRUCTS_H_ */
