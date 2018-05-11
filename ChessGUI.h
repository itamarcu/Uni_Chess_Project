#ifndef UNI_CHESS_PROJECT_GUI_H
#define UNI_CHESS_PROJECT_GUI_H

#include <SDL.h>
#include <SDL_video.h>
#include <strings.h>
#include <ctype.h>
#include "ChessGameLogic.h"
#include "Window.h"
#include "Button.h"
#include "OptionsButtons.h"
#include "SlotOptions.h"
#include "Undo.h"
#include "HandleAndDrawGame.h"
#include "FileOperations.h"

#define MAIN_MENU_TITLE_PATH (PICTURES_DIRECTORY SEP "title_main-menu.bmp")
#define DEFAULT_WINDOW_WIDTH 700
#define DEFAULT_WINDOW_HEIGHT 700

#define MAIN_MENU_TEXTURES 2
#define NEW_GAME_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_new-game.bmp")
#define LOAD_GAME_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_load-game.bmp")
#define QUIT_GAME_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_quit.bmp")
#define DEFAULT_BUTTONS_WIDTH 400
#define DEFAULT_BUTTONS_HEIGHT 40
#define MAIN_MENU_WIDGETS 3

#define GAME_MODE_TITLE_PATH (PICTURES_DIRECTORY SEP "title_choose-game-mode.bmp")
#define GAME_MODE_TEXTURES 2
#define ONE_PLAYER_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_1_player.bmp")
#define TWO_PLAYERS_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_2_player.bmp")
#define GAME_MODE_WIDGETS 3

#define OPTIONS_WINDOW_TITLE_PATH (PICTURES_DIRECTORY SEP "title_choose-game-settings.bmp")
#define OPTIONS_WINDOW_TEXTURES 4
#define DIFF_LEVEL_TEX_PATH (PICTURES_DIRECTORY SEP "diff_level.bmp")
#define USER_COLOR_TEX_PATH (PICTURES_DIRECTORY SEP "user_color.bmp")
#define OPTIONS_WINDOW_DEFAULT_TEX_WIDTH 250
#define OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT 40
#define OPTIONS_WINDOW_WIDGETS 5
#define START_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_start.bmp")
#define START_BUTTON_WIDTH 116
#define START_BUTTON_HEIGHT 40
#define AMATEUR_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_amateur.bmp")
#define EASY_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_easy.bmp")
#define MODERATE_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_moderate.bmp")
#define HARD_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_hard.bmp")
#define EXPERT_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_expert.bmp")
#define BLACK_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_black.bmp")
#define WHITE_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_white.bmp")

#define PICK_SLOT_TITLE_PATH (PICTURES_DIRECTORY SEP "title_pick-a-slot.bmp")
#define PICK_SLOT_WINDOW_TEXTURES 3
#define PICK_SLOT_WINDOW_WIDGETS 2

#define GAME_WINDOW_HEIGHT 800
#define GAME_BOARD_HEIGHT 700
#define GAME_WINDOW_TEXTURES 1
#define GAME_WINDOW_WIDGETS 7 // the game_gui and 6 buttons.
#define DEFAULT_GAME_BUTTON_WIDTH 180
#define DEFAULT_GAME_BUTTON_HEIGHT 50
#define UNDO_MOVE_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_undo-move.bmp")
#define MAIN_MENU_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_main-menu.bmp")
#define SAVE_GAME_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_save-game.bmp")
#define LOAD_GAME2_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_load-game2.bmp")
#define QUIT2_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_quit2.bmp")
#define RESTART_BUTTON_PATH (PICTURES_DIRECTORY SEP "button_restart.bmp")



void GUI_main_loop(Game *game);


void build_main_menu(Game *game, windows_t *windows);


void build_game_mode_window(Game *game, windows_t *windows);


void build_one_player_options_window(Game *game, windows_t *windows);


void build_pick_slot_window(Game *game, windows_t *windows);


void build_game_window(Game *game, windows_t *windows);



#endif //UNI_CHESS_PROJECT_GUI_H