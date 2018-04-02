#include "ChessGUI.h"

void GUI_main_loop(game_t *game)
{
    // initialize SDL2 for video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
        return ;
    }
    window_t *main_menu = create_main_menu(game);
    window_t *game_mode_window = create_game_mode_window(game);
    GAME_WINDOW curr_window = MAIN_MENU;
    SDL_ShowWindow(main_menu->window);
    // drawing,event handling, and switching windows loop
    Windows *windows = (Windows *) malloc(sizeof(Windows));
    windows->main_menu = main_menu;
    windows->game_mode = game_mode_window;
    while (curr_window != QUIT) {
        switch (curr_window) {
            case MAIN_MENU:
                handle_events_and_draw(main_menu, &curr_window, windows);
                break;
            case GAME_MODE:
                handle_events_and_draw(game_mode_window, &curr_window, windows);
            default:
                break;
        }
    }

    // free everything and finish
    destroy_window(game_mode_window);
    destroy_window(main_menu);
    SDL_Quit();
}

void handle_events_and_draw(window_t *current_window, GAME_WINDOW *curr_window, Windows *windows) {
    SDL_Event e;
    GAME_WINDOW prev_window = *curr_window;
    int curr_window_posX;
    int curr_window_posY;
    draw_window(current_window);
    while (SDL_PollEvent(&e)) {
        *curr_window = handle_window_event(current_window, &e);
    }
    if (*curr_window != prev_window) {
        current_window->next_window = prev_window;
        SDL_GetWindowPosition(current_window->window, &curr_window_posX, &curr_window_posY);
        SDL_Delay(10);
        switch (*curr_window) {
            case GAME_MODE:
                switch_to_window_at_pos(windows->game_mode, curr_window_posX, curr_window_posY);
                break;
            case MAIN_MENU:
                switch_to_window_at_pos(windows->main_menu, curr_window_posX, curr_window_posY);
                break;
            default:
                break;
        }
        SDL_HideWindow(current_window->window);
    }
}

window_t *create_main_menu(game_t *game) {
    window_t *main_menu = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, MAIN_MENU_TEXTURES,
                                                     MAIN_MENU_WIDGETS, MAIN_MENU);

    add_chess_BG_and_title(main_menu, MAIN_MENU_TITLE_PATH);

    int centered_button_height = (DEFAULT_WINDOW_HEIGHT - DEFAULT_BUTTONS_HEIGHT) / 2;
    SDL_Rect new_game_button_rect;
    new_game_button_rect.x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 2;
    new_game_button_rect.y = centered_button_height - 2 * DEFAULT_BUTTONS_HEIGHT;
    new_game_button_rect.w = DEFAULT_BUTTONS_WIDTH;
    new_game_button_rect.h = DEFAULT_BUTTONS_HEIGHT;

    SDL_Rect load_game_button_rect;
    load_game_button_rect.x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 2;
    load_game_button_rect.y = centered_button_height;
    load_game_button_rect.w = DEFAULT_BUTTONS_WIDTH;
    load_game_button_rect.h = DEFAULT_BUTTONS_HEIGHT;

    SDL_Rect quit_game_button_rect;
    quit_game_button_rect.x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 2;
    quit_game_button_rect.y = centered_button_height + 2 * DEFAULT_BUTTONS_HEIGHT;
    quit_game_button_rect.w = DEFAULT_BUTTONS_WIDTH;
    quit_game_button_rect.h = DEFAULT_BUTTONS_HEIGHT;

    widget_t *new_game_button = create_button_switch_between_windows(main_menu, game, NEW_GAME_BUTTON_PATH,
                                                                   new_game_button_rect,
                                                                   GAME_MODE);
    widget_t *load_game_button = create_button_switch_between_windows(main_menu, game, LOAD_GAME_BUTTON_PATH,
                                                                    load_game_button_rect,
                                                                    LOAD_GAME);
    widget_t *quit_game_button = create_button_switch_between_windows(main_menu, game, QUIT_GAME_BUTTON_PATH,
                                                                    quit_game_button_rect,
                                                                    QUIT);

    add_widget_to_window(main_menu, new_game_button);
    add_widget_to_window(main_menu, load_game_button);
    add_widget_to_window(main_menu, quit_game_button);

    return main_menu;
}

