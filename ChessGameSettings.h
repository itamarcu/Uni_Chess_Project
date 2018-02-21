#ifndef UNI_CHESS_PROJECT_GAMESETTINGS_H
#define UNI_CHESS_PROJECT_GAMESETTINGS_H

#include "ChessBoard.h"
#include <stdbool.h>

#define PROGRAM_MODE_CONSOLE 'c'
#define PROGRAM_MODE_GRAPHICAL 'g'

#define GAME_STATE_SETTINGS 1
#define GAME_STATE_GAME 2
#define GAME_STATE_QUIT 3

#define GAME_MODE_SINGLEPLAYER 1
#define GAME_MODE_MULTIPLAYER 2

#define USER_COLOR_WHITE 1
#define USER_COLOR_BLACK 2

#define GAME_WINNER_NONE 3
#define GAME_WINNER_DRAW 0
#define GAME_WINNER_WHITE 1
#define GAME_WINNER_BLACK 2

#define AMATEUR_DIFFICULTY 1
#define EASY_DIFFICULTY 2
#define MODERATE_DIFFICULTY 3
#define HARD_DIFFICULTY 4
#define EXPERT_DIFFICULTY 5


typedef struct game_type {
    char program_mode; // PROGRAM_MODE_*
    int state; // GAME_STATE_*
    int game_mode; // GAME_MODE_*
    int difficulty; // 1 .. 5
    int user_color; // USER_COLOR_*
    int current_user; // ^ ditto ^
    Board *board;
    int winner; // GAME_CURRENT_WINNER_*
} Game;


void cmd_game_mode(Game *game, int gameMode);
/*
 * assuming difficulty
 */
void cmd_difficulty(Game *game, int difficulty);

void cmd_user_color(Game *game, int color);
/*
 * returns true if the game loaded successfully and false otherwise.
 */
bool cmd_load(Game *game, char* path);

void set_default_settings(Game *game);

void print_settings(Game *game);

void free_game(Game *game);

char *difficulty_int_to_string(int difficulty);

#endif //UNI_CHESS_PROJECT_GAMESETTINGS_H
