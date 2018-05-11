#ifndef BUTTON_H_
#define BUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include "GUIStructs.h"
#include "Auxiliary.h"

#define ALPHA_FACTOR_MOUSE_OVER 170

/**
 *
 * @param window the relevant window_t of the button
 * @param image the path to the image of the button
 * @param next_window if the button will change the window with switch_window_and_change_prev_window_action
 * it will change to next_window.
 * @param the action to make when clicking on the button
 * @return pointer to a widget containing button_t* as data. returns NULL on error.
 */
widget_t *create_button(
        window_t *window,
        Game *game,
        const char *image,
        SDL_Rect location,
        window_t *next_window,
        void (*action)(widget_t *src));

/**
 * destroying the widget of the button.
 * @param src a pointer to a widget containing button_t* as its data.
 */

void destroy_button(widget_t *src);

/**
 * drawing the button in its location in its window.
 * @param src a pointer to a widget containing button_t* as its data.
 */
void draw_button(widget_t *src);

/**
 * switching to the window in src->next_window and changing its previous window to be src->window.
 * @param src a pointer to a widget containing button_t* as its data.
 */
void switch_window_and_change_prev_window_action(widget_t *src);

/**
 * switching to src->window->next_window or quiting if window->next_window_frame == QUIT.
 * @param src a pointer to a widget containing button_t* as its data.
 */
void switch_to_next_window_action(widget_t *src);

/**
 * switching to src->window->prev_window
 * @param src a pointer to a widget containing button_t* as its data.
 */
void switch_to_prev_window_action(widget_t *src);


#endif /* BUTTON_H_ */

