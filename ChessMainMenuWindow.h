#ifndef ChessMainMenuWindow_H_
#define ChessMainMenuWindow_H_

#include "Window.h"
#include "Button.h"
#include <SDL.h>
#include <SDL_video.h>

#define MAIN_MENU_TITLE_PATH "..\\pictures\\title_main-menu.bmp"
#define MAIN_MENU_WIDTH 700
#define MAIN_MENU_HEIGHT 700
#define MAIN_MENU_TEXTURES 2
#define NEW_GAME_BUTTON_PATH "..\\pictures\\button_new-game.bmp"
#define LOAD_GAME_BUTTON_PATH "..\\pictures\\button_load-game.bmp"
#define QUIT_GAME_BUTTON_PATH "..\\pictures\\button_quit.bmp"
#define MAIN_MENU_BUTTONS_WIDTH 400
#define MAIN_MENU_BUTTONS_HEIGHT 40
#define MAIN_MENU_WIDGETS 3

Window* createMainMenu();

#endif /* ChessMainMenuWindow_H_ */
