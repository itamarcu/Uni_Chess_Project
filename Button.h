#ifndef BUTTON_H_
#define BUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include "GUIStructs.h"
//#include "Widget.h"

#define ALPHA_FACTOR_MOUSE_OVER 200

Widget* createButton(
		Window* window,
		const char* image,
		SDL_Rect location,
        GAME_WINDOW next_window,
		void (*action)(Widget *src));
void destroyButton(Widget* src);
void handleButtonEvent(Widget* src, SDL_Event* event);
void drawButton(Widget* widget);
void switchWindowAction(Widget* widget);
Widget* createButtonSwitchBetweenWindows(
        Window* window,
        const char* image,
        SDL_Rect location,
        GAME_WINDOW next_window);
#endif /* BUTTON_H_ */
