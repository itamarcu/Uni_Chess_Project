#include "ChessGUI.h"

void GUI_main_loop(game_t *game)
{
    // initialize SDL2 for video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
        return ;
    }
    window_t *main_menu = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, MAIN_MENU_TEXTURES,
                                                       MAIN_MENU_WIDGETS, MAIN_MENU);
    window_t *game_mode_window = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                                              GAME_MODE_TEXTURES, GAME_MODE_WIDGETS, GAME_MODE);
    window_t *options_window = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                                            OPTIONS_WINDOW_TEXTURES,
                                                            OPTIONS_WINDOW_WIDGETS, ONE_PLAYER_OPTIONS);
    window_t *game_window = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, GAME_WINDOW_TEXTURES,
                                                         GAME_WINDOW_WIDGETS, GAME);
    window_t *pick_slot_window = create_empty_centered_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                                              PICK_SLOT_WINDOW_TEXTURES,
                                                              PICK_SLOT_WINDOW_WIDGETS, PICK_SLOT);

    windows_t *windows = (windows_t *) malloc(sizeof(windows_t));
    windows->main_menu = main_menu;
    windows->game_mode_window = game_mode_window;
    windows->options_window = options_window;
    windows->game_window = game_window;
    windows->pick_slot_window = pick_slot_window;

    main_menu->windows = windows;
    game_mode_window->windows = windows;
    options_window->windows = windows;
    game_window->windows = windows;
    pick_slot_window->windows = windows;

    build_main_menu(game, windows);
    build_game_mode_window(game, windows);
    build_one_player_options_window(game, windows);
    build_game_window(game, windows);
    build_pick_slot_window(game, windows);

    GAME_WINDOW curr_window = MAIN_MENU;
    SDL_ShowWindow(main_menu->window);
    // drawing,event handling, and switching windows loop

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
            case GAME:
                handle_events_and_draw(game_window, &curr_window, windows);
                break;
            case PICK_SLOT:
                handle_events_and_draw(pick_slot_window, &curr_window, windows);
            default:
                break;
        }
    }

    // free everything and finish
    destroy_window(game_window);
    destroy_window(options_window);
    destroy_window(game_mode_window);
    destroy_window(main_menu);
    destroy_window(pick_slot_window);
    SDL_Quit();
}

void handle_events_and_draw(window_t *current_window, GAME_WINDOW *curr_window, windows_t *windows) {
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
                show_window_at_pos(windows->game_mode_window, curr_window_posX, curr_window_posY);
                break;
            case MAIN_MENU:
                show_window_at_pos(windows->main_menu, curr_window_posX, curr_window_posY);
                break;
            case ONE_PLAYER_OPTIONS:
                show_window_at_pos(windows->options_window, curr_window_posX, curr_window_posY);
                break;
            case GAME:
                show_window_at_pos(windows->game_window, curr_window_posX, curr_window_posY);
                break;
            case PICK_SLOT:
                show_window_at_pos(windows->pick_slot_window, curr_window_posX, curr_window_posY);
            default:
                break;
        }
        SDL_HideWindow(current_window->window);
    }
}

void *build_main_menu(game_t *game, windows_t *windows) {
    window_t *main_menu = windows->main_menu;

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

    widget_t *new_game_button = create_button(main_menu, game, NEW_GAME_BUTTON_PATH,
                                              new_game_button_rect, windows->game_mode_window,
                                              new_game_button_action);
    widget_t *load_game_button = create_button(main_menu, game, LOAD_GAME_BUTTON_PATH,
                                               load_game_button_rect, windows->pick_slot_window,
                                               load_button_main_menu_action);
    widget_t *quit_game_button = create_button(main_menu, game, QUIT_GAME_BUTTON_PATH,
                                               quit_game_button_rect, NULL, quit_button_action);

    add_widget_to_window(main_menu, new_game_button);
    add_widget_to_window(main_menu, load_game_button);
    add_widget_to_window(main_menu, quit_game_button);
}

void new_game_button_action(widget_t *widget) {
    start_game(widget->game);
    reset_game_gui((game_gui_t *) widget->window->windows->game_window->widgets[0]->data, widget->game);
    switch_window_and_change_prev_window_action(widget);
}
void load_button_main_menu_action(widget_t *widget) {
    switch_window_and_change_prev_window_action(widget);
    widget->next_window->next_window = widget->window->windows->game_window;
    slot_options_t *slot_options = (slot_options_t *) widget->next_window->widgets[0]->data;
    slot_options->is_loading_mode = true;
}

