#include "Minimax.h"
#include "PossibleMoveLogic.h"

ComputerMove computer_move(Game *game) {
    ComputerMove m = recursively_minimax_best_move(game->board, game->current_player == WHITE, MIN_SCORE_VALUE,
                                                   MAX_SCORE_VALUE,
                                                   game->difficulty);
    game->board->grid[m.r2][m.c2] = game->board->grid[m.r1][m.c1];
    game->board->grid[m.r1][m.c1] = EMPTY_SPACE;
    return m;
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

int calculate_simple_board_score(Board *board) {
    int sum = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            sum += PIECE_SCORES[board->grid[x][y]];
        }
    }

    return sum;
}

ComputerMove make_best_move_from_endgame_scenario(Board *board, bool player_is_white) {
    ComputerMove best_move;
    best_move.r1 = -2; // signifies that it's s leaf node - no best move from here
    // Stop, give heuristic value (board score)
    bool game_ended = !check_if_player_can_move(board, player_is_white); // gotta test every time, because draws

    if (game_ended) {
        if (check_if_king_is_threatened(board, true))  // is white Checked?
            best_move.score = SCORE_BLACK_CHECKMATE; // Black win
        else if (check_if_king_is_threatened(board, false))  // is black Checked?
            best_move.score = SCORE_WHITE_CHECKMATE; // White win
        else
            best_move.score = 0; // Draw / Tie
        return best_move;
    } else {
        best_move.score = calculate_simple_board_score(board);
        return best_move;
    }
}

ComputerMove
recursively_minimax_best_move(Board *board, bool player_is_white, int alpha, int beta, int depthRemaining) {
    ComputerMove best_move;
    best_move.r1 = -1; // signifies that there is no possible move found yet
    if (depthRemaining == 0) {
        return make_best_move_from_endgame_scenario(board, player_is_white);
    }

    best_move.score = player_is_white ? MIN_SCORE_VALUE : MAX_SCORE_VALUE;

    // Remember:
    // white is maximizing (+), black is minimizing (-)
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            char piece = board->grid[x][y];
            if (is_empty_space(piece) || is_white_piece(piece) != player_is_white)
                continue;
            PossibleMove possible_moves[MOVES_ARRAY_SIZE];
            get_possible_moves(board, x, y, possible_moves);
            int i = 0;
            for (; i < MOVES_ARRAY_SIZE; i++) {
                PossibleMove m = possible_moves[i];
                if (!m.is_legal)
                    break; // (all moves from now on are guaranteed to be not possible)

                // Recursively try each possible move
                char target_piece = board->grid[m.row][m.col];
                board->grid[x][y] = EMPTY_SPACE;
                board->grid[m.row][m.col] = piece;

                ComputerMove best_recursively = recursively_minimax_best_move(board, !player_is_white, alpha,
                                                                              beta, depthRemaining - 1);
                // Alpha-Beta Pruning
                if (player_is_white) {
                    if (best_move.score < best_recursively.score) {
                        best_move.r1 = x;
                        best_move.c1 = y;
                        best_move.r2 = m.row;
                        best_move.c2 = m.col;
                        best_move.score = best_recursively.score;
                        if (alpha < best_move.score)
                            alpha = best_move.score;
                    }
                } else {
                    if (best_move.score > best_recursively.score) {
                        best_move.r1 = x;
                        best_move.c1 = y;
                        best_move.r2 = m.row;
                        best_move.c2 = m.col;
                        best_move.score = best_recursively.score;
                        if (beta > best_move.score)
                            beta = best_move.score;
                    }
                }
                board->grid[x][y] = piece;
                board->grid[m.row][m.col] = target_piece;
                if (beta <= alpha) {
                    goto pruned;
                }
            }
        }
    }
    if (best_move.r1 == -1) {
        //Depth isn't 0, but game is still over in this scenario
        return make_best_move_from_endgame_scenario(board, player_is_white);
    }

    pruned:
    return best_move;
}



