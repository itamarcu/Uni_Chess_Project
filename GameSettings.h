#ifndef UNI_CHESS_PROJECT_GAMESETTINGS_H
#define UNI_CHESS_PROJECT_GAMESETTINGS_H


#include "Game.h"
#include "Parser.h"

void cmd_game_mode(Game *game, Command *command);

void cmd_difficulty(Game *game, Command *command);

void cmd_user_color(Game *game, Command *command);

void cmd_load(Game *game, Command *command);

void cmd_default(Game *game, Command *command);

void cmd_print_settings(Game *game, Command *command);


#endif //UNI_CHESS_PROJECT_GAMESETTINGS_H
