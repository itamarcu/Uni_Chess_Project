#include "ChessBoard.h"


Board *make_starting_board() {
    Board *board = malloc(sizeof(Board));
    char whiteFirstRow[8] = {
            WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK
    };
    char blackFirstRow[8] = {
            BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK
    };
    if (board == NULL) {
        println_error("MALLOC failure in make_starting_board!");
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

Board *copy_board(Board *board) {
    Board *clone = malloc(sizeof(Board));
    if (clone == NULL) {
        println_error("MALLOC failure in copy_board!");
        return NULL;
    }
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            clone->grid[i][j] = board->grid[i][j];

    return clone;
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

const char *name_of_piece(char piece) {
    switch (piece) {
        case EMPTY_SPACE:
            return "EMPTY_SPACE_THIS_IS_A_BUG";
        case WHITE_PAWN:
        case BLACK_PAWN:
            return "pawn";
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            return "bishop";
        case WHITE_ROOK:
        case BLACK_ROOK:
            return "rook";
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            return "knight";
        case WHITE_QUEEN:
        case BLACK_QUEEN:
            return "queen";
        case WHITE_KING:
        case BLACK_KING:
            return "king";
        default:
            println_error("Bug 1659189572037, piece is %c", piece);
            return "BUG";
    }
}

bool is_piece_or_empty_space(char c) {
    switch (c) {
        case WHITE_PAWN:
        case BLACK_PAWN:
        case WHITE_BISHOP:
        case BLACK_BISHOP:
        case WHITE_ROOK:
        case BLACK_ROOK:
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
        case WHITE_QUEEN:
        case BLACK_QUEEN:
        case WHITE_KING:
        case BLACK_KING:
        case EMPTY_SPACE:
            return true;
        default:
            return false;
    }
}