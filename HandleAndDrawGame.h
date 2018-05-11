#ifndef HANDLE_AND_DRAW_GAME_H_
#define HANDLE_AND_DRAW_GAME_H_

#include <SDL.h>
#include <SDL_video.h>
#include <math.h>
#include "GUIStructs.h"
#include "ChessGameSettings.h"
#include "Window.h"
#include "ChessGameLogic.h"
#include "Minimax.h"

#define BLACK_BISHOP_PATH (PICTURES_DIRECTORY SEP "black_bishop.bmp")
#define BLACK_KING_PATH (PICTURES_DIRECTORY SEP "black_king.bmp")
#define BLACK_KNIGHT_PATH (PICTURES_DIRECTORY SEP "black_knight.bmp")
#define BLACK_PAWN_PATH (PICTURES_DIRECTORY SEP "black_pawn.bmp")
#define BLACK_QUEEN_PATH (PICTURES_DIRECTORY SEP "black_queen.bmp")
#define BLACK_ROOK_PATH (PICTURES_DIRECTORY SEP "black_rook.bmp")
#define WHITE_BISHOP_PATH (PICTURES_DIRECTORY SEP "white_bishop.bmp")
#define WHITE_KING_PATH (PICTURES_DIRECTORY SEP "white_king.bmp")
#define WHITE_KNIGHT_PATH (PICTURES_DIRECTORY SEP "white_knight.bmp")
#define WHITE_PAWN_PATH (PICTURES_DIRECTORY SEP "white_pawn.bmp")
#define WHITE_QUEEN_PATH (PICTURES_DIRECTORY SEP "white_queen.bmp")
#define WHITE_ROOK_PATH (PICTURES_DIRECTORY SEP "white_rook.bmp")

#define STANDARD_SQUARE_PATH (PICTURES_DIRECTORY SEP "standard_square.bmp")
#define THREATENED_SQUARE_PATH (PICTURES_DIRECTORY SEP "threatened_square.bmp")
#define CAPTURE_SQUARE_PATH (PICTURES_DIRECTORY SEP "capture_square.bmp")
#define THREATENED_CAPTURE_SQUARE_PATH (PICTURES_DIRECTORY SEP "threatened_capture_square.bmp")
#define FOCUSED_SQUARE_PATH (PICTURES_DIRECTORY SEP "focused_square.bmp")
#define FOCUSED_SQUARE_ENEMY_PATH (PICTURES_DIRECTORY SEP "focused_square_enemy.bmp")

#define BOARD_W_H 700
#define AVERAGE_SQUARE_WIDTH 83.125
#define AVERAGE_SQUARE_HEIGHT 82.875
#define FIRST_SQUARE_X_Y 19
#define FIRST_PIECE_X_Y 28
#define PIECE_DEST_W_H 64

/**
 * Creates the GUI widget for the game window.
 * @return pointer to widget containing game_gui_t as data. return NULL on error.
 */
widget_t *create_game_gui(
        window_t *window,
        Game *game);

/**
 * Resets game GUI so that there is no selected piece, no highlights, etc.
 */
void reset_game_gui(game_gui_t *game_gui, Game *game);

/**
 * Special-case function - will make the computer's first move when the game is loaded, if needed.
 */
void maybe_make_first_computer_turn(Game *game, game_gui_t *game_gui, window_t *window);


#endif /* HANDLE_AND_DRAW_GAME_ */
