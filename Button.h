#ifndef BUTTON_H_
#define BUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include "GUIStructs.h"
//#include "widget_t.h"

#define ALPHA_FACTOR_MOUSE_OVER 200

widget_t *create_button(
        window_t *window,
        game_t *game,
        const char *image,
        SDL_Rect location,
        GAME_WINDOW next_window,
        void (*action)(widget_t *src));

void destroy_button(widget_t *src);

void handle_button_event(widget_t *src, SDL_Event *event);

void draw_button(widget_t *widget);

void switch_window_action(widget_t *widget);

widget_t *create_button_switch_between_windows(
        window_t *window,
        game_t *game,
        const char *image,
        SDL_Rect location,
        GAME_WINDOW next_window);

#endif /* BUTTON_H_ */
