#include "ChessGameLogic.h"
#include "Undo.h"


int start_game(Game *game) {
    game->state = GAME_STATE_GAME;
    game->current_player = WHITE;
    game->winner = GAME_WINNER_NONE;
    free_board(game->board);
    game->board = make_starting_board();
    if (game->board == NULL)
        return -1;
    free_history(game->history);
    game->history = malloc(sizeof(History));
    if (game->history == NULL) {
        println_error("ERROR: could not allocate memory for game->history");
        return -1;
    }
    game->history->count = 0;
    push_move_to_history(game, 1, 2, 3, 4); // arbitrary values - not important
    return 0;
}

bool check_if_king_is_threatened(Board *board, bool checking_for_white) {
    int rk, ck;
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            if ((board->grid[row][col] == BLACK_KING && !checking_for_white)
                || (board->grid[row][col] == WHITE_KING && checking_for_white)) {
                rk = row;
                ck = col;
                goto FoundKing;
            }
        }

    println_error("CRITICAL ERROR 879456: King not found in board!!?!?");
    return false; //should never happen!!!

    FoundKing:
    //for each enemy unit, check if it's able to capture the king
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            if (!is_empty_space(board->grid[row][col]) &&
                is_white_piece(board->grid[row][col]) != checking_for_white) {
                if (is_partially_legal_move(board->grid, row, col, rk, ck)) {
                    return true;
                }
            }
        }

    return false;
}

void move_was_made(Game *game, int r1, int c1, int r2, int c2) {
    //Push board to undo history
    push_move_to_history(game, r1, c1, r2, c2);

    //Check if next player is checkmated
    bool game_ended = !check_if_player_can_move(game->board, game->current_player != WHITE);

    if (game_ended) {
        game->state = GAME_STATE_QUIT;
        if (check_if_king_is_threatened(game->board, true))  // is white Checked?
            game->winner = GAME_WINNER_BLACK;
        else if (check_if_king_is_threatened(game->board, false))  // is black Checked?
            game->winner = GAME_WINNER_WHITE;
        else
            game->winner = GAME_WINNER_DRAW;
    }

    if (game->program_mode == PROGRAM_MODE_CONSOLE && check_if_king_is_threatened(game->board, !game->current_player))
        println_output("Check: %s king is threatened", color_string(!game->current_player));

    change_current_player(game);
}

/**
 * Returns true if the checked player has 0 possible moves
 * @param game the game
 * @param checking_for_white true if checking for white player
 */
bool check_if_player_can_move(Board *board, bool checking_for_white) {
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            char piece = board->grid[row][col];
            if (is_empty_space(piece))
                continue;
            if (is_white_piece(piece) != checking_for_white)
                continue;
            if (has_any_possible_moves(board, row, col))
                return true;
        }
    return false;
}

void change_current_player(Game *game) {
    if (game->current_player == WHITE) {
        game->current_player = BLACK;
    } else {
        game->current_player = WHITE;
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
 *
 * Does not check for threats or king danger.
 */
bool is_partially_legal_move_without_start(char grid[8][8], int r1, int c1, int r2, int c2,
                                           char moving_piece, char target_piece) {
    int dr = sign(r2 - r1);
    int dc = sign(c2 - c1);
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
            if (dr == 0) {
                for (int col = c1 + dc; col != c2; col += dc) {
                    if (!is_empty_space(grid[r1][col])) {
                        return false;
                    }
                }
                return true;
            }
            if (dc == 0) {
                for (int row = r1 + dr; row != r2; row += dr) {
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
            if (abs(r2 - r1) != abs(c2 - c1)) {
                return false;
            }

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
            if (dr == 0) {
                for (int col = c1 + dc; col != c2; col += dc) {
                    if (!is_empty_space(grid[r1][col])) {
                        return false;
                    }
                }
                return true;
            }
            if (dc == 0) {
                for (int row = r1 + dr; row != r2; row += dr) {
                    if (!is_empty_space(grid[row][c1])) {
                        return false;
                    }
                }
                return true;
            }

            if (abs(r2 - r1) != abs(c2 - c1)) {
                return false;
            }

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
GAME_ACTION_RESULT console_cmd_move(Game *game, int r1, int c1, int r2, int c2) {
    if (0 > r1 || r1 >= 8 || 0 > c1 || c1 >= 8 || 0 > r2 || r2 >= 8 || 0 > c2 || c2 >= 8) {
        return INVALID_POS;
    }

    char moving_piece = game->board->grid[r1][c1];
    char target_piece = game->board->grid[r2][c2];

    //If moving from empty, or moving enemy unit
    if (is_empty_space(moving_piece) || (is_white_piece(moving_piece) != (game->current_player == WHITE))) {
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
    bool king_already_checked = check_if_king_is_threatened(game->board, game->current_player == WHITE);
    //Make move attempt
    game->board->grid[r2][c2] = moving_piece;
    game->board->grid[r1][c1] = EMPTY_SPACE;
    int king_will_be_checked = check_if_king_is_threatened(game->board, game->current_player == WHITE);
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
