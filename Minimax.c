#include "Minimax.h"
#include "PossibleMoveLogic.h"

ComputerMove computer_move(game_t *game) {
    bool computer_is_white = (game->user_color == BLACK);
    int board_score = calculate_simple_board_score(game->board);
    return recursively_minimax_best_move(game->board, computer_is_white, board_score);
}

/**
 * global/static "dictionary", mapping pieces (chars, implicitly converted to ints) to their scoring values.
 */
int PIECE_SCORES[256];

void init_scoring_function() {
    for (int i = 0; i < 256; i++) {
        PIECE_SCORES[i] = 0;
    }
    PIECE_SCORES[EMPTY_SPACE] = 0;
    PIECE_SCORES[WHITE_PAWN] = +1;
    PIECE_SCORES[BLACK_PAWN] = -1;
    PIECE_SCORES[WHITE_BISHOP] = +3;
    PIECE_SCORES[BLACK_BISHOP] = -3;
    PIECE_SCORES[WHITE_KNIGHT] = +3;
    PIECE_SCORES[BLACK_KNIGHT] = -3;
    PIECE_SCORES[WHITE_ROOK] = +5;
    PIECE_SCORES[BLACK_ROOK] = -5;
    PIECE_SCORES[WHITE_QUEEN] = +9;
    PIECE_SCORES[BLACK_QUEEN] = -9;
    PIECE_SCORES[WHITE_KING] = +100;
    PIECE_SCORES[BLACK_KING] = -100;
}

int calculate_simple_board_score(board_t *board) {
    int sum = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            sum += PIECE_SCORES[board->grid[x][y]];
        }
    }

    return sum;
}

ComputerMove recursively_minimax_best_move(board_t *board, bool player_is_white, int board_score) {
    ComputerMove best_move;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            char piece = board->grid[x][y];
            if (is_empty_space(piece) || is_white_piece(piece) != player_is_white)
                continue;
            possible_move_t possible_moves[MOVES_ARRAY_SIZE];
            get_possible_moves(board, x, y, possible_moves);
            for (int i = 0; i < MOVES_ARRAY_SIZE; i++) {
                possible_move_t m = possible_moves[i];
                if (!m.is_possible)
                    break; // (all moves from now on are guaranteed to be not possible)
                // Recursively try each possible move
                board_t *possible_board = copy_board(board);
                int new_board_score = board_score;
                char captured_piece = possible_board->grid[m.row][m.col];
                if (!is_empty_space(captured_piece)) {
                    new_board_score -= PIECE_SCORES[captured_piece];
                }
                possible_board->grid[x][y] = EMPTY_SPACE;
                possible_board->grid[m.row][m.col] = piece;
            }
        }
    }

    best_move.r1 = 1;
    best_move.c1 = 2;
    best_move.r2 = 3;
    best_move.c2 = 4;
    best_move.moving_piece = 'M';
    return best_move;
}



