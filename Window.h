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

/**
 * Destroys a window and frees anything in it.
 */
void destroy_window(window_t *src);

/**
 * Handles an SDL_Event on a window and all widgets inside it.
 *
 * For example, this handles the X (exit) button on the corner.
 * @param src
 * @param event
 * @return next window frame of the program.
 */
GAME_WINDOW handle_window_event(window_t *src, SDL_Event *event);

/**
 * Draws a window on the screen (includes a small time delay).
 */
void draw_window(window_t *src);

/**
 * Creates an SDL texture from a path.
 *
 * Can fail if there's no such file or if it's in the wrong format.
 *
 * @param texture_path relative or full path to texture.
 * @param renderer renderer for the texture.
 * @return pointer to new texture object, or NULL (+error message) if there was a failure.
 */
SDL_Texture *create_texture_from_path(char *texture_path, SDL_Renderer *renderer);

/**
 * Adds a texture and its rectangle shape to the list of textures in the window.
 */
void add_texture_to_window(window_t *src, SDL_Texture *texture, SDL_Rect *rect);

/**
 * Adds a widget to a window.
 */
void add_widget_to_window(window_t *src, struct widget_t *widget);

/**
 * Adds the background chess image to a window, and the title text image.
 * @param title_image_path path to image of the title text.
 * @return 0 on success, -1 on fail
 */
int add_chess_BG_and_title(window_t *src, char *title_image_path);

/**
 * Creates a new window with nothing inside it (and the appropriate attributes).
 */
window_t *create_empty_centered_window(int window_width, int window_height, int num_of_textures, int num_of_widgets,
                                       GAME_WINDOW game_window);

/**
 * Adds a "back" button to a window. This button will go to the previous window.
 *
 * @return 0 on success, -1 on fail
 */
int add_back_button_to_window(window_t *window, Game *game);

/**
 * Replaces the current window with a new window at a specified position.
 */
void show_window_at_pos(window_t *next_window, int window_posX, int window_posY);

/**
 * Shows a message box containing text and several buttons. When the message box is pressed (or destroyed),
 * this function returns with the pressed button as output.
 *
 * The `default_option` parameter is used in case the `NO_MESSAGE_BOX` global flag is turned on.
 * In that case, no message box will be shown, and instead the function will return with default_option as the output.
 *
 * @param window origin window
 * @param buttons array of buttons to be shown
 * @param num_of_buttons amount of buttons in the array
 * @param box_title title of the message box
 * @param message message within the message box
 * @param default_option identifier number of the default pressed button.
 * @return identifier number of pressed button, or -1 if there was an error.
 */
int show_message_box(window_t *window, SDL_MessageBoxButtonData *buttons, int num_of_buttons, const char *box_title,
                     const char *message, int default_option);

/**
 * Shows an error message box. Also prints the error to the console.
 * The window only has an "OK" button on it.
 *
 * This function should only be called for unexpected errors.
 *
 * @param window origin window
 * @param message message to put on the message box
 * @return 1 if the error message was correctly shown and the button was pressed,
 * or a negative number in case of a failure.
 */
int show_error_message_box(window_t *window, const char *message);

#endif /* WINDOW_H_ */
