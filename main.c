#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ChessConsoleUI.h"
#include "ChessGUI.h"

int main(int argc, char *argv[]) {
    init_scoring_function();

    Game *game = malloc(sizeof(*game));
    if (argc <= 1) { //console mode is default
        game->program_mode = PROGRAM_MODE_CONSOLE;
    } else if (strcmp(argv[1], "-c") == 0) //console mode
    {
        game->program_mode = PROGRAM_MODE_CONSOLE;

    } else if (strcmp(argv[1], "-g") == 0) //graphic mode
    {
        game->program_mode = PROGRAM_MODE_GRAPHICAL;
    }

    reset_default_settings(game);
    game->board = NULL;
    game->history = NULL;

    if (game->program_mode == PROGRAM_MODE_CONSOLE) {
        CUI_main_loop(game);
    } else
        GUI_main_loop(game);

    free_game(game);
    return 0;
}