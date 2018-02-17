
#ifndef UNI_CHESS_PROJECT_BOARD_H
#define UNI_CHESS_PROJECT_BOARD_H


#include <stdlib.h>
#include "Auxiliary.h"
#include "Structs.h"


Board* make_starting_board();


void free_board(Board *board);

bool is_empty_space(char piece);
bool is_white_piece(char piece);
/**
 * Returns false if either iece is empty space, or if they are different color (black and white)
 */
bool are_same_color(char piece1, char piece2);
#endif //UNI_CHESS_PROJECT_BOARD_H
