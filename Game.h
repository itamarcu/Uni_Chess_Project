
#ifndef UNI_CHESS_PROJECT_GAME_H
#define UNI_CHESS_PROJECT_GAME_H

#include "Board.h"
#include "Parser.h"
#include <stdbool.h>

#define PROGRAM_MODE_CONSOLE 1
#define PROGRAM_MODE_GRAPHICAL 2

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

typedef struct game_type {
    int program_mode; // PROGRAM_MODE_*
    int state; // GAME_STATE_*
    int game_mode; // GAME_MODE_*
    int difficulty; // 1 .. 5
    int user_color; // USER_COLOR_*
    int current_user; // ^ ditto ^
    Board *board;
    int winner; // GAME_CURRENT_WINNER_*
} Game;

/**
 * Main game loop, the game takes place in a loop inside this function.
 */
void main_loop(Game *game);

/**
 * Free game struct and all objects inside it (recursively) to clean up.
 */
void free_game(Game *game);
/**
 * Free command struct and all objects inside it (recursively) to clean up.
 */
void free_command(Command *command);

/**
 * ...
 */
void set_game_state_to_settings(Game *game);

void move_was_made(Game *game);

void change_current_player(Game *game);

/**
 * @return true when the king of the current turn color is checked
 */
bool is_current_checked(Game *game);


void cmd_game_mode(Game *game, Command *command);

void cmd_difficulty(Game *game, Command *command);

void cmd_user_color(Game *game, Command *command);

void cmd_load(Game *game, Command *command);

void cmd_default(Game *game, Command *command);

void cmd_print_settings(Game *game, Command *command);

void cmd_quit(Game *game, Command *command);

void cmd_start(Game *game, Command *command);

void unabbreviated_cmd_move(Game *game, Command *command);

void abbreviated_cmd_move(Game *game, Command *command);

void cmd_get_moves(Game *game, Command *command);

void cmd_save(Game *game, Command *command);

void cmd_undo(Game *game, Command *command);

void cmd_reset(Game *game, Command *command);

#endif //UNI_CHESS_PROJECT_GAME_H
