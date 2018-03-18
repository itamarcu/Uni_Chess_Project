#ifndef UNI_CHESS_PROJECT_GUI_H
#define UNI_CHESS_PROJECT_GUI_H
#define SDL_MAIN_NEEDED

#include "ChessGameLogic.h"
#include "ChessMainMenuWindow.h"
#include "ChessGameModeWindow.h"
#include <SDL.h>
#include <SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <strings.h>
#include <ctype.h>

void GUI_main_loop(Game *game);



#endif //UNI_CHESS_PROJECT_GUI_H