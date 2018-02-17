#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Auxiliary.h"
#include "Game.h"

int main(int argc, char **argv) {
    if (argc <= 1) {
        println_error("Missing game mode flag! (\"-c\" or \"-g\"). Quitting!");
        return 987;
    }
    Game *game = (Game *) malloc(sizeof(*game));

    if (strcmp(argv[1], "-c") == 0) //console mode
    {
        println_output("Chess");
        println_output("-------");
        game->program_mode = PROGRAM_MODE_CONSOLE;
    } else if (strcmp(argv[1], "-g") == 0) //graphic mode
    {
        println_debug("%s", "Graphic mode???");
        game->program_mode = PROGRAM_MODE_GRAPHICAL;
    }

    game->difficulty = 2;
    game->game_mode = GAME_MODE_SINGLEPLAYER;
    game->user_color = USER_COLOR_WHITE;
    game->board = NULL;
    set_game_state_to_settings(game);
    main_loop(game);

    free_game(game);
    return 0;
}