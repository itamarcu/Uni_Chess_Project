#ifndef GUI_STRUCTS_H_
#define GUI_STRUCTS_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "ChessGameSettings.h"

typedef struct widget_t widget_t;
typedef struct button_t button_t;
typedef struct options_buttons_t options_buttons_t;
typedef struct slot_options_t slot_options_t;
typedef struct window_t window_t;

// This is the number of shown options at a given time in the slot_options widget (this is not the total number of options ! ! !):
#define NUMBER_OF_DRAWN_OPTIONS 5

typedef enum _GAME_WINDOW {
    MAIN_MENU,
    GAME_MODE,
    ONE_PLAYER_OPTIONS,
    LOAD_GAME,
    GAME,
    QUIT
} GAME_WINDOW;

struct widget_t {
    window_t *window;
    game_t *game;
    void (*draw)(struct widget_t *);
    void (*handle_event)(struct widget_t *, SDL_Event *);
    void (*destroy)(struct widget_t *);
    void *data;
};

struct button_t {
    SDL_Texture *texture;
    SDL_Rect location;
    Uint8 current_alpha_factor;
    GAME_WINDOW next_window;
    void (*action)(widget_t *src);
};

struct options_buttons_t {
    widget_t **buttons;
    int num_of_buttons;
    int current_pushed_button;
};

struct slot_options_t {
    SDL_Texture **options_numbered_textures;
    SDL_Texture *options_textures[NUMBER_OF_DRAWN_OPTIONS];
    SDL_Texture *background_tex;
    SDL_Texture *arrow_up_tex;
    SDL_Texture *arrow_down_tex;
    SDL_Texture *commit_chosen_option_tex;
    int max_options;
    int current_picked_slot;
    int current_top_slot;
};

struct window_t {
    SDL_Window *window;
    int height;
    int width;
    SDL_Renderer *renderer;
    SDL_Texture **textures;
    SDL_Rect **tex_rects;
    struct widget_t **widgets;
    int num_of_tex;
    int num_of_widgets;
    GAME_WINDOW next_window_frame;
    GAME_WINDOW next_window;
    GAME_WINDOW prev_window;
};

typedef struct windows {
    window_t *main_menu;
    window_t *game_mode;
    window_t *options_window;
    window_t *load_game;
    window_t *game_window;
} Windows;
#endif /* GUI_STRUCTS_H_ */
