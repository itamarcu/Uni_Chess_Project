#ifndef OPTIONS_BUTTONS_H_
#define OPTIONS_BUTTONS_H_

#include <SDL.h>
#include <SDL_video.h>
#include "GUIStructs.h"

widget_t *create_options_buttons(window_t *window, Game *game, widget_t **buttons, int num_of_buttons);

void destroy_options_buttons(widget_t *src);

void handle_options_buttons_event(widget_t *src, SDL_Event *e);

void draw_options_buttons(widget_t *src);

#endif /* OPTIONS_BUTTONS_H_ */

