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
    window_t *options_window = create_one_player_options_window(game);
    GAME_WINDOW curr_window = MAIN_MENU;
    SDL_ShowWindow(main_menu->window);
    // drawing,event handling, and switching windows loop
    Windows *windows = (Windows *) malloc(sizeof(Windows));
    windows->main_menu = main_menu;
    windows->game_mode = game_mode_window;
    windows->options_window = options_window;
    while (curr_window != QUIT) {
        switch (curr_window) {
            case MAIN_MENU:
                handle_events_and_draw(main_menu, &curr_window, windows);
                break;
            case GAME_MODE:
                handle_events_and_draw(game_mode_window, &curr_window, windows);
                break;
            case ONE_PLAYER_OPTIONS:
                handle_events_and_draw(options_window, &curr_window, windows);
                break;
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
        current_window->next_window_frame = prev_window;
        SDL_GetWindowPosition(current_window->window, &curr_window_posX, &curr_window_posY);
        SDL_Delay(10);
        switch (*curr_window) {
            case GAME_MODE:
                switch_to_window_at_pos(windows->game_mode, curr_window_posX, curr_window_posY);
                break;
            case MAIN_MENU:
                switch_to_window_at_pos(windows->main_menu, curr_window_posX, curr_window_posY);
                break;
            case ONE_PLAYER_OPTIONS:
                switch_to_window_at_pos(windows->options_window, curr_window_posX, curr_window_posY);
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
                                                                     new_game_button_rect, GAME_MODE);
    widget_t *load_game_button = create_button_switch_between_windows(main_menu, game, LOAD_GAME_BUTTON_PATH,
                                                                      load_game_button_rect, LOAD_GAME);
    widget_t *quit_game_button = create_button_switch_between_windows(main_menu, game, QUIT_GAME_BUTTON_PATH,
                                                                      quit_game_button_rect, QUIT);

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
    game_mode_window->prev_window = MAIN_MENU;
    add_back_button_to_window(game_mode_window, game);

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
    widget->window->next_window_frame = ((button_t *) widget->data)->next_window;
    widget->game->game_mode = GAME_MODE_MULTIPLAYER;
}

void two_players_button_action(widget_t *widget) {
    widget->window->next_window_frame = ((button_t *) widget->data)->next_window;
    widget->game->game_mode = GAME_MODE_SINGLEPLAYER;
}

window_t *create_one_player_options_window(game_t *game) {
    window_t *options_window = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                                            OPTIONS_WINDOW_TEXTURES,
                                                            OPTIONS_WINDOW_WIDGETS, ONE_PLAYER_OPTIONS);
    add_chess_BG_and_title(options_window, OPTIONS_WINDOW_TITLE_PATH);
    options_window->prev_window = GAME_MODE;
    add_back_button_to_window(options_window, game);

    SDL_Rect *diff_level_tex_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    diff_level_tex_rect->x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 10;
    diff_level_tex_rect->y = (DEFAULT_WINDOW_HEIGHT - OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT) / 5;
    diff_level_tex_rect->w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    diff_level_tex_rect->h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
    SDL_Texture *diff_level_tex;
    if ((diff_level_tex = create_texture_from_path(DIFF_LEVEL_TEX_PATH, options_window->renderer)) == NULL) {
        destroy_window(options_window);
        return NULL;
    }
    add_texture_to_window(options_window, diff_level_tex, diff_level_tex_rect);

    SDL_Rect *user_color_tex_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    user_color_tex_rect->x = diff_level_tex_rect->x;
    user_color_tex_rect->y = diff_level_tex_rect->y + 8 * OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
    user_color_tex_rect->w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    user_color_tex_rect->h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
    SDL_Texture *user_color_tex;
    if ((user_color_tex = create_texture_from_path(USER_COLOR_TEX_PATH, options_window->renderer)) == NULL) {
        destroy_window(options_window);
        return NULL;
    }
    add_texture_to_window(options_window, user_color_tex, user_color_tex_rect);

    SDL_Rect start_button_rect;
    start_button_rect.x = options_window->width - START_BUTTON_WIDTH;
    start_button_rect.y = options_window->height - START_BUTTON_HEIGHT;
    start_button_rect.w = START_BUTTON_WIDTH;
    start_button_rect.h = START_BUTTON_HEIGHT;

    widget_t *start_button = create_button_switch_between_windows(options_window, game, START_BUTTON_PATH,
                                                                  start_button_rect, GAME);
    if (start_button == NULL) {
        return NULL;
    }
    add_widget_to_window(options_window, start_button);

    SDL_Rect amateur_button_rect;
    amateur_button_rect.x = diff_level_tex_rect->x + (int) (1.5 * OPTIONS_WINDOW_DEFAULT_TEX_WIDTH);
    amateur_button_rect.y = diff_level_tex_rect->y;
    amateur_button_rect.w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    amateur_button_rect.h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;

    SDL_Rect easy_button_rect;
    easy_button_rect.x = amateur_button_rect.x;
    easy_button_rect.y = amateur_button_rect.y + OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
    easy_button_rect.w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    easy_button_rect.h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;

    SDL_Rect moderate_button_rect;
    moderate_button_rect.x = amateur_button_rect.x;
    moderate_button_rect.y = easy_button_rect.y + OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
    moderate_button_rect.w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    moderate_button_rect.h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;

    SDL_Rect hard_button_rect;
    hard_button_rect.x = amateur_button_rect.x;
    hard_button_rect.y = moderate_button_rect.y + OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
    hard_button_rect.w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    hard_button_rect.h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;

    SDL_Rect expert_button_rect;
    expert_button_rect.x = amateur_button_rect.x;
    expert_button_rect.y = hard_button_rect.y + OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
    expert_button_rect.w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    expert_button_rect.h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;

    widget_t *amateur_button = create_button(options_window, game, AMATEUR_BUTTON_PATH, amateur_button_rect,
                                             ONE_PLAYER_OPTIONS, amateur_button_action);
    widget_t *easy_button = create_button(options_window, game, EASY_BUTTON_PATH, easy_button_rect,
                                          ONE_PLAYER_OPTIONS, easy_button_action);
    widget_t *moderate_button = create_button(options_window, game, MODERATE_BUTTON_PATH, moderate_button_rect,
                                              ONE_PLAYER_OPTIONS, moderate_button_action);
    widget_t *hard_button = create_button(options_window, game, HARD_BUTTON_PATH, hard_button_rect,
                                          ONE_PLAYER_OPTIONS, hard_button_action);
    widget_t *expert_button = create_button(options_window, game, EXPERT_BUTTON_PATH, expert_button_rect,
                                            ONE_PLAYER_OPTIONS, expert_button_action);

    widget_t **diff_buttons = (widget_t **) malloc(6 * sizeof(widget_t *));
    diff_buttons[0] = amateur_button;
    diff_buttons[1] = easy_button;
    diff_buttons[2] = moderate_button;
    diff_buttons[3] = hard_button;
    diff_buttons[4] = expert_button;
    widget_t *diff_options_buttons = create_options_buttons(options_window, game, diff_buttons, 5);
    add_widget_to_window(options_window, diff_options_buttons);

    SDL_Rect white_button_rect;
    white_button_rect.x = amateur_button_rect.x;
    white_button_rect.y = user_color_tex_rect->y;
    white_button_rect.w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    white_button_rect.h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;

    SDL_Rect black_button_rect;
    black_button_rect.x = white_button_rect.x;
    black_button_rect.y = white_button_rect.y + OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
    black_button_rect.w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
    black_button_rect.h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;

    widget_t *white_button = create_button(options_window, game, WHITE_BUTTON_PATH, white_button_rect,
                                           ONE_PLAYER_OPTIONS, white_button_action);
    widget_t *black_button = create_button(options_window, game, BLACK_BUTTON_PATH, black_button_rect,
                                           ONE_PLAYER_OPTIONS, black_button_action);

    widget_t **colors_buttons = (widget_t **) malloc(3 * sizeof(widget_t *));
    colors_buttons[0] = white_button;
    colors_buttons[1] = black_button;
    widget_t *colors_options_buttons = create_options_buttons(options_window, game, colors_buttons, 2);
    add_widget_to_window(options_window, colors_options_buttons);
    return options_window;
}