void quit_button_action(widget_t *widget) {
    widget->window->next_window_frame = QUIT;
}

void *build_game_mode_window(game_t *game, windows_t *windows) {
    window_t *game_mode_window = windows->game_mode_window;
    add_chess_BG_and_title(game_mode_window, GAME_MODE_TITLE_PATH);
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
                                                windows->options_window, one_player_button_action);
    widget_t *two_players_button = create_button(game_mode_window, game, TWO_PLAYERS_BUTTON_PATH,
                                                 two_players_button_rect,
                                                 windows->game_window, two_players_button_action);

    add_widget_to_window(game_mode_window, one_player_button);
    add_widget_to_window(game_mode_window, two_players_button);

}

void one_player_button_action(widget_t *widget) {
    widget->game->game_mode = GAME_MODE_SINGLEPLAYER;
    switch_window_and_change_prev_window_action(widget);
}

void two_players_button_action(widget_t *widget) {
    widget->game->game_mode = GAME_MODE_MULTIPLAYER;
    switch_window_and_change_prev_window_action(widget);
}


void *build_one_player_options_window(game_t *game, windows_t *windows) {
    window_t *options_window = windows->options_window;
    add_chess_BG_and_title(options_window, OPTIONS_WINDOW_TITLE_PATH);
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

    widget_t *start_button = create_button(options_window, game, START_BUTTON_PATH,
                                           start_button_rect, windows->game_window,
                                           switch_window_and_change_prev_window_action);
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
                                             NULL, amateur_button_action);
    widget_t *easy_button = create_button(options_window, game, EASY_BUTTON_PATH, easy_button_rect,
                                          NULL, easy_button_action);
    widget_t *moderate_button = create_button(options_window, game, MODERATE_BUTTON_PATH, moderate_button_rect,
                                              NULL, moderate_button_action);
    widget_t *hard_button = create_button(options_window, game, HARD_BUTTON_PATH, hard_button_rect,
                                          NULL, hard_button_action);
    widget_t *expert_button = create_button(options_window, game, EXPERT_BUTTON_PATH, expert_button_rect,
                                            NULL, expert_button_action);

    widget_t **diff_buttons = (widget_t **) malloc(6 * sizeof(widget_t *));
    diff_buttons[0] = amateur_button;
    diff_buttons[1] = easy_button;
    diff_buttons[2] = moderate_button;
    diff_buttons[3] = hard_button;
    diff_buttons[4] = expert_button;
    widget_t *diff_options_buttons_widget = create_options_buttons(options_window, game, diff_buttons, 5);
    options_buttons_t *diff_option_buttons = (options_buttons_t *) diff_options_buttons_widget->data;
    diff_option_buttons->current_pushed_button = 1;
    ((button_t *) diff_buttons[1]->data)->current_alpha_factor = ALPHA_FACTOR_MOUSE_OVER;
    add_widget_to_window(options_window, diff_options_buttons_widget);

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
                                           NULL, white_button_action);
    widget_t *black_button = create_button(options_window, game, BLACK_BUTTON_PATH, black_button_rect,
                                           NULL, black_button_action);

    widget_t **colors_buttons = (widget_t **) malloc(3 * sizeof(widget_t *));
    colors_buttons[0] = white_button;
    colors_buttons[1] = black_button;
    widget_t *colors_options_buttons_widget = create_options_buttons(options_window, game, colors_buttons, 2);
    options_buttons_t *colors_options_buttons = (options_buttons_t *) colors_options_buttons_widget->data;
    colors_options_buttons->current_pushed_button = 0;
    ((button_t *) colors_buttons[0]->data)->current_alpha_factor = ALPHA_FACTOR_MOUSE_OVER;
    add_widget_to_window(options_window, colors_options_buttons_widget);
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

void *build_pick_slot_window(game_t *game, windows_t *windows) {
    window_t *pick_slot_window = windows->pick_slot_window;
    add_chess_BG_and_title(pick_slot_window, PICK_SLOT_TITLE_PATH);
    pick_slot_window->next_window = windows->game_window; // will change if picking a slot for saving a game instead of loading a game.

    widget_t *slot_options_widget = create_slot_options(pick_slot_window, game, 10,
                                                        (DEFAULT_WINDOW_WIDTH - SLOT_OPTIONS_WIDTH) / 2, 250,
                                                        save_load_game_slots_action);
    add_widget_to_window(pick_slot_window,
                         slot_options_widget); // here pick_slow_window->widgets[0] == slot_options_widget.
    add_back_button_to_window(pick_slot_window, game);
    slot_options_t *slot_options = (slot_options_t *) (slot_options_widget->data);
    slot_options->is_loading_mode = true;
}

