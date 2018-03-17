#include "ChessMainMenuWindow.h"
#include <stdio.h>
#include <stdlib.h>
#include "Auxiliary.h"

Window* createMainMenu()
{
    Window *main_menu = createEmptyCenteredWindow(MAIN_MENU_WIDTH, MAIN_MENU_HEIGHT, MAIN_MENU_TEXTURES, MAIN_MENU_WIDGETS, MAIN_MENU);

    addChessBGAndTitle(main_menu, MAIN_MENU_TITLE_PATH);

    int centered_button_height = (MAIN_MENU_HEIGHT - MAIN_MENU_BUTTONS_HEIGHT)/2;
    SDL_Rect new_game_button_rect;
    new_game_button_rect.x = (MAIN_MENU_WIDTH - MAIN_MENU_BUTTONS_WIDTH)/2;
    new_game_button_rect.y = centered_button_height - 2 * MAIN_MENU_BUTTONS_HEIGHT;
    new_game_button_rect.w = MAIN_MENU_BUTTONS_WIDTH;
    new_game_button_rect.h = MAIN_MENU_BUTTONS_HEIGHT;

    SDL_Rect load_game_button_rect;
    load_game_button_rect.x = (MAIN_MENU_WIDTH - MAIN_MENU_BUTTONS_WIDTH)/2;
    load_game_button_rect.y = centered_button_height;
    load_game_button_rect.w = MAIN_MENU_BUTTONS_WIDTH;
    load_game_button_rect.h = MAIN_MENU_BUTTONS_HEIGHT;

    SDL_Rect quit_game_button_rect;
    quit_game_button_rect.x = (MAIN_MENU_WIDTH - MAIN_MENU_BUTTONS_WIDTH)/2;
    quit_game_button_rect.y = centered_button_height + 2 * MAIN_MENU_BUTTONS_HEIGHT;
    quit_game_button_rect.w = MAIN_MENU_BUTTONS_WIDTH;
    quit_game_button_rect.h = MAIN_MENU_BUTTONS_HEIGHT;

    Widget *new_game_button = createButtonSwitchBetweenWindows(main_menu, NEW_GAME_BUTTON_PATH , new_game_button_rect, GAME_MODE);
    Widget *load_game_button = createButtonSwitchBetweenWindows(main_menu, LOAD_GAME_BUTTON_PATH, load_game_button_rect, LOAD_GAME);
    Widget *quit_game_button = createButtonSwitchBetweenWindows(main_menu, QUIT_GAME_BUTTON_PATH, quit_game_button_rect, QUIT);

    addWidgetToWindow(main_menu, new_game_button);
    addWidgetToWindow(main_menu, load_game_button);
    addWidgetToWindow(main_menu, quit_game_button);

    return main_menu;
}

void NewGameAction(Window *src){}
void LoadGameAction(Window *src){}
void QuitGameAction(Window *src){}