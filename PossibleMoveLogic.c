#include "PossibleMoveLogic.h"

/**
* Returns true if the move is valid (this is much faster than update_move_by_potential_threats)
*
* NOTE: this assumes correct input (correct r1, r2, nonempty...)
*/
bool optimized_move_legality_check(Board *board, int r1, int c1, int r2, int c2);

bool has_enemy_in_that_direction(Board *board, bool is_white, int row, int col, int row_delta, int col_delta);

void update_move_by_potential_threats(Board *board, PossibleMove *move, int r1, int c1);

void add_move_to_possibilities(Board *board, PossibleMove *moves, int index, int r1, int c1, int r2, int c2);


bool optimized_move_legality_check(Board *board, int r1, int c1, int r2, int c2) {
    char moving_piece = board->grid[r1][c1];
    char target_piece = board->grid[r2][c2];
    bool is_white = is_white_piece(moving_piece);
    if (!is_empty_space(target_piece)) {
        if (is_white_piece(target_piece) == is_white) {
            //attempt to move into allied unit
            return false;
        }
    }

    //---Test for king danger and piece danger---

    //Temporarily make move attempt
    board->grid[r2][c2] = moving_piece;
    board->grid[r1][c1] = EMPTY_SPACE;

    int rk, ck;
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            if ((board->grid[row][col] == BLACK_KING && !is_white)
                || (board->grid[row][col] == WHITE_KING && is_white)) {
                rk = row;
                ck = col;
                goto FoundKing;
            }
        }
    println_error("CRITICAL ERROR 21357987: King not found in board!!?!?");
    return false;

    FoundKing:
    // for each 'inverse-reachable' position on the board, check if the unit there is an enemy that can reach
    // start with 8 directions
    for (int row_delta = -1; row_delta <= 1; row_delta++)
        for (int col_delta = -1; col_delta <= 1; col_delta++) {
            if (row_delta == 0 && col_delta == 0)
                continue;
            if (has_enemy_in_that_direction(board, is_white, rk, ck, row_delta, col_delta)) {
                //king will be threatened! no need to check anything more.
                //Undo move
                board->grid[r2][c2] = target_piece;
                board->grid[r1][c1] = moving_piece;
                return false;
            }
        }

    // also check knights
    int knight_x_deltas[8] = {+1, +2, +2, +1, -1, -2, -2, -1}; //start from up-up-right and move clockwise
    int knight_y_deltas[8] = {-2, -1, +1, +2, +2, +1, -1, -2}; //start from up-up-right and move clockwise
    for (int i = 0; i < 8; i++) {
        int row_from_k = rk + knight_x_deltas[i];
        int col_from_k = ck + knight_y_deltas[i];
        if (row_from_k < 0 || row_from_k >= 8 || col_from_k < 0 || col_from_k >= 8)
            continue;
        char piece_k = board->grid[row_from_k][col_from_k];
        if (!is_empty_space(piece_k) && (is_white_piece(piece_k) != is_white)) {
            if (piece_k == WHITE_KNIGHT || piece_k == BLACK_KNIGHT) {
                //king will be threatened! no need to check anything more.
                //Undo move
                board->grid[r2][c2] = target_piece;
                board->grid[r1][c1] = moving_piece;
                return false;
            }
        }
    }

    // Undo the temporary move
    board->grid[r2][c2] = target_piece;
    board->grid[r1][c1] = moving_piece;

    return true;
}

