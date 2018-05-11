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
#define NUMBERS_X_ON_SLOT 14
#define NUMBERS_Y_ON_SLOT 14
#define NUMBER_OF_DRAWN_SLOTS 5

/**
 * creating a widget containing slot_options_t * as its data.
 * using all the digits from 0 - 9 pictures as surfaces and creating all the textures needed for the slots.
 * @param number_of_slots the total number of slots that can be used. assuming number_of_slots > 0;
 * @param x_pos the position in x axis of the first slot drawn on window.
 * @param y_pos the position in y axis of the first slot drawn on window.
 * @param action the action to make when clicking on one of the slot, when they are not disabled.
 * @return a pointer to the widget containing slot_options_t * as its data. returns NULL on error.
 */
widget_t *create_slot_options(
        window_t *window,
        Game *game,
        int number_of_slots,
        int x_pos,
        int y_pos,
        void (*action)(widget_t *src, int clicked_index));

#endif //SLOTOPTIONS_H_
