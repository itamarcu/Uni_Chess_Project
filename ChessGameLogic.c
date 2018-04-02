#include "ChessGameLogic.h"

bool is_partially_legal_move(char grid[8][8], int r1, int c1, int r2, int c2);

/**
 * Helper function - checks for move legality with the assumptions that
 * all parameters are inside the grid and grid[r1][c1] has been emptied.
 */
bool is_partially_legal_move_without_start(char grid[8][8], int r1, int c1, int r2, int c2, char moving_piece,
                                           char target_piece);

void start_game(game_t *game) {
    game->state = GAME_STATE_GAME;
    game->current_user = WHITE;
    game->winner = GAME_WINNER_NONE;
    free_board(game->board);
    game->board = make_starting_board();
}

bool is_current_checked(game_t *game) {
    bool is_white_king = game->current_user == WHITE;
    int rk, ck;
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            if ((game->board->grid[row][col] == BLACK_KING && !is_white_king)
                || (game->board->grid[row][col] == WHITE_KING && is_white_king)) {
                rk = row;
                ck = col;
                goto FoundKing;
            }
        }

    println_error("CRITICAL ERROR: King not found in board!!?!?");
    return false; //should never happen!!!

    FoundKing:
    //for each enemy unit, check if it's able to capture the king
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            if (!is_empty_space(game->board->grid[row][col]) &&
                is_white_piece(game->board->grid[row][col]) != is_white_king) {
                if (is_partially_legal_move(game->board->grid, row, col, rk, ck)) {
                    return true;
                }
            }
        }

    return false;
}

GAME_ACTION_RESULT move_was_made(game_t *game) {
    //Check if next player is checkmated
    bool game_ended = check_checkmate(game, game->current_user != WHITE);

    if (game_ended) {
        game->state = GAME_STATE_QUIT;
    } else {
        change_current_player(game);
    }
}

bool check_checkmate(game_t *game, bool checking_for_white) {
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            char piece = game->board->grid[row][col];
            if (is_empty_space(piece))
                continue;
            if (is_white_piece(piece) == checking_for_white)
                continue;
            possible_move_t moves[MOVES_ARRAY_SIZE];
            if (get_possible_moves(game, row, col, moves) != SUCCESS)
                println_error("BUG 15907624");

            if (moves[0].is_possible)
                return false;
        }
    return true;
}

void change_current_player(game_t *game) {
    if (game->current_user == WHITE) {
        game->current_user = BLACK;
    } else {
        game->current_user = WHITE;
    }
}

/**
 * Helper function - checks for move legality with the assumption that
 * all parameters are inside the grid.
 */
bool is_partially_legal_move(char grid[8][8], int r1, int c1, int r2, int c2) {
    char moving_piece = grid[r1][c1];
    char target_piece = grid[r2][c2];
    grid[r1][c1] = '_'; //helps make code cleaner in other function
    bool result = is_partially_legal_move_without_start(grid, r1, c1, r2, c2, moving_piece, target_piece);
    grid[r1][c1] = moving_piece; //undoes the thing
    return result;
}

/**
 * Helper function - checks for move legality with the assumptions that
 * all parameters are inside the grid and grid[r1][c1] has been emptied.
 */
bool is_partially_legal_move_without_start(char grid[8][8], int r1, int c1, int r2, int c2,
                                           char moving_piece, char target_piece) {
    switch (moving_piece) {
        case WHITE_PAWN:
            //advance 2 forward from start, or 1 forward, through and into empty spaces
            if ((c1 == c2 && is_empty_space(grid[r1 + 1][c1]) && is_empty_space(target_piece)) &&
                ((r2 == 3 && r1 == 1) || (r2 == r1 + 1))) {
                return true;
            }
                //advance 1 diagonally forward into an enemy piece
            else if ((c2 == c1 - 1 || c2 == c1 + 1) && r2 == r1 + 1
                     && !is_empty_space(target_piece) && !is_white_piece(target_piece)) {
                return true;
            }
            return false;
        case BLACK_PAWN:
            //advance 2 forward from start, or 1 forward, through and into empty spaces
            if ((c1 == c2 && is_empty_space(grid[r1 - 1][c1]) && is_empty_space(target_piece)) &&
                ((r2 == 4 && r1 == 6) || (r2 == r1 - 1))) {
                return true;
            }
                //advance 1 diagonally forward into an enemy piece
            else if ((c2 == c1 - 1 || c2 == c1 + 1) && r2 == r1 - 1
                     && !is_empty_space(target_piece) && is_white_piece(target_piece)) {
                return true;
            }
            return false;
        case BLACK_ROOK:
        case WHITE_ROOK:
            //move in a straight unobstructed line
            if (r1 == r2) {
                for (int col = min(c1, c2); col < max(c1, c2); col++) {
                    if (!is_empty_space(grid[r1][col])) {
                        return false;
                    }
                }
                return true;
            }
            if (c1 == c2) {
                for (int row = min(r1, r2); row < max(r1, r2); row++) {
                    if (!is_empty_space(grid[row][c1])) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        case BLACK_BISHOP:
        case WHITE_BISHOP:
            //move in a 45 degree diagonal unobstructed line
        {
            int delta_row = r2 - r1;
            int delta_col = c2 - c1;
            if (abs(delta_row) != abs(delta_col)) {
                return false;
            }

            int dr = sign(delta_row);
            int dc = sign(c2 - c1);
            for (int row = r1, col = c1; row != r2 && col != c2; row += dr, col += dc) {
                if (!is_empty_space(grid[row][col])) {
                    return false;
                }
            }
            return true;
        }
        case BLACK_QUEEN:
        case WHITE_QUEEN:
            //move in a straight or diagonal unobstructed line.
            //I'll just copy paste from bishop and rook, sorry.
            if (r1 == r2) {
                for (int col = min(c1, c2); col < max(c1, c2); col++) {
                    if (!is_empty_space(grid[r1][col])) {
                        return false;
                    }
                }
                return true;
            }
            if (c1 == c2) {
                for (int row = min(r1, r2); row < max(r1, r2); row++) {
                    if (!is_empty_space(grid[row][c1])) {
                        return false;
                    }
                }
                return true;
            }

            int delta_row = r2 - r1;
            int delta_col = c2 - c1;
            if (abs(delta_row) != abs(delta_col)) {
                return false;
            }

            int dr = sign(delta_row);
            int dc = sign(c2 - c1);
            for (int row = r1, col = c1; row != r2 && col != c2; row += dr, col += dc) {
                if (!is_empty_space(grid[row][col])) {
                    return false;
                }
            }
            return true;
        case BLACK_KNIGHT:
        case WHITE_KNIGHT:
            //can move 2 in any axis and 1 in another axis, may 'skip' pieces.
            //only need to check validity of position! :)
            return r1 != r2 && c1 != c2 && abs(r2 - r1) + abs(c2 - c1) == 3;
        case BLACK_KING:
        case WHITE_KING:
            //move 1 square in any adjacent direction (including diagonals)
            return abs(r2 - r1) <= 1 && abs(c2 - c1) <= 1;
        default:
            println_error("unknown piece! %c", moving_piece);
            return false;
    }
}

/**
 * This uses zero-based indexing for rows and columns, please remember
 */
GAME_ACTION_RESULT console_cmd_move(game_t *game, int r1, int c1, int r2, int c2) {
    if (0 > r1 || r1 >= 8 || 0 > c1 || c1 >= 8 || 0 > r2 || r2 >= 8 || 0 > c2 || c2 >= 8) {
        return INVALID_POS;
    }

    char moving_piece = game->board->grid[r1][c1];
    char target_piece = game->board->grid[r2][c2];

    //If moving from empty, or moving enemy unit
    if (is_empty_space(moving_piece) || (is_white_piece(moving_piece) != (game->current_user == WHITE))) {
        return NO_PIECE_IN_LOCATION;
    }
    //If moving into ally unit
    if (!is_empty_space(target_piece) && (is_white_piece(moving_piece) == is_white_piece(target_piece))) {
        return ILLEGAL_MOVE;
    }

    bool is_legal_move = is_partially_legal_move(game->board->grid, r1, c1, r2, c2);

    if (!is_legal_move) {
        return ILLEGAL_MOVE;
    }

    //Now we must test for king danger
    bool king_already_checked = is_current_checked(game);
    //Make move attempt
    game->board->grid[r2][c2] = moving_piece;
    game->board->grid[r1][c1] = EMPTY_SPACE;
    int king_will_be_checked = is_current_checked(game);
    if (king_will_be_checked) {
        //undo move attempt
        game->board->grid[r2][c2] = target_piece;
        game->board->grid[r1][c1] = moving_piece;
        if (king_already_checked)
            return KING_STILL_THREATENED;
        else
            return KING_WILL_BE_THREATENED;
    }

    //Move attempt has been done and results will stay
    return SUCCESS;
}
