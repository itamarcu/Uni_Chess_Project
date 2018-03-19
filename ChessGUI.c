#include "ChessGUI.h"

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
    Windows *windows = (Windows *) malloc(sizeof(Windows));
    windows->main_menu = main_menu;
    windows->game_mode = game_mode_window;
    while (curr_window != QUIT) {
        switch (curr_window) {
            case MAIN_MENU:
                handleEventsAndDraw(main_menu, &curr_window, windows);
                break;
            case GAME_MODE:
                handleEventsAndDraw(game_mode_window, &curr_window, windows);
            default:
                break;
        }
    }

    // free everything and finish
    destroyWindow(game_mode_window);
    destroyWindow(main_menu);
    SDL_Quit();
}

void handleEventsAndDraw(Window *current_window, GAME_WINDOW *curr_window, Windows *windows) {
    SDL_Event e;
    GAME_WINDOW prev_window = *curr_window;
    int curr_window_posX;
    int curr_window_posY;
    drawWindow(current_window);
    while (SDL_PollEvent(&e)) {
        *curr_window = handleWindowEvent(current_window, &e);
    }
    if (*curr_window != prev_window) {
        current_window->nextWindow = prev_window;
        SDL_GetWindowPosition(current_window->window, &curr_window_posX, &curr_window_posY);
        switch (*curr_window) {
            case GAME_MODE:
                SDL_SetWindowPosition(windows->game_mode->window, curr_window_posX, curr_window_posY);
                SDL_ShowWindow(windows->game_mode->window);
                break;
            case MAIN_MENU:
                SDL_SetWindowPosition(windows->main_menu->window, curr_window_posX, curr_window_posY);
                SDL_ShowWindow(windows->main_menu->window);
                break;
            default:
                break;
        }
        SDL_HideWindow(current_window->window);
    }
}