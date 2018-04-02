#ifndef UNI_CHESS_PROJECT_GUI_H
#define UNI_CHESS_PROJECT_GUI_H

#include "ChessGameLogic.h"
#include "Window.h"
#include "Button.h"
#include <SDL.h>
#include <SDL_video.h>
#include <strings.h>
#include <ctype.h>

#define MAIN_MENU_TITLE_PATH "..\\pictures\\title_main-menu.bmp"
#define DEFAULT_WINDOW_WIDTH 700
#define DEFAULT_WINDOW_HEIGHT 700
#define MAIN_MENU_TEXTURES 2
#define NEW_GAME_BUTTON_PATH "..\\pictures\\button_new-game.bmp"
#define LOAD_GAME_BUTTON_PATH "..\\pictures\\button_load-game.bmp"
#define QUIT_GAME_BUTTON_PATH "..\\pictures\\button_quit.bmp"
#define DEFAULT_BUTTONS_WIDTH 400
#define DEFAULT_BUTTONS_HEIGHT 40
#define MAIN_MENU_WIDGETS 3

#define GAME_MODE_TITLE_PATH "..\\pictures\\title_choose-game-mode.bmp"
#define GAME_MODE_TEXTURES 2
#define ONE_PLAYER_BUTTON_PATH "..\\pictures\\button_1_player.bmp"
#define TWO_PLAYERS_BUTTON_PATH "..\\pictures\\button_2_player.bmp"
#define GAME_MODE_WIDGETS 3


void GUI_main_loop(game_t *game);

void handle_events_and_draw(window_t *current_window, GAME_WINDOW *curr_window, Windows *windows);

window_t *create_main_menu(game_t *game);

window_t *create_game_mode_window(game_t *game);

void one_player_button_action(widget_t *widget);

void two_players_button_action(widget_t *widget);

window_t *create_one_player_options_window(game_t *game);

#endif //UNI_CHESS_PROJECT_GUI_H