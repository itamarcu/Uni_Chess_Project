#include "OptionsButtons.h"

widget_t *create_options_buttons(window_t *window, Game *game, widget_t **buttons, int num_of_buttons) {
    widget_t *res = (widget_t *) malloc(sizeof(widget_t));
    if (res == NULL)
        goto PRINT_ERROR_RETURN_NULL;

    options_buttons_t *data = (options_buttons_t *) malloc(sizeof(options_buttons_t));
    if (data == NULL) {
        free(res);
        goto PRINT_ERROR_RETURN_NULL;
    }
    res->window = window;
    res->game = game;
    res->destroy = destroy_options_buttons;
    res->draw = draw_options_buttons;
    res->handle_event = handle_options_buttons_event;
    res->data = data;
    data->buttons = buttons;
    data->num_of_buttons = num_of_buttons;
    data->current_pushed_button = 0;
    return res;

    PRINT_ERROR_RETURN_NULL:

    println_error("ERROR: problem occurred when trying to create the buttons options widget");
    return NULL;

}

void destroy_options_buttons(widget_t *src) {
    options_buttons_t *options_buttons = (options_buttons_t *) src->data;
    for (int i = 0; i < options_buttons->num_of_buttons; i++) {
        destroy_button(options_buttons->buttons[i]);
    }
    free(options_buttons->buttons);
    free(options_buttons);
    free(src);
}

void handle_options_buttons_event(widget_t *src, SDL_Event *e) {
    options_buttons_t *options_buttons = (options_buttons_t *) src->data;
    widget_t **buttons = options_buttons->buttons;
    SDL_Point mouse_pos = {.x = e->button.x, .y = e->button.y};
    button_t *button;
    switch (e->type) {
        case SDL_MOUSEBUTTONUP:
            for (int i = 0; i < options_buttons->num_of_buttons; i++) {
                button = (button_t *) (buttons[i]->data);
                if (SDL_PointInRect(&mouse_pos, &button->location)) {
                    ((button_t *) (buttons[options_buttons->current_pushed_button]->data))->current_alpha_factor = 255; // unclick last pushed button.
                    options_buttons->current_pushed_button = i;
                    button->action(src);
                    button->current_alpha_factor = ALPHA_FACTOR_MOUSE_OVER;
                }
            }
            break;
        default:
            break;
    }
}

void draw_options_buttons(widget_t *src) {
    options_buttons_t *options_buttons = (options_buttons_t *) src->data;
    widget_t **buttons = options_buttons->buttons;
    for (int i = 0; i < options_buttons->num_of_buttons; i++) {
        draw_button(buttons[i]);
    }
}