window_t *create_game_mode_window(game_t *game) {
    window_t *game_mode_window = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                                            GAME_MODE_TEXTURES,
                                                            GAME_MODE_WIDGETS, GAME_MODE);
    add_chess_BG_and_title(game_mode_window, GAME_MODE_TITLE_PATH);
    add_back_button_to_window(game_mode_window, game, MAIN_MENU);

    int centered_button_height = (DEFAULT_WINDOW_HEIGHT - DEFAULT_BUTTONS_HEIGHT) / 2;
    SDL_Rect one_player_button_rect;
    one_player_button_rect.x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 2;
    one_player_button_rect.y = centered_button_height - 1 * DEFAULT_BUTTONS_HEIGHT;
    one_player_button_rect.w = DEFAULT_BUTTONS_WIDTH;
    one_player_button_rect.h = DEFAULT_BUTTONS_HEIGHT;

    SDL_Rect two_players_button_rect;
    two_players_button_rect.x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 2;
    two_players_button_rect.y = centered_button_height + 1 * DEFAULT_BUTTONS_HEIGHT;
    two_players_button_rect.w = DEFAULT_BUTTONS_WIDTH;
    two_players_button_rect.h = DEFAULT_BUTTONS_HEIGHT;

    widget_t *one_player_button = create_button(game_mode_window, game, ONE_PLAYER_BUTTON_PATH, one_player_button_rect,
                                              ONE_PLAYER_OPTIONS, one_player_button_action);
    widget_t *two_players_button = create_button(game_mode_window, game, TWO_PLAYERS_BUTTON_PATH, two_players_button_rect,
                                               GAME, two_players_button_action);

    add_widget_to_window(game_mode_window, one_player_button);
    add_widget_to_window(game_mode_window, two_players_button);

    return game_mode_window;
}

void one_player_button_action(widget_t *widget) {
    widget->window->next_window = ((button_t *) widget->data)->next_window;
    widget->game->game_mode = GAME_MODE_MULTIPLAYER;
}

void two_players_button_action(widget_t *widget) {
    widget->window->next_window = ((button_t *) widget->data)->next_window;
    widget->game->game_mode = GAME_MODE_SINGLEPLAYER;
}

window_t *create_one_player_options_window(game_t *game) {
    window_t *game_mode_window = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                                            GAME_MODE_TEXTURES,
                                                            GAME_MODE_WIDGETS, GAME_MODE);
    add_chess_BG_and_title(game_mode_window, GAME_MODE_TITLE_PATH);
    add_back_button_to_window(game_mode_window, game, MAIN_MENU);

    int centered_button_height = (DEFAULT_WINDOW_HEIGHT - DEFAULT_BUTTONS_HEIGHT) / 2;
    SDL_Rect one_player_button_rect;
    one_player_button_rect.x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 2;
    one_player_button_rect.y = centered_button_height - 1 * DEFAULT_BUTTONS_HEIGHT;
    one_player_button_rect.w = DEFAULT_BUTTONS_WIDTH;
    one_player_button_rect.h = DEFAULT_BUTTONS_HEIGHT;

    SDL_Rect two_players_button_rect;
    two_players_button_rect.x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 2;
    two_players_button_rect.y = centered_button_height + 1 * DEFAULT_BUTTONS_HEIGHT;
    two_players_button_rect.w = DEFAULT_BUTTONS_WIDTH;
    two_players_button_rect.h = DEFAULT_BUTTONS_HEIGHT;

    widget_t *one_player_button = create_button(game_mode_window, game, ONE_PLAYER_BUTTON_PATH, one_player_button_rect,
                                              ONE_PLAYER_OPTIONS, one_player_button_action);
    widget_t *two_players_button = create_button(game_mode_window, game, TWO_PLAYERS_BUTTON_PATH, two_players_button_rect,
                                               GAME, two_players_button_action);

    add_widget_to_window(game_mode_window, one_player_button);
    add_widget_to_window(game_mode_window, two_players_button);

    return game_mode_window;
}