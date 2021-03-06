#ifndef GUI_STRUCTS_H_
#define GUI_STRUCTS_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "ChessGameSettings.h"

#define NUM_COMBINED_PIECES_TYPES 12

typedef struct widget_t widget_t;
typedef struct button_t button_t;
typedef struct options_buttons_t options_buttons_t;
typedef struct slot_options_t slot_options_t;
typedef struct game_gui_t game_gui_t;
typedef struct window_t window_t;
typedef struct windows_t windows_t;


// This is the number of shown options at a given time in the slot_options widget (this is not the total number of options ! ! !):
#define NUMBER_OF_DRAWN_OPTIONS 5

/**
 * all the differnt windows in enum.
 */
typedef enum _GAME_WINDOW {
    MAIN_MENU,
    GAME_MODE,
    ONE_PLAYER_OPTIONS,
    PICK_SLOT,
    GAME,
    QUIT
} GAME_WINDOW;

/**
 * a widget can contain different datas. it can be button_t, game_gui_t, options_buttons_t, and slot_options_t.
 * every kind of widget should fill the widget drawing function, handle event function and destroy function.
 * if is_disabled == true then the widget should do nothing at handle_event.
 */
struct widget_t {
    window_t *window;
    window_t *next_window;
    Game *game;
    bool is_disabled;
    void (*draw)(struct widget_t *);
    void (*handle_event)(struct widget_t *, SDL_Event *);
    void (*destroy)(struct widget_t *);
    void *data;
};

/**
 * a button widget data.
 */
struct button_t {
    SDL_Texture *texture;
    SDL_Rect location;
    Uint8 current_alpha_factor;
    void (*action)(widget_t *src);
};

/**
 * options buttons widget data. just a list of buttons that only one can be picked at a time.
 */
struct options_buttons_t {
    widget_t **buttons;
    int num_of_buttons;
    int current_pushed_button;
};

/**
 * slot options widget data. slot_textures will contain all the relevant numbered textures according num_of_slots
 * using the digits pictures.
 */
struct slot_options_t {
    SDL_Texture **slots_textures;
    int num_of_slots;
    int current_top_slot;
    SDL_Texture *arrow_up_tex;
    SDL_Texture *arrow_down_tex;
    SDL_Rect first_slot_location;
    SDL_Rect arrow_up_location;
    SDL_Rect arrow_down_location;
    bool is_loading_mode;
    bool *is_saved_slots;

    Uint8 *current_slots_alpha_factor;
    Uint8 arrow_up_alpha_factor;
    Uint8 arrow_down_alpha_factor;
    void (*action)(widget_t *src, int clicked_index);
};

/**
 * all the relevant data for the widget that drawing and handling the game.
 */
struct game_gui_t {
    SDL_Texture *board_BG;
    SDL_Texture *standard_square;
    SDL_Texture *threatened_square;
    SDL_Texture *capture_square;
    SDL_Texture *threatened_capture_square;
    SDL_Texture *pieces[NUM_COMBINED_PIECES_TYPES];
    SDL_Texture *highlighted_squares[8][8];
    SDL_Texture *curr_pieces[8][8];
    SDL_Rect board_square_rects[8][8];
    SDL_Rect board_pieces_rects[8][8];
    SDL_Rect board_dst_rect;
    bool is_piece_focused;
    bool is_focused_piece_belonging_to_enemy;
    SDL_Texture *focused_square;
    SDL_Texture *focused_square_enemy;
    int focused_piece_row;
    int focused_piece_col;
    bool is_possible_move[8][8];
    widget_t *undo_button;
};
/**
 * all the relevant window data. textures to draw, widgets with there locations to draw and handle, and more.
 */
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
    windows_t *windows;
};
/**
 * all the windows in the game.
 */
struct windows_t {
    window_t *main_menu;
    window_t *game_mode_window;
    window_t *options_window;
    window_t *pick_slot_window;
    window_t *game_window;
};
#endif /* GUI_STRUCTS_H_ */