bool has_enemy_in_that_direction(Board *board, bool is_white, int row, int col, int row_delta, int col_delta) {
    int original_row = row;
    int original_col = col;
    row += row_delta;
    col += col_delta;
    while (row >= 0 && row < 8 && col >= 0 && col < 8) {
        char piece = board->grid[row][col];
        if (!is_empty_space(piece)) {
            if (is_white_piece(piece) == is_white)
                return false; //found allied unit blocking that direction of attack
            switch (piece) {
                case WHITE_PAWN:
                    return original_row - 1 == row && (original_col + 1 == col
                                                       || original_col - 1 == col);
                case BLACK_PAWN:
                    return original_row + 1 == row && (original_col + 1 == col
                                                       || original_col - 1 == col);
                case WHITE_BISHOP:
                case BLACK_BISHOP:
                    return row_delta * col_delta != 0; // diagonal
                case WHITE_ROOK:
                case BLACK_ROOK:
                    return row_delta * col_delta == 0; // horizontal or vertical
                case WHITE_KNIGHT:
                case BLACK_KNIGHT:
                    return false; //those two are checked someplace else, can't capture in directions
                case WHITE_QUEEN:
                case BLACK_QUEEN:
                    return true;
                case WHITE_KING:
                case BLACK_KING:
                    return abs(row - original_row) <= 1 && abs(col - original_col) <= 1;
                default:
                    println_error("BUG 7984324764 with piece %c", piece);
                    return false;
            }
        }
        row += row_delta;
        col += col_delta;
    }
    return false;
}

void update_move_by_potential_threats(Board *board, PossibleMove *move, int r1, int c1) {
    int r2 = move->row;
    int c2 = move->col;


    char moving_piece = board->grid[r1][c1];
    char target_piece = board->grid[r2][c2];
    bool is_white = is_white_piece(moving_piece);
    if (!is_empty_space(target_piece)) {
        if (is_white_piece(target_piece) == is_white) {
            //attempt to move into allied unit
            move->is_legal = false;
            return;
        }
    }

    //---Test for king danger and piece danger---

    //Temporarily make move attempt
    board->grid[r2][c2] = moving_piece;
    board->grid[r1][c1] = EMPTY_SPACE;
    bool will_piece_be_threatened = false;

    int rk, ck;
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            if ((board->grid[row][col] == BLACK_KING && !is_white)
                || (board->grid[row][col] == WHITE_KING && is_white)) {
                rk = row;
                ck = col;
                goto FoundKing;
            }
        }
    println_error("CRITICAL ERROR 975312: King not found in board!!?!?");
    return;

    FoundKing:
    // for each 'inverse-reachable' position on the board, check if the unit there is an enemy that can reach
    // start with 8 directions
    for (int row_delta = -1; row_delta <= 1; row_delta++)
        for (int col_delta = -1; col_delta <= 1; col_delta++) {
            if (row_delta == 0 && col_delta == 0)
                continue;
            if (has_enemy_in_that_direction(board, is_white, rk, ck, row_delta, col_delta)) {
                //king will be threatened! no need to check anything more.
                move->is_legal = false;
                //Undo move
                board->grid[r2][c2] = target_piece;
                board->grid[r1][c1] = moving_piece;
                return;
            }
            if (!will_piece_be_threatened &&
                has_enemy_in_that_direction(board, is_white, r2, c2, row_delta, col_delta))
                will_piece_be_threatened = true;
        }

    // also check knights
    int knight_x_deltas[8] = {+1, +2, +2, +1, -1, -2, -2, -1}; //start from up-up-right and move clockwise
    int knight_y_deltas[8] = {-2, -1, +1, +2, +2, +1, -1, -2}; //start from up-up-right and move clockwise
    for (int i = 0; i < 8; i++) {
        int row_from_k = rk + knight_x_deltas[i];
        int col_from_k = ck + knight_y_deltas[i];
        if (!(row_from_k < 0 || row_from_k >= 8 || col_from_k < 0 || col_from_k >= 8)) {
            char piece_k = board->grid[row_from_k][col_from_k];
            if (!is_empty_space(piece_k) && (is_white_piece(piece_k) != is_white)) {
                if (piece_k == WHITE_KNIGHT || piece_k == BLACK_KNIGHT) {
                    //king will be threatened! no need to check anything more.
                    move->is_legal = false;
                    //Undo move
                    board->grid[r2][c2] = target_piece;
                    board->grid[r1][c1] = moving_piece;
                    return;
                }
            }
        }
        if (!will_piece_be_threatened) {
            int row_from_p = r2 + knight_x_deltas[i];
            int col_from_p = c2 + knight_y_deltas[i];
            if (row_from_p < 0 || row_from_p >= 8 || col_from_p < 0 || col_from_p >= 8)
                continue;
            char piece_p = board->grid[row_from_p][col_from_p];
            if (!is_empty_space(piece_p) && (is_white_piece(piece_p) != is_white)
                && (piece_p == WHITE_KNIGHT || piece_p == BLACK_KNIGHT)) {
                will_piece_be_threatened = true;
            }

        }
    }

    // Undo the temporary move
    board->grid[r2][c2] = target_piece;
    board->grid[r1][c1] = moving_piece;

    move->is_legal = true;
    move->is_capturing = target_piece != EMPTY_SPACE;
    move->is_threatened_by_opponent = will_piece_be_threatened;
}

