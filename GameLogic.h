
#ifndef UNI_CHESS_PROJECT_GAMELOGIC_H
#define UNI_CHESS_PROJECT_GAMELOGIC_H


#include "Board.h"
#include "Game.h"
#include "Structs.h"

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



/**
 * @return true when the king of the current turn color is checked
 */
bool is_current_checked(Game *game);


void cmd_start(Game *game, Command *command);

void unabbreviated_cmd_move(Game *game, Command *command);

void abbreviated_cmd_move(Game *game, Command *command);

void cmd_get_moves(Game *game, Command *command);

void cmd_save(Game *game, Command *command);

void cmd_undo(Game *game, Command *command);

void cmd_reset(Game *game, Command *command);

#endif //UNI_CHESS_PROJECT_GAMELOGIC_H
