#ifndef OPTIONS_BUTTONS_H_
#define OPTIONS_BUTTONS_H_

#include <SDL.h>
#include <SDL_video.h>
#include "GUIStructs.h"

/**
 *
 * @param buttons a list of buttons containing all the options.
 * only one option at a time can be picked.
 * @return pointer to the a widget, containing options_buttons_t* as its data. returns NULL on error.
 */
widget_t *create_options_buttons(window_t *window, Game *game, widget_t **buttons, int num_of_buttons);

#endif /* OPTIONS_BUTTONS_H_ */

