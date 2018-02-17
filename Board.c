
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

/**
 * example:
 *
 * 8| R _ _ n _ K _ _ |
 * 7| M M _ _ _ _ _ N |
 * 6| _ _ M _ _ _ M M |
 * 5| _ _ _ _ _ _ _ _ |
 * 4| _ _ _ q _ _ _ _ |
 * 3| _ b _ _ _ _ _ m |
 * 2| m _ m _ _ k m _ |
 * 1| _ _ _ _ r _ _ _ |
 *   -----------------
 *    A B C D E F G H
 */
void print_board(Board *board) {
    for (int row = 7; row >= 0; row--) {
        printf("%d|", (row + 1));
        for (int col = 0; col < 8; col++) {
            printf(" %c", board->grid[row][col]);
        }
        printf(" |\n");
    }
    printf("  -----------------\n"); // that's 2 spaces and 17 dashes
    printf("   A B C D E F G H\n");  // that's 3 spaces and A B ... H
}


bool is_empty_space(char piece) {
    return piece == '_';
}

bool is_white_piece(char piece) {
    return tolower(piece) == piece;
}