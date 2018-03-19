#ifndef UNI_CHESS_PROJECT_GUI_H
#define UNI_CHESS_PROJECT_GUI_H

#include "ChessGameLogic.h"
#include "ChessMainMenuWindow.h"
#include "ChessGameModeWindow.h"
#include <SDL.h>
#include <SDL_video.h>
#include <strings.h>
#include <ctype.h>

void GUI_main_loop(Game *game);

void handleEventsAndDraw(Window *current_window, GAME_WINDOW *curr_window, Windows *windows);


#endif //UNI_CHESS_PROJECT_GUI_H