void add_move_to_possibilities(Board *board, PossibleMove *moves, int index, int r1, int c1, int r2, int c2) {
    moves[index].row = r2;
    moves[index].col = c2;
    if (r2 < 0 || r2 >= 8 || c2 < 0 || c2 >= 8)
        moves[index].is_legal = false;
    else
        update_move_by_potential_threats(board, moves + index, r1, c1);
}

GAME_ACTION_RESULT
get_possible_moves(Board *board, int row, int col, PossibleMove possible_moves[MOVES_ARRAY_SIZE]) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return INVALID_POS;
    }

    char piece = board->grid[row][col];
    if (piece == EMPTY_SPACE) {
        return NO_PIECE_IN_LOCATION;
    }

    for (int i = 0; i < MOVES_ARRAY_SIZE; i++) {
        possible_moves[i].is_legal = false;
    }

    int next_move_index = 0;

    switch (piece) {
        case WHITE_PAWN:
            add_move_to_possibilities(board, possible_moves, next_move_index, row, col, row + 1, col);
            possible_moves[next_move_index].is_legal &= !possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_legal)
                next_move_index += 1;
            add_move_to_possibilities(board, possible_moves, next_move_index, row, col, row + 1, col - 1);
            possible_moves[next_move_index].is_legal &= possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_legal)
                next_move_index += 1;
            add_move_to_possibilities(board, possible_moves, next_move_index, row, col, row + 1, col + 1);
            possible_moves[next_move_index].is_legal &= possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_legal)
                next_move_index += 1;
            if (row == 1 && is_empty_space(board->grid[row + 1][col])) //starting move
            {
                add_move_to_possibilities(board, possible_moves, next_move_index, row, col, row + 2, col);
                possible_moves[next_move_index].is_legal &= !possible_moves[next_move_index].is_capturing;
            }
            break;
        case BLACK_PAWN:
            add_move_to_possibilities(board, possible_moves, next_move_index, row, col, row - 1, col);
            possible_moves[next_move_index].is_legal &= !possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_legal)
                next_move_index += 1;

            add_move_to_possibilities(board, possible_moves, next_move_index, row, col, row - 1, col - 1);
            possible_moves[next_move_index].is_legal &= possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_legal)
                next_move_index += 1;
            add_move_to_possibilities(board, possible_moves, next_move_index, row, col, row - 1, col + 1);
            possible_moves[next_move_index].is_legal &= possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_legal)
                next_move_index += 1;
            if (row == 6 && is_empty_space(board->grid[row - 1][col])) //starting move
            {
                add_move_to_possibilities(board, possible_moves, next_move_index, row, col, row - 2, col);
                possible_moves[next_move_index].is_legal &= !possible_moves[next_move_index].is_capturing;
            }
            break;
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            for (int row_delta = -1; row_delta <= 1; row_delta++) {
                for (int col_delta = -1; col_delta <= 1; col_delta++) {
                    if (row_delta == 0 && col_delta == 0)
                        continue;
                    if (row_delta * col_delta == 0) //if not diagonal
                        continue;
                    int r = row + row_delta;
                    int c = col + col_delta;
                    for (; 0 <= r && r < 8 && 0 <= c && c < 8; r += row_delta, c += col_delta) {
                        add_move_to_possibilities(board, possible_moves, next_move_index, row, col, r, c);

                        // Do not change the order here:
                        PossibleMove *possible_move = &possible_moves[next_move_index];
                        if (possible_move->is_legal)
                            next_move_index += 1;
                        if (!is_empty_space(board->grid[r][c]))
                            break;
                    }
                }
            }
            break;
        case WHITE_ROOK:
        case BLACK_ROOK:
            for (int row_delta = -1; row_delta <= 1; row_delta++) {
                for (int col_delta = -1; col_delta <= 1; col_delta++) {
                    if (row_delta == 0 && col_delta == 0)
                        continue;
                    if (row_delta * col_delta != 0) //if not horizontal/vertical
                        continue;
                    int r = row + row_delta;
                    int c = col + col_delta;
                    for (; 0 <= r && r < 8 && 0 <= c && c < 8; r += row_delta, c += col_delta) {
                        add_move_to_possibilities(board, possible_moves, next_move_index, row, col, r, c);

                        // Do not change the order here:
                        PossibleMove *possible_move = &possible_moves[next_move_index];
                        if (possible_move->is_legal)
                            next_move_index += 1;
                        if (!is_empty_space(board->grid[r][c]))
                            break;
                    }
                }
            }
            break;
        case WHITE_KNIGHT:
        case BLACK_KNIGHT: {
            //start from up-up-right and move clockwise
            int knight_x_deltas[8] = {+1, +2, +2, +1, -1, -2, -2, -1}; //
            int knight_y_deltas[8] = {-2, -1, +1, +2, +2, +1, -1, -2}; //

            for (int i = 0; i < 8; i++) {
                int r = row + knight_x_deltas[i];
                int c = col + knight_y_deltas[i];
                add_move_to_possibilities(board, possible_moves, next_move_index, row, col, r, c);
                if (possible_moves[next_move_index].is_legal)
                    next_move_index += 1;
            }
        }
            break;
        case WHITE_QUEEN:
        case BLACK_QUEEN:
            for (int row_delta = -1; row_delta <= 1; row_delta++) {
                for (int col_delta = -1; col_delta <= 1; col_delta++) {
                    if (row_delta == 0 && col_delta == 0)
                        continue;
                    int r = row + row_delta;
                    int c = col + col_delta;
                    for (; 0 <= r && r < 8 && 0 <= c && c < 8; r += row_delta, c += col_delta) {
                        add_move_to_possibilities(board, possible_moves, next_move_index, row, col, r, c);

                        // Do not change the order here:
                        PossibleMove *possible_move = &possible_moves[next_move_index];
                        if (possible_move->is_legal)
                            next_move_index += 1;
                        if (!is_empty_space(board->grid[r][c]))
                            break;
                    }
                }
            }
            break;
        case WHITE_KING:
        case BLACK_KING:
            for (int row_delta = -1; row_delta <= 1; row_delta++) {
                for (int col_delta = -1; col_delta <= 1; col_delta++) {
                    if (row_delta == 0 && col_delta == 0)
                        continue;
                    int r = row + row_delta;
                    int c = col + col_delta;
                    if (0 <= r && r < 8 && 0 <= c && c < 8) {
                        add_move_to_possibilities(board, possible_moves, next_move_index, row, col, r, c);
                        if (possible_moves[next_move_index].is_legal)
                            next_move_index += 1;
                    }
                }
            }
            break;
        default:
            println_error("BUG 123978134 with piece %c (%d)", piece, piece);
            break;
    }

    return SUCCESS;
}

