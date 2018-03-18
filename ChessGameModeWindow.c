#include "ChessGameModeWindow.h"
#include <stdio.h>
#include <stdlib.h>

Window* createGameModeWindow()
{
    Window *game_mode_window = createEmptyCenteredWindow(GAME_MODE_WIDTH, GAME_MODE_HEIGHT, GAME_MODE_TEXTURES, GAME_MODE_WIDGETS, GAME_MODE);
    addChessBGAndTitle(game_mode_window, GAME_MODE_TITLE_PATH);
    addBackButtonToWindow(game_mode_window, MAIN_MENU);

    int centered_button_height = (GAME_MODE_HEIGHT - GAME_MODE_BUTTONS_HEIGHT)/2;
    SDL_Rect new_game_button_rect;
    new_game_button_rect.x = (GAME_MODE_WIDTH - GAME_MODE_BUTTONS_WIDTH)/2;
    new_game_button_rect.y = centered_button_height - 2 * GAME_MODE_BUTTONS_HEIGHT;
    new_game_button_rect.w = GAME_MODE_BUTTONS_WIDTH;
    new_game_button_rect.h = GAME_MODE_BUTTONS_HEIGHT;

    SDL_Rect load_game_button_rect;
    load_game_button_rect.x = (GAME_MODE_WIDTH - GAME_MODE_BUTTONS_WIDTH)/2;
    load_game_button_rect.y = centered_button_height;
    load_game_button_rect.w = GAME_MODE_BUTTONS_WIDTH;
    load_game_button_rect.h = GAME_MODE_BUTTONS_HEIGHT;

    SDL_Rect quit_game_button_rect;
    quit_game_button_rect.x = (GAME_MODE_WIDTH - GAME_MODE_BUTTONS_WIDTH)/2;
    quit_game_button_rect.y = centered_button_height + 2 * GAME_MODE_BUTTONS_HEIGHT;
    quit_game_button_rect.w = GAME_MODE_BUTTONS_WIDTH;
    quit_game_button_rect.h = GAME_MODE_BUTTONS_HEIGHT;

    //Widget *new_game_button = createButtonSwitchBetweenWindows(game_mode_window, NEW_GAME_BUTTON_PATH , new_game_button_rect, GAME_MODE);
    //Widget *load_game_button = createButtonSwitchBetweenWindows(game_mode_window, LOAD_GAME_BUTTON_PATH, load_game_button_rect, LOAD_GAME);

    //addWidgetToWindow(game_mode_window, new_game_button);
    //addWidgetToWindow(game_mode_window, load_game_button);

    return game_mode_window;
}