void amateur_button_action(widget_t *widget) {
    widget->game->difficulty = AMATEUR_DIFFICULTY;
}

void easy_button_action(widget_t *widget) {
    widget->game->difficulty = EASY_DIFFICULTY;
}

void moderate_button_action(widget_t *widget) {
    widget->game->difficulty = MODERATE_DIFFICULTY;
}

void hard_button_action(widget_t *widget) {
    widget->game->difficulty = HARD_DIFFICULTY;
}

void expert_button_action(widget_t *widget) {
    widget->game->difficulty = EXPERT_DIFFICULTY;
}

void white_button_action(widget_t *widget) {
    widget->game->user_color = WHITE;
}

void black_button_action(widget_t *widget) {
    widget->game->user_color = BLACK;
}

//window_t *create_game_window(game_t *game) {
//    window_t *game_window = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, GAME_WINDOW_TEXTURES,
//                                                            GAME_WINDOW_WIDGETS, GAME);
//    add_chess_BG_and_title(game_window, OPTIONS_WINDOW_TITLE_PATH);
//    game_window->prev_window = GAME_MODE;
//    add_back_button_to_window(game_window, game);
//
//    SDL_Rect *diff_level_tex_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
//    diff_level_tex_rect->x = (DEFAULT_WINDOW_WIDTH - DEFAULT_BUTTONS_WIDTH) / 10;
//    diff_level_tex_rect->y = (DEFAULT_WINDOW_HEIGHT - OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT) / 5;
//    diff_level_tex_rect->w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
//    diff_level_tex_rect->h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
//    SDL_Texture *diff_level_tex;
//    if((diff_level_tex = create_texture_from_path(DIFF_LEVEL_TEX_PATH, game_window->renderer)) == NULL)
//    {
//        destroy_window(game_window);
//        return NULL;
//    }
//    add_texture_to_window(game_window, diff_level_tex, diff_level_tex_rect);
//
//    SDL_Rect *user_color_tex_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
//    user_color_tex_rect->x = diff_level_tex_rect->x;
//    user_color_tex_rect->y = diff_level_tex_rect->y + 2 * OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
//    user_color_tex_rect->w = OPTIONS_WINDOW_DEFAULT_TEX_WIDTH;
//    user_color_tex_rect->h = OPTIONS_WINDOW_DEFAULT_TEX_HEIGHT;
//    SDL_Texture *user_color_tex;
//    if((user_color_tex = create_texture_from_path(USER_COLOR_TEX_PATH, game_window->renderer)) == NULL)
//    {
//        destroy_window(game_window);
//        return NULL;
//    }
//    add_texture_to_window(game_window, user_color_tex, user_color_tex_rect);
//
//    SDL_Rect start_button_rect;
//    start_button_rect.x = game_window->width - START_BUTTON_WIDTH;
//    start_button_rect.y = game_window->height - START_BUTTON_HEIGHT;
//    start_button_rect.w = START_BUTTON_WIDTH;
//    start_button_rect.h = START_BUTTON_HEIGHT;
//
//    widget_t *start_button = create_button_switch_between_windows(game_window, game, START_BUTTON_PATH,
//                                                                  start_button_rect, GAME);
//    if(start_button == NULL)
//    {
//        return NULL;
//    }
//    add_widget_to_window(game_window,start_button);
//
//    return game_window;
//}