bool has_any_possible_moves(Board *board, int row, int col) {
    char piece = board->grid[row][col];

    switch (piece) {
        case WHITE_PAWN:
            if (
                    (row < 7 && is_empty_space(board->grid[row + 1][col]) &&
                     optimized_move_legality_check(board, row, col, row + 1, col))
                    ||
                    (row < 7 && col > 0 && !is_empty_space(board->grid[row + 1][col - 1]) &&
                     optimized_move_legality_check(board, row, col, row + 1, col - 1))
                    ||
                    (row < 7 && col < 7 && !is_empty_space(board->grid[row + 1][col + 1]) &&
                     optimized_move_legality_check(board, row, col, row + 1, col + 1))
                    ||
                    (row == 1 && is_empty_space(board->grid[row + 1][col]) &&
                     optimized_move_legality_check(board, row, col, row + 2, col))
                    ) {
                return true;

            }
            break;
        case BLACK_PAWN:
            if (
                    (row > 0 && is_empty_space(board->grid[row - 1][col]) &&
                     optimized_move_legality_check(board, row, col, row - 1, col))
                    ||
                    (row > 0 && col > 0 && !is_empty_space(board->grid[row - 1][col - 1]) &&
                     optimized_move_legality_check(board, row, col, row - 1, col - 1))
                    ||
                    (row > 0 && col < 7 && !is_empty_space(board->grid[row - 1][col + 1]) &&
                     optimized_move_legality_check(board, row, col, row - 1, col + 1))
                    ||
                    (row == 6 && is_empty_space(board->grid[row - 1][col]) &&
                     optimized_move_legality_check(board, row, col, row - 2, col))
                    ) {
                return true;
            }

            break;
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            for (int row_delta = -1; row_delta <= 1; row_delta++) {
                for (int col_delta = -1; col_delta <= 1; col_delta++) {
                    if (row_delta == 0 && col_delta == 0)
                        continue;
                    if (row_delta * col_delta == 0) //if not diagonal
                        continue;
                    int r = row + row_delta;
                    int c = col + col_delta;
                    for (; 0 <= r && r < 8 && 0 <= c && c < 8; r += row_delta, c += col_delta) {
                        if (optimized_move_legality_check(board, row, col, r, c)) {
                            return true;
                        }
                        if (!is_empty_space(board->grid[r][c]))
                            break;
                    }
                }
            }
            break;
        case WHITE_ROOK:
        case BLACK_ROOK:
            for (int row_delta = -1; row_delta <= 1; row_delta++) {
                for (int col_delta = -1; col_delta <= 1; col_delta++) {
                    if (row_delta == 0 && col_delta == 0)
                        continue;
                    if (row_delta * col_delta != 0) //if not horizontal/vertical
                        continue;
                    int r = row + row_delta;
                    int c = col + col_delta;
                    for (; 0 <= r && r < 8 && 0 <= c && c < 8; r += row_delta, c += col_delta) {
                        if (optimized_move_legality_check(board, row, col, r, c)) {
                            return true;
                        }
                        if (!is_empty_space(board->grid[r][c]))
                            break;
                    }
                }
            }
            break;
        case WHITE_KNIGHT:
        case BLACK_KNIGHT: {
            //start from up-up-right and move clockwise
            int knight_x_deltas[8] = {+1, +2, +2, +1, -1, -2, -2, -1}; //
            int knight_y_deltas[8] = {-2, -1, +1, +2, +2, +1, -1, -2}; //

            for (int i = 0; i < 8; i++) {
                int r = row + knight_x_deltas[i];
                int c = col + knight_y_deltas[i];
                if (optimized_move_legality_check(board, row, col, r, c)) {
                    return true;
                }
            }
        }
            break;
        case WHITE_QUEEN:
        case BLACK_QUEEN:
            for (int row_delta = -1; row_delta <= 1; row_delta++) {
                for (int col_delta = -1; col_delta <= 1; col_delta++) {
                    if (row_delta == 0 && col_delta == 0)
                        continue;
                    int r = row + row_delta;
                    int c = col + col_delta;
                    for (; 0 <= r && r < 8 && 0 <= c && c < 8; r += row_delta, c += col_delta) {
                        if (optimized_move_legality_check(board, row, col, r, c)) {
                            return true;
                        }
                        if (!is_empty_space(board->grid[r][c]))
                            break;
                    }
                }
            }
            break;
        case WHITE_KING:
        case BLACK_KING:
            for (int row_delta = -1; row_delta <= 1; row_delta++) {
                for (int col_delta = -1; col_delta <= 1; col_delta++) {
                    if (row_delta == 0 && col_delta == 0)
                        continue;
                    int r = row + row_delta;
                    int c = col + col_delta;
                    if (0 <= r && r < 8 && 0 <= c && c < 8) {
                        if (optimized_move_legality_check(board, row, col, r, c)) {
                            return true;
                        }
                    }
                }
            }
            break;
        default:
            println_error("BUG 789745643213 with piece %c (%d)", piece, piece);
            break;
    }

    return false;
}