#ifndef SLOTOPTIONS_H_
#define SLOTOPTIONS_H_

#include <SDL.h>
#include <SDL_video.h>
#include "GUIStructs.h"
#include "ChessGameSettings.h"
#include "Window.h"
#include "FileOperations.h"

#define BLANK_SLOT_PATH (PICTURES_DIRECTORY SEP "blank_slot.bmp")
#define ARROW_UP_PATH (PICTURES_DIRECTORY SEP "arrow_up.bmp")
#define ARROW_DOWN_PATH (PICTURES_DIRECTORY SEP "arrow_down.bmp")
#define ZERO_PATH (PICTURES_DIRECTORY SEP "0.bmp")
#define ONE_PATH (PICTURES_DIRECTORY SEP "1.bmp")
#define TWO_PATH (PICTURES_DIRECTORY SEP "2.bmp")
#define THREE_PATH (PICTURES_DIRECTORY SEP "3.bmp")
#define FOUR_PATH (PICTURES_DIRECTORY SEP "4.bmp")
#define FIVE_PATH (PICTURES_DIRECTORY SEP "5.bmp")
#define SIX_PATH (PICTURES_DIRECTORY SEP "6.bmp")
#define SEVEN_PATH (PICTURES_DIRECTORY SEP "7.bmp")
#define EIGHT_PATH (PICTURES_DIRECTORY SEP "8.bmp")
#define NINE_PATH (PICTURES_DIRECTORY SEP "9.bmp")
#define SAVED_GAMES_DIRECTORY "saved-games"

#define SLOT_WIDTH 180
#define SLOT_HEIGHT 60
#define ARROWS_WIDTH 40
#define ARROWS_HEIGHT 150
#define SLOT_OPTIONS_WIDTH 220
#define SLOT_OPTIONS_HEIGHT 300
#define NUMBERS_X_ON_SLOT 14
#define NUMBERS_Y_ON_SLOT 14
#define NUMBER_OF_DRAWN_SLOTS 5

widget_t *create_slot_options(
        window_t *window,
        Game *game,
        int number_of_slots,
        int x_pos,
        int y_pos,
        void (*action)(widget_t *src, int clicked_index));

void destroy_slot_options(widget_t *src);

void handle_slot_options_event(widget_t *src, SDL_Event *event);

void draw_slot_options(widget_t *widget);

#endif //SLOTOPTIONS_H_
