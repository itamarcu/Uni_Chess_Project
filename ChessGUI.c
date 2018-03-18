#include "ChessGUI.h"


#define SPRT_W 7
#define SPRT_LEN 27

void GUI_main_loop(Game *game)
{
    // initialize SDL2 for video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
        return ;
    }
    Window *main_menu = createMainMenu();
    Window *game_mode_window = createGameModeWindow();
    GAME_WINDOW curr_window = MAIN_MENU;
    SDL_ShowWindow(main_menu->window);
    // drawing,event handling, and switching windows loop

    int curr_window_posX;
    int curr_window_posY;
    while (curr_window != QUIT) {
        switch (curr_window) {
            case MAIN_MENU:
                drawWindow(main_menu);
                while (SDL_PollEvent(&e)) {
                    curr_window = handleWindowEvent(main_menu, &e);
                }
                if (curr_window != MAIN_MENU) {
                    main_menu->nextWindow = MAIN_MENU;
                    SDL_GetWindowPosition(main_menu->window, &curr_window_posX, &curr_window_posY);
                    switch (curr_window) {
                        case GAME_MODE:
                            SDL_SetWindowPosition(game_mode_window->window, curr_window_posX, curr_window_posY);
                            SDL_ShowWindow(game_mode_window->window);
                            break;
                        default:
                            break;
                    }
                    SDL_HideWindow(main_menu->window);
                }
                break;
            case GAME_MODE:
                SDL_ShowWindow(game_mode_window->window);
                drawWindow(game_mode_window);
                while (SDL_PollEvent(&e)) {
                    curr_window = handleWindowEvent(game_mode_window, &e);
                }

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

void handleEventsAndDraw(Window *current_window, GAME_WINDOW game_window, GAME_WINDOW *curr_window) {
    SDL_Event e;
    drawWindow(window);
    while (SDL_PollEvent(&e)) {
        *curr_window = handleWindowEvent(window, &e);
    }
    if (*curr_window != game_window) {
        window->nextWindow = game_window;
        SDL_GetWindowPosition(window->window, &curr_window_posX, &curr_window_posY);
        switch (curr_window) {
            case GAME_MODE:
                SDL_SetWindowPosition(game_mode_window->window, curr_window_posX, curr_window_posY);
                SDL_ShowWindow(game_mode_window->window);
                break;
            default:
                break;
        }
        SDL_HideWindow(window->window);
    }
}