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
typedef struct windows_t windows_t;


// This is the number of shown options at a given time in the slot_options widget (this is not the total number of options ! ! !):
#define NUMBER_OF_DRAWN_OPTIONS 5

typedef enum _GAME_WINDOW {
    MAIN_MENU,
    GAME_MODE,
    ONE_PLAYER_OPTIONS,
    PICK_SLOT,
    GAME,
    QUIT
} GAME_WINDOW;

struct widget_t {
    window_t *window;
    window_t *next_window;
    game_t *game;
    bool is_disabled;
    void (*draw)(struct widget_t *);
    void (*handle_event)(struct widget_t *, SDL_Event *);
    void (*destroy)(struct widget_t *);
    void *data;
};

struct button_t {
    SDL_Texture *texture;
    SDL_Rect location;
    Uint8 current_alpha_factor;
    void (*action)(widget_t *src);
};

struct options_buttons_t {
    widget_t **buttons;
    int num_of_buttons;
    int current_pushed_button;
};

struct slot_options_t {
    SDL_Texture **slots_textures;
    SDL_Texture *arrow_up_tex;
    SDL_Texture *arrow_down_tex;
    SDL_Rect first_slot_location;
    SDL_Rect arrow_up_location;
    SDL_Rect arrow_down_location;
    int num_of_slots;
    int current_top_slot;
    bool is_loading_mode;

    void (*action)(widget_t *src);
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
    window_t *next_window;
    GAME_WINDOW window_enum_val;
    GAME_WINDOW next_window_frame;
    GAME_WINDOW prev_window;
};

struct windows_t {
    window_t *main_menu;
    window_t *game_mode_window;
    window_t *options_window;
    window_t *pick_slot_window;
    window_t *game_window;
};
#endif /* GUI_STRUCTS_H_ */
