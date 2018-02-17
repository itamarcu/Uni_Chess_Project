
#ifndef UNI_CHESS_PROJECT_GAME_H
#define UNI_CHESS_PROJECT_GAME_H

#include "Board.h"
#include "Parser.h"
#include "GameLogic.h"
#include <stdbool.h>




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

void cmd_quit(Game *game, Command *command);

#endif //UNI_CHESS_PROJECT_GAME_H