void save_load_game_slots_action(widget_t *src, int clicked_index) {
    slot_options_t *slot_options = (slot_options_t *) src->data;
    char slot_num_str[10];
    char full_path[30];
    itoa(clicked_index + 1, slot_num_str, 10);
    if (sprintf(full_path, "%s%s.save", GAME_SLOTS_PATH, slot_num_str) < 0) {
        return; // TO-DO fatal error to handle.
    }
    if (slot_options->is_loading_mode) {
        if (slot_options->is_saved_slots[clicked_index]) {
            if (load_game_from_path(src->game, full_path) == false) {
                if (show_error_message_box(src->window,
                                           "There was an error trying to load the game, please try again") < 0) {
                    return; // TO-DO fatal error to handle.
                }
            } else {
                reset_game_gui((game_gui_t *) src->window->windows->game_window->widgets[0]->data, src->game);
                src->game->is_saved = true;
                switch_to_next_window_action(src);
            }
        }
    } else { // saving a game
        // check if the slot is not disables because there is not saved game in the slot
        if (save_game_to_path(src->game, full_path) == false) {
            if (show_error_message_box(src->window, "There was an error trying to save the game, please try again") < 0)
                return; // TO-DO fatal error to handle.

        } else {
            src->game->is_saved = true;
            slot_options->is_saved_slots[clicked_index] = true;
            switch_to_next_window_action(src);
        }
    }
}

void *build_game_window(game_t *game, windows_t *windows) {
    start_game(game);
    window_t *game_window = windows->game_window;
    SDL_SetRenderDrawColor(game_window->renderer, 155, 255, 255, 0);

    widget_t *game_gui_widget = create_game_gui(game_window, game);
    add_widget_to_window(game_window, game_gui_widget);

    add_back_button_to_window(game_window, game);

    SDL_Rect undo_move_rec;
    undo_move_rec.x = 0;
    undo_move_rec.y = GAME_BOARD_HEIGHT;
    undo_move_rec.w = DEFAULT_GAME_BUTTON_WIDTH;
    undo_move_rec.h = DEFAULT_GAME_BUTTON_HEIGHT;

    SDL_Rect save_button_rec;
    save_button_rec.x = (DEFAULT_WINDOW_WIDTH - (int) (2.5 * DEFAULT_GAME_BUTTON_WIDTH));
    save_button_rec.y = GAME_BOARD_HEIGHT;
    save_button_rec.w = DEFAULT_GAME_BUTTON_WIDTH;
    save_button_rec.h = DEFAULT_GAME_BUTTON_HEIGHT;

    SDL_Rect load_button_rec;
    load_button_rec.x = (DEFAULT_WINDOW_WIDTH - (int) (1.5 * DEFAULT_GAME_BUTTON_WIDTH));
    load_button_rec.y = GAME_BOARD_HEIGHT;
    load_button_rec.w = DEFAULT_GAME_BUTTON_WIDTH;
    load_button_rec.h = DEFAULT_GAME_BUTTON_HEIGHT;

    SDL_Rect restart_button_rec;
    restart_button_rec.x = (DEFAULT_WINDOW_WIDTH - 3 * DEFAULT_GAME_BUTTON_WIDTH);
    restart_button_rec.y = GAME_BOARD_HEIGHT + DEFAULT_GAME_BUTTON_HEIGHT;
    restart_button_rec.w = DEFAULT_GAME_BUTTON_WIDTH;
    restart_button_rec.h = DEFAULT_GAME_BUTTON_HEIGHT;

    SDL_Rect main_menu_button_rec;
    main_menu_button_rec.x = (DEFAULT_WINDOW_WIDTH - 2 * DEFAULT_GAME_BUTTON_WIDTH);
    main_menu_button_rec.y = GAME_BOARD_HEIGHT + DEFAULT_GAME_BUTTON_HEIGHT;
    main_menu_button_rec.w = DEFAULT_GAME_BUTTON_WIDTH;
    main_menu_button_rec.h = DEFAULT_GAME_BUTTON_HEIGHT;

    SDL_Rect quit_button_rec;
    quit_button_rec.x = (DEFAULT_WINDOW_WIDTH - DEFAULT_GAME_BUTTON_WIDTH);
    quit_button_rec.y = GAME_BOARD_HEIGHT + DEFAULT_GAME_BUTTON_HEIGHT;
    quit_button_rec.w = DEFAULT_GAME_BUTTON_WIDTH;
    quit_button_rec.h = DEFAULT_GAME_BUTTON_HEIGHT;

    widget_t *undo_move_button = create_button(game_window, game, UNDO_MOVE_BUTTON_PATH, undo_move_rec,
                                               NULL, undo_button_action);
    widget_t *save_button = create_button(game_window, game, SAVE_GAME_BUTTON_PATH,
                                          save_button_rec,
                                          windows->pick_slot_window, save_button_action);
    widget_t *load_button = create_button(game_window, game, LOAD_GAME2_BUTTON_PATH,
                                          load_button_rec,
                                          windows->pick_slot_window, load_button_game_action);
    widget_t *restart_button = create_button(game_window, game, RESTART_BUTTON_PATH,
                                             restart_button_rec,
                                             NULL, restart_button_action);
    widget_t *main_menu_button = create_button(game_window, game, MAIN_MENU_BUTTON_PATH,
                                               main_menu_button_rec,
                                               windows->main_menu, show_unsaved_game_box_message);
    widget_t *quit_button = create_button(game_window, game, QUIT2_BUTTON_PATH,
                                          quit_button_rec,
                                          NULL, show_unsaved_game_box_message);

    add_widget_to_window(game_window, undo_move_button);
    add_widget_to_window(game_window, save_button);
    add_widget_to_window(game_window, load_button);
    add_widget_to_window(game_window, restart_button);
    add_widget_to_window(game_window, main_menu_button);
    add_widget_to_window(game_window, quit_button);


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

}

