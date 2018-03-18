#include "ChessGUI.h"


#define SPRT_W 7
#define SPRT_LEN 27

void GUI_main_loop(Game *game)
{
    // initialize SDL2 for video
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
        return ;
    }
    Window *main_menu = createMainMenu();
    Window *game_mode_window = createGameModeWindow();
    GAME_WINDOW curr_window = MAIN_MENU;
    // drawing,event handling, and switching windows loop
    SDL_Event e;
    while (curr_window != QUIT) {
        switch (curr_window) {
            case MAIN_MENU:
                SDL_ShowWindow(main_menu->window);
                SDL_RaiseWindow(main_menu->window);
                while (SDL_PollEvent(&e)) {
                    curr_window = handleWindowEvent(main_menu, &e);
                }

                drawWindow(main_menu);
                if (curr_window != MAIN_MENU) {
                    main_menu->nextWindow = MAIN_MENU;
                    SDL_HideWindow(main_menu->window);
                }
                break;
            case GAME_MODE:
                SDL_ShowWindow(game_mode_window->window);
                SDL_RaiseWindow(game_mode_window->window);
                while (SDL_PollEvent(&e)) {
                    curr_window = handleWindowEvent(game_mode_window, &e);
                }

                drawWindow(game_mode_window);
                if (curr_window != GAME_MODE) {
                    game_mode_window->nextWindow = GAME_MODE;
                    SDL_HideWindow(game_mode_window->window);
                }
                break;
            default:
                break;
        }
    }

    // free everything and finish
    destroyWindow(game_mode_window);
    destroyWindow(main_menu);
    SDL_Quit();

}
