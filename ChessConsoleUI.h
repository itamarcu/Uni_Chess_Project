#ifndef UNI_CHESS_PROJECT_CONSOLEUI_H
#define UNI_CHESS_PROJECT_CONSOLEUI_H

#include <stdio.h>
#include "ChessParser.h"
#include "ChessGameLogic.h"
#include "Minimax.h"
#include <strings.h>
#include <ctype.h>

void print_board(board_t *board);

void CUI_settings_case(game_t *game);

void CUI_game_case(game_t *game);

void CUI_main_loop(game_t *game);

#endif //UNI_CHESS_PROJECT_CONSOLEUI_H
