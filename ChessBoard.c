#include "ChessBoard.h"


Board *make_starting_board() {
    Board *board = (Board *) malloc(sizeof(*board));
    char whiteFirstRow[8] = {WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK};
    char blackFirstRow[8] = {BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK};
    if (board == NULL) {
        return NULL;
    }
    for (int i = 2; i <= 5; i++)
        for (int j = 0; j < 8; j++)
            board->grid[i][j] = EMPTY_SPACE;

    for (int k = 0; k < 8; k++) {
        board->grid[0][k] = whiteFirstRow[k];
        board->grid[1][k] = WHITE_PAWN;
        board->grid[6][k] = BLACK_PAWN;
        board->grid[7][k] = blackFirstRow[k];
    }
    return board;
}

void free_board(Board *board) {
    free(board);
}


bool is_empty_space(char piece) {
    return piece == EMPTY_SPACE;
}

bool is_white_piece(char piece) {
    return tolower(piece) == piece;
}