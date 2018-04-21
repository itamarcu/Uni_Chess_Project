#include "PossibleMoveLogic.h"

/*
 * WARNING: UGLY CODE AHEAD
 * (it's well-optimized though)
 */

/**
 * This function is faster than checking for each piece if it threatens this spot
 */
bool has_enemy_in_that_direction(game_t *game, bool is_white, int row, int col, int row_delta, int col_delta) {
    int original_row = row;
    int original_col = col;
    row += row_delta;
    col += col_delta;
    while (row >= 0 && row < 8 && col >= 0 && col < 8) {
        char piece = game->board->grid[row][col];
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
                    println_error("BUG 7984324764");
                    return false;
            }
        }
        row += row_delta;
        col += col_delta;
    }
    return false;
}

void update_move_by_potential_threats(game_t *game, possible_move_t *move, int r1, int c1) {
    int r2 = move->row;
    int c2 = move->col;


    char moving_piece = game->board->grid[r1][c1];
    char target_piece = game->board->grid[r2][c2];
    bool is_white = is_white_piece(moving_piece);
    if (!is_empty_space(target_piece)) {
        if (is_white_piece(target_piece) == is_white) {
            //attempt to move into allied unit
            move->is_possible = false;
            return;
        }
    }

    //---Test for king danger and piece danger---

    //Temporarily make move attempt
    game->board->grid[r2][c2] = moving_piece;
    game->board->grid[r1][c1] = EMPTY_SPACE;
    bool will_piece_be_threatened = false;

    int rk, ck;
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            if ((game->board->grid[row][col] == BLACK_KING && !is_white)
                || (game->board->grid[row][col] == WHITE_KING && is_white)) {
                rk = row;
                ck = col;
                goto FoundKing;
            }
        }
    println_error("CRITICAL ERROR: King not found in board!!?!?");
    return;

    FoundKing:
    // for each 'inverse-reachable' position on the board, check if the unit there is an enemy that can reach
    // start with 8 directions
    for (int row_delta = -1; row_delta <= 1; row_delta++)
        for (int col_delta = -1; col_delta <= 1; col_delta++) {
            if (row_delta == 0 && col_delta == 0)
                continue;
            if (has_enemy_in_that_direction(game, is_white, rk, ck, row_delta, col_delta)) {
                //king will be threatened! no need to check anything more.
                move->is_possible = false;
                //Undo move
                game->board->grid[r2][c2] = target_piece;
                game->board->grid[r1][c1] = moving_piece;
                return;
            }
            if (!will_piece_be_threatened &&
                has_enemy_in_that_direction(game, is_white, r2, c2, row_delta, col_delta))
                will_piece_be_threatened = true;
        }

    // also check knights
    int knight_x_deltas[8] = {+1, +2, +2, +1, -1, -2, -2, -1}; //start from up-up-right and move clockwise
    int knight_y_deltas[8] = {-2, -1, +1, +2, +2, +1, -1, -2}; //start from up-up-right and move clockwise
    for (int i = 0; i < 8; i++) {
        char piece_k = game->board->grid[rk + knight_x_deltas[i]][ck + knight_y_deltas[i]];
        if (!is_empty_space(piece_k) && (is_white_piece(piece_k) != is_white)) {
            if (piece_k == WHITE_KNIGHT || piece_k == BLACK_KNIGHT) {
                //king will be threatened! no need to check anything more.
                move->is_possible = false;
                //Undo move
                game->board->grid[r2][c2] = target_piece;
                game->board->grid[r1][c1] = moving_piece;
                return;
            }
        } else if (!will_piece_be_threatened) {
            char piece_p = game->board->grid[r2 + knight_x_deltas[i]][c2 + knight_y_deltas[i]];
            if (!is_empty_space(piece_p) && (is_white_piece(piece_p) != is_white)
                && (piece_p == WHITE_KNIGHT || piece_p == BLACK_KNIGHT)) {
                will_piece_be_threatened = true;
            }

        }
    }

    // Undo the temporary move
    game->board->grid[r2][c2] = target_piece;
    game->board->grid[r1][c1] = moving_piece;

    move->is_possible = true;
    move->is_capturing = target_piece != EMPTY_SPACE;
    move->is_threatened_by_opponent = will_piece_be_threatened;
}

