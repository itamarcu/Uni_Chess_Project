#ifndef UNI_CHESS_PROJECT_GAMESETTINGS_H
#define UNI_CHESS_PROJECT_GAMESETTINGS_H

#include <stdbool.h>
#include "ChessBoard.h"
#include "string.h"

#define PROGRAM_MODE_CONSOLE 'c'
#define PROGRAM_MODE_GRAPHICAL 'g'

#define GAME_STATE_SETTINGS 1
#define GAME_STATE_GAME 2
#define GAME_STATE_QUIT 3

#define GAME_MODE_SINGLEPLAYER 1
#define GAME_MODE_MULTIPLAYER 2

#define GAME_WINNER_NONE 3
#define GAME_WINNER_DRAW 0
#define GAME_WINNER_WHITE 1 //please don't change, this is identical to WHITE and BLACK in Auxiliary.h
#define GAME_WINNER_BLACK 2 //ditto

#define AMATEUR_DIFFICULTY 1
#define EASY_DIFFICULTY 2
#define MODERATE_DIFFICULTY 3
#define HARD_DIFFICULTY 4
#define EXPERT_DIFFICULTY 5

#define HISTORY_SIZE 7  // Current board is included

typedef struct history_t {
    board_t *prev_boards[HISTORY_SIZE]; // first element is most recent
    unsigned int prev_moves[HISTORY_SIZE]; // each one defines the move via a four-char integer (int32). I know, this is ugly!
    int count;  // 0 when empty, HISTORY_SIZE when full
} History;

typedef struct game_t {
    char program_mode; // PROGRAM_MODE_*
    int state; // GAME_STATE_*
    int game_mode; // GAME_MODE_*
    int difficulty; // 1 .. 5
    int user_color; // USER_COLOR_*
    int current_player; // ^ ditto ^
    board_t *board;
    int winner; // GAME_CURRENT_WINNER_*
    History *history;
    bool is_saved;
} game_t;

/**
 * returns true if the game saved successfully and false otherwise.
 */
bool save_game_to_path(game_t *game, char *path);

/*
 * assuming the file exist in path and containing the saved game with the
 * returns true if the game loaded successfully and false otherwise.
 */
bool load_game_from_path(game_t *game, char *path);

bool is_file_exists(char *path);

/*
 * assuming the file exists in path
 */
bool is_file_empty(char *path);

void reset_default_settings(game_t *game);

void free_history(History *history);

void free_game(game_t *game);

char *difficulty_string(int difficulty_int);

char *color_string(int color_int);

#endif //UNI_CHESS_PROJECT_GAMESETTINGS_H
