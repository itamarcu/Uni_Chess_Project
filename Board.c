
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "Board.h"


Board *make_starting_board() {
    Board *board = (Board *) malloc(sizeof(*board));
    if (board == NULL) {
        return NULL;
    }

    char grid[8][8] = {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
                       {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M'},
                       {'_', '_', '_', '_', '_', '_', '_', '_'},
                       {'_', '_', '_', '_', '_', '_', '_', '_'},
                       {'_', '_', '_', '_', '_', '_', '_', '_'},
                       {'_', '_', '_', '_', '_', '_', '_', '_'},
                       {'m', 'm', 'm', 'm', 'm', 'm', 'm', 'm'},
                       {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}};

    //Copy array with reverse row order, because that's how it is...
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board->grid[8 - 1 - i][j] = grid[i][j];


    return board;
}

void free_board(Board *board) {
    free(board);
}


bool is_empty_space(char piece) {
    return piece == '_';
}

bool is_white_piece(char piece) {
    return tolower(piece) == piece;
}