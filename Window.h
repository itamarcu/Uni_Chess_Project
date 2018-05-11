#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIStructs.h"
#include "Widget.h"
#include "Button.h"
#include "Auxiliary.h"


#define DEFAULT_BG_PATH (PICTURES_DIRECTORY SEP "chessboard.bmp")
#define DEFAULT_BACK_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_back.bmp")
#define DEFAULT_BACK_BUTTON_WIDTH 116
#define DEFAULT_BACK_BUTTON_HEIGHT 40

void destroy_window(window_t *src);

GAME_WINDOW handle_window_event(window_t *src, SDL_Event *event);

void draw_window(window_t *src);

SDL_Texture *create_texture_from_path(char *tex_path, SDL_Renderer *rend);

void add_texture_to_window(window_t *src, SDL_Texture *texture, SDL_Rect *rect);

void add_widget_to_window(window_t *src, struct widget_t *widget);

void add_chess_BG_and_title(window_t *src, char *title_path);

window_t *create_empty_centered_window(int window_width, int window_height, int num_of_textures, int num_of_widgets,
                                     GAME_WINDOW game_window);

void add_back_button_to_window(window_t *window, Game *game);

void show_window_at_pos(window_t *next_window, int window_posX, int window_posY);

int show_message_box(window_t *window, SDL_MessageBoxButtonData *buttons, int num_of_buttons, const char *box_title,
                     const char *message);

int show_error_message_box(window_t *window, const char *message);

#endif /* WINDOW_H_ */
