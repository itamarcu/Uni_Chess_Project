#ifndef HANDLE_AND_DRAW_GAME_H_
#define HANDLE_AND_DRAW_GAME_H_

#include <SDL.h>
#include <SDL_video.h>
#include <math.h>
#include "GUIStructs.h"
#include "ChessGameSettings.h"
#include "Window.h"

#define BLACK_BISHOP_PATH "..\\pictures\\black_bishop.bmp"
#define BLACK_KING_PATH "..\\pictures\\black_king.bmp"
#define BLACK_KNIGHT_PATH "..\\pictures\\black_knight.bmp"
#define BLACK_PAWN_PATH "..\\pictures\\black_pawn.bmp"
#define BLACK_QUEEN_PATH "..\\pictures\\black_queen.bmp"
#define BLACK_ROOK_PATH "..\\pictures\\black_rook.bmp"
#define WHITE_BISHOP_PATH "..\\pictures\\white_bishop.bmp"
#define WHITE_KING_PATH "..\\pictures\\white_king.bmp"
#define WHITE_KNIGHT_PATH "..\\pictures\\white_knight.bmp"
#define WHITE_PAWN_PATH "..\\pictures\\white_pawn.bmp"
#define WHITE_QUEEN_PATH "..\\pictures\\white_queen.bmp"
#define WHITE_ROOK_PATH "..\\pictures\\white_rook.bmp"

#define STANDARD_SQUARE_PATH "..\\pictures\\standard_square.bmp"
#define THREATENED_SQUARE_PATH "..\\pictures\\threatened_square.bmp"
#define CAPTURE_SQUARE_PATH "..\\pictures\\capture_square.bmp"
#define THREATENED_CAPTURE_SQUARE_PATH "..\\pictures\\threatened_capture_square.bmp"

#define BOARD_W_H 700
#define AVERAGE_SQUARE_WIDTH 83.125
#define AVERAGE_SQUARE_HEIGHT 82.875
#define FIRST_SQUARE_X_Y 18
#define FIRST_PEICE_X_Y 28
#define PEICE_DEST_W_H 64

widget_t *create_game_gui(
        window_t *window,
        game_t *game);

void update_game_gui_board(game_gui_t *game_gui, game_t *game);

void reset_game_gui(game_gui_t *game_gui, game_t *game);

void destroy_game_gui(widget_t *src);

void handle_game_gui_event(widget_t *src, SDL_Event *event);

void draw_game_gui(widget_t *widget);

#endif /* HANDLE_AND_DRAW_GAME_ */
