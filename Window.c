#include "Window.h"

void destroy_window(window_t *src) {
    int i;
    for (i = 0; i < src->num_of_widgets; i++) {
        destroy_widget(src->widgets[i]);
    }
    for (i = 0; i < src->num_of_tex; i++) {
        free(src->tex_rects[i]);
    }
    for (i = 0; i < src->num_of_tex; i++) {
        SDL_DestroyTexture(src->textures[i]);
    }
    free(src->widgets);
    free(src->tex_rects);
    free(src->textures);
    SDL_DestroyRenderer(src->renderer);
    SDL_DestroyWindow(src->window);
    free(src);
}

GAME_WINDOW handle_window_event(window_t *src, SDL_Event *e) {
    int i;
    widget_t *widget;
    for (i = 0; i < src->num_of_widgets; i++) {
        widget = src->widgets[i];
        widget->handle_event(widget, e);
    }
    switch (e->type) {
        case SDL_QUIT:
            src->next_window_frame = QUIT;
            break;
        case SDL_WINDOWEVENT:
            switch (e->window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    src->next_window_frame = QUIT;
                    break;
                default:
                    break;
            }
        default:
            break;
    }
    return src->next_window_frame;
}

void draw_window(window_t *src) {
    int i;
    widget_t *widget;
    SDL_Texture *texture;
    SDL_Rect *texRect;
    SDL_Renderer *renderer = src->renderer;
    SDL_RenderClear(renderer);
    for (i = 0; i < src->num_of_tex; i++) {
        texture = src->textures[i];
        texRect = src->tex_rects[i];
        SDL_RenderCopy(renderer, texture, NULL, texRect);
    }
    for (i = 0; i < src->num_of_widgets; i++) {
        widget = src->widgets[i];
        widget->draw(widget);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

SDL_Texture *create_texture_from_path(char *tex_path, SDL_Renderer *rend) {
    SDL_Surface *surface = SDL_LoadBMP(tex_path);
    if (surface == NULL) {
//        printf("ERROR: unable to load image: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}

void add_texture_to_window(window_t *src, SDL_Texture *texture, SDL_Rect *rect) {
    src->textures[src->num_of_tex] = texture;
    src->tex_rects[src->num_of_tex] = rect;
    src->num_of_tex++;
}

void add_widget_to_window(window_t *src, widget_t *widget) {
    src->widgets[src->num_of_widgets] = widget;
    src->num_of_widgets++;
}

void add_chess_BG_and_title(window_t *src, char *title_path) {
    SDL_Renderer *rend = src->renderer;
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    SDL_Surface *background_surface = SDL_LoadBMP(DEFAULT_BG_PATH);
    if (background_surface == NULL) {
        printf("ERROR: unable to load image: %s\n", SDL_GetError());
        destroy_window(src);
        return;
    }
    SDL_Texture *background_texture = SDL_CreateTextureFromSurface(rend, background_surface);
    if (background_texture == NULL) {
        printf("ERROR: unable to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(background_surface);
        destroy_window(src);
        return;
    }
    SDL_FreeSurface(background_surface);
    SDL_Texture *title_texture;
    if ((title_texture = create_texture_from_path(title_path, rend)) == NULL) {
        printf("ERROR: unable to create texture: %s\n", SDL_GetError());
        SDL_DestroyTexture(background_texture);
        destroy_window(src);
        return;
    }

    SDL_Rect *title_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    int title_width, title_height = 0;
    SDL_QueryTexture(title_texture, NULL, NULL, &title_width, &title_height);
    title_rect->x = (src->width - title_width) / 2;
    title_rect->y = (src->height - title_height) / 10;
    title_rect->w = title_width;
    title_rect->h = title_height;

    add_texture_to_window(src, background_texture, NULL);
    add_texture_to_window(src, title_texture, title_rect);
}

window_t *create_empty_centered_window(int window_width, int window_height, int num_of_textures, int num_of_widgets,
                                     GAME_WINDOW game_window) {
    window_t *window = (window_t *) malloc(sizeof(window_t));
    window->window = SDL_CreateWindow(
            "Chess Game",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width,
            window_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (window->window == NULL) {
        printf("ERROR: unable to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }
    SDL_HideWindow(window->window);
    window->height = window_height;
    window->width = window_width;
    window->renderer = SDL_CreateRenderer(window->window, -1,
                                          SDL_RENDERER_PRESENTVSYNC); // need to check if should add SDL_RENDERER_ACCELERATED or SDL_RENDERER_SOFTWARE (should match settings in the VM)
    if (window->renderer == NULL) {
        printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window->window);
        SDL_Quit();
        return NULL;
    }
    window->textures = (SDL_Texture **) malloc((num_of_textures + 1) * sizeof(SDL_Texture *));
    window->tex_rects = (SDL_Rect **) malloc((num_of_textures + 1) * sizeof(SDL_Rect *));
    window->widgets = (widget_t **) malloc((num_of_widgets + 1) * sizeof(widget_t *));

    window->num_of_tex = 0;
    window->num_of_widgets = 0;
    window->window_enum_val = game_window;
    window->next_window_frame = game_window;
    return window;
}

void add_back_button_to_window(window_t *window, game_t *game) {
    SDL_Rect new_game_button_rect;
    new_game_button_rect.x = 0;
    new_game_button_rect.y = window->height - DEFAULT_BACK_BUTTON_HEIGHT;
    new_game_button_rect.w = DEFAULT_BACK_BUTTON_WIDTH;
    new_game_button_rect.h = DEFAULT_BACK_BUTTON_HEIGHT;
    widget_t *back_button = create_button(window, game, DEFAULT_BACK_BUTTON_PATH,
                                          new_game_button_rect, NULL, switch_to_prev_window_action);
    add_widget_to_window(window, back_button);
}

void show_window_at_pos(window_t *next_window, int window_posX, int window_posY) {
    SDL_SetWindowPosition(next_window->window, window_posX, window_posY);
    draw_window(next_window);
    SDL_ShowWindow(next_window->window);
    SDL_RaiseWindow(next_window->window);
}

int show_message_box(window_t *window, SDL_MessageBoxButtonData buttons[], int num_of_buttons, const char *box_title,
                     const char *message) {
    const SDL_MessageBoxColorScheme colorScheme = {
            { /* .colors (.r, .g, .b) */
                    /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                    {255, 0, 0},
                    /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                    {0, 255, 0},
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                    {255, 255, 0},
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                    {0, 0, 255},
                    /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                    {255, 0, 255}
            }
    };
    const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            window->window, /* .window */
            box_title, /* .title */
            message, /* .message */
            num_of_buttons, /* .numbuttons */
            buttons, /* .buttons */
            &colorScheme /* .colorScheme */
    };

    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return -1;
    }
    if (buttonid == -1) {
        SDL_Log("no selection");
        return 0;
    } else {
        SDL_Log("selection was %s", buttons[buttonid].text);
        return buttonid;
    }
}

int show_error_message_box(window_t *window, const char *message) {
    SDL_MessageBoxButtonData buttons[] = {
            {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Ok"}
    };
    return show_message_box(window, buttons, 1, "ERROR", message);
}