void add_move_to_possibilities(game_t *game, possible_move_t *moves, int index, int r1, int c1, int r2, int c2) {
    moves[index].row = r2;
    moves[index].col = c2;
    if (r2 < 0 || r2 >= 8 || c2 < 0 || c2 >= 8)
        moves[index].is_possible = false;
    else
        update_move_by_potential_threats(game, moves + index, r1, c1);
}

/**
 * @param possible_moves empty array of PossibleMove objects, to be filled. The array is guaranteed to
 * have only possible moves up to a certain index, and then only impossible moves afterwards.
 * @return result of action. Will not fill moves if action is invalid.
 */
GAME_ACTION_RESULT get_possible_moves(game_t *game, int row, int col, possible_move_t *possible_moves) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return INVALID_POS;
    }

    char piece = game->board->grid[row][col];
    if (piece == EMPTY_SPACE) {
        return NO_PIECE_IN_LOCATION;
    }

    //possible_move_t *possible_moves = (possible_move_t *) malloc(sizeof(possible_move_t) * MOVES_ARRAY_SIZE);


    for (int i = 0; i < MOVES_ARRAY_SIZE; i++) {
        possible_moves[i].is_possible = false;
    }

    int next_move_index = 0;

    switch (piece) {
        case WHITE_PAWN:
            add_move_to_possibilities(game, possible_moves, next_move_index, row, col, row + 1, col);
            possible_moves[next_move_index].is_possible &= !possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_possible)
                next_move_index += 1;
            add_move_to_possibilities(game, possible_moves, next_move_index, row, col, row + 1, col - 1);
            possible_moves[next_move_index].is_possible &= possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_possible)
                next_move_index += 1;
            add_move_to_possibilities(game, possible_moves, next_move_index, row, col, row + 1, col + 1);
            possible_moves[next_move_index].is_possible &= possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_possible)
                next_move_index += 1;
            if (row == 1) //starting move
            {
                add_move_to_possibilities(game, possible_moves, next_move_index, row, col, row + 2, col);
                possible_moves[next_move_index].is_possible &= !possible_moves[next_move_index].is_capturing;
            }
            break;
        case BLACK_PAWN:
            add_move_to_possibilities(game, possible_moves, next_move_index, row, col, row - 1, col);
            possible_moves[next_move_index].is_possible &= !possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_possible)
                next_move_index += 1;

            add_move_to_possibilities(game, possible_moves, next_move_index, row, col, row - 1, col - 1);
            possible_moves[next_move_index].is_possible &= possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_possible)
                next_move_index += 1;
            add_move_to_possibilities(game, possible_moves, next_move_index, row, col, row - 1, col + 1);
            possible_moves[next_move_index].is_possible &= possible_moves[next_move_index].is_capturing;
            if (possible_moves[next_move_index].is_possible)
                next_move_index += 1;
            if (row == 6) //starting move
            {
                add_move_to_possibilities(game, possible_moves, next_move_index, row, col, row - 2, col);
                possible_moves[next_move_index].is_possible &= !possible_moves[next_move_index].is_capturing;
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
                        add_move_to_possibilities(game, possible_moves, next_move_index, row, col, r, c);
                        if (!possible_moves[next_move_index].is_possible) //if blocked, no need to check more in this direction
                            break;
                        next_move_index += 1;
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
                        add_move_to_possibilities(game, possible_moves, next_move_index, row, col, r, c);
                        if (!possible_moves[next_move_index].is_possible) //if blocked, no need to check more in this direction
                            break;
                        next_move_index += 1;
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
                add_move_to_possibilities(game, possible_moves, next_move_index, row, col, r, c);
                if (possible_moves[next_move_index].is_possible)
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
                        add_move_to_possibilities(game, possible_moves, next_move_index, row, col, r, c);
                        if (!possible_moves[next_move_index].is_possible) //if blocked, no need to check more in this direction
                            break;
                        next_move_index += 1;
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
                        add_move_to_possibilities(game, possible_moves, next_move_index, row, col, r, c);
                        if (possible_moves[next_move_index].is_possible)
                            next_move_index += 1;
                    }
                }
            }
            break;
        default:
            println_error("BUG 123978134");
            break;
    }

    return SUCCESS;
}