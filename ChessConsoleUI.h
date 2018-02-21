#ifndef UNI_CHESS_PROJECT_CONSOLEUI_H
#define UNI_CHESS_PROJECT_CONSOLEUI_H

#include <stdio.h>
#include "ChessParser.h"
#include "ChessGameLogic.h"
#include <minmax.h>
#include <strings.h>
#include <ctype.h>

void print_board(Board *board);

void CUI_settings_case(Game *game);

void CUI_game_case(Game *game);

void CUI_main_loop(Game *game);

#endif //UNI_CHESS_PROJECT_CONSOLEUI_H
