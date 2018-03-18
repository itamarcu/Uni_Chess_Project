#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIStructs.h"
#include "Widget.h"
#include "Button.h"


#define DEFAULT_BG_PATH "..\\pictures\\chessboard.bmp"
#define DEFAULT_TITLE_WIDTH 154
#define DEFAULT_TITLE_HEIGHT 40
#define DEFAULT_BACK_BUTTON_PATH "..\\pictures\\button_back.bmp"
#define DEFAULT_BACK_BUTTON_WIDTH 116
#define DEFAULT_BACK_BUTTON_HEIGHT 40

void destroyWindow(Window *src);

GAME_WINDOW handleWindowEvent(Window *src, SDL_Event *event);

void drawWindow(Window *src);

void addTextureToWindow(Window *src, SDL_Texture *texture, SDL_Rect *rect);

void addWidgetToWindow(Window *src, struct widget_t *widget);

void addChessBGAndTitle(Window *src, char *title_path);

Window *createEmptyCenteredWindow(int window_width, int window_height, int num_of_textures, int num_of_widgets,
                                  GAME_WINDOW game_window);

void addBackButtonToWindow(Window *window, GAME_WINDOW back_window);

#endif /* WINDOW_H_ */
