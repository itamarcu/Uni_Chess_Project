#ifndef ChessGameModeWindow_H_
#define ChessGameModeWindow_H_

#include "Window.h"
#include "Button.h"
#include <SDL.h>
#include <SDL_video.h>

#define GAME_MODE_TITLE_PATH "..\\pictures\\title_choose-game-mode.bmp"
#define GAME_MODE_WIDTH 700
#define GAME_MODE_HEIGHT 700
#define GAME_MODE_TEXTURES 2
#define ONE_PLAYER_BUTTON_PATH "..\\pictures\\button_new-game.bmp"
#define TWO_PLAYERS_BUTTON_PATH "..\\pictures\\button_load-game.bmp"
#define GAME_MODE_BUTTONS_WIDTH 400
#define GAME_MODE_BUTTONS_HEIGHT 40
#define GAME_MODE_WIDGETS 3

Window* createGameModeWindow();

#endif /* ChessGameModeWindow_H_ */