void undo_button_action(widget_t *widget) {
    switch (undo_move(widget->game)) {
        case SUCCESS:
            break;
        case EMPTY_HISTORY:
            show_error_message_box(widget->window, "ERROR: Empty history, no move to undo");
            break;
        default:
            println_error("Unexpected enum value after using cmd_undo");
            break;
    }
    reset_game_gui((game_gui_t *) widget->window->widgets[0]->data, widget->game);
}

void save_button_action(widget_t *widget) {
    widget->next_window->next_window = widget->window;
    slot_options_t *slot_options = (slot_options_t *) widget->next_window->widgets[0]->data;
    slot_options->is_loading_mode = false;
    switch_window_and_change_prev_window_action(widget);
}

void load_button_game_action(widget_t *widget) {
    widget->next_window->next_window = widget->window;
    slot_options_t *slot_options = (slot_options_t *) widget->next_window->widgets[0]->data;
    slot_options->is_loading_mode = true;
    switch_window_and_change_prev_window_action(widget);
}

void restart_button_action(widget_t *widget) {
    start_game(widget->game);
    reset_game_gui((game_gui_t *) widget->window->widgets[0]->data, widget->game);
}

void show_unsaved_game_box_message(widget_t *widget) {
    window_t *after_save_window = widget->next_window;
    window_t *pick_slot_window = widget->window->windows->pick_slot_window;
    if (!(widget->game->is_saved)) {
        SDL_MessageBoxButtonData buttons[] = {
                {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "save first"},
                {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "Cancel"},
                {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 3, "yes"}
        };
        int button_id = show_message_box(widget->window, buttons, 3, "Continue without saving?",
                                         "Are you sure you want to Continue without saving?");
        slot_options_t *slot_options;
        switch (button_id) {
            case 1:
                pick_slot_window->next_window = after_save_window;
                slot_options = (slot_options_t *) pick_slot_window->widgets[0]->data;
                slot_options->is_loading_mode = false;
                widget->next_window = widget->window->windows->pick_slot_window;
                switch_window_and_change_prev_window_action(widget);
                widget->next_window = after_save_window;
                break;
            case 0:
            case 2:
                break;
            case 3:
                if (after_save_window == NULL) {
                    widget->window->next_window_frame = QUIT;
                } else {
                    // after_save_window should be main menu
                    switch_window_and_change_prev_window_action(widget);
                }
                break;
            default:
                //error displaying the box.
                break;
        }
    } else {
        if (after_save_window == NULL) {
            widget->window->next_window_frame = QUIT;
        } else {
            // after_save_window should be main menu
            switch_window_and_change_prev_window_action(widget);
        }
    }
}
