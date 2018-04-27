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
        window_t *next_window,
        void (*action)(widget_t *src));

void destroy_button(widget_t *src);

void handle_button_event(widget_t *src, SDL_Event *e);

void draw_button(widget_t *src);

void switch_window_and_change_prev_window_action(widget_t *widget);

void switch_to_next_window_action(widget_t *widget);

void switch_to_prev_window_action(widget_t *widget);


#endif /* BUTTON_H_ */

