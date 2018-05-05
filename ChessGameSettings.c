#include "ChessGameSettings.h"

bool save_game_to_path(game_t *game, char *path) {
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        fclose(f);
        return false;
    }
    char *color = "white\n";
    if (game->current_player != WHITE) {
        color = "black\n";
    }
    if (fputs(color, f) == EOF) {
        fclose(f);
        return false;
    }
    char *settings = "SETTINGS:\n";
    if (fputs(settings, f) == EOF) {
        fclose(f);
        return false;
    }
    char *game_mode = "GAME_MODE: 1-player\n";
    if (game->game_mode == GAME_MODE_MULTIPLAYER) {
        game_mode = "GAME_MODE: 2-player\n";
    }
    if (fputs(game_mode, f) == EOF) {
        fclose(f);
        return false;
    }
    if (game->game_mode == GAME_MODE_SINGLEPLAYER) {
        char *diff;
        switch (game->difficulty) {
            case AMATEUR_DIFFICULTY:
                diff = "DIFFICULTY: amateur\n";
                break;
            case EASY_DIFFICULTY:
                diff = "DIFFICULTY: easy\n";
                break;
            case MODERATE_DIFFICULTY:
                diff = "DIFFICULTY: moderate\n";
                break;
            case HARD_DIFFICULTY:
                diff = "DIFFICULTY: hard\n";
                break;
            case EXPERT_DIFFICULTY:
                diff = "DIFFICULTY: expert\n";
                break;
            default:
                fclose(f);
                return false;
        }
        if (fputs(diff, f) == EOF) {
            fclose(f);
            return false;
        }

        char *user_color = "USER_COLOR: white\n";
        if (game->user_color == BLACK) {
            user_color = "USER_COLOR: black\n";
        }
        if (fputs(user_color, f) == EOF) {
            fclose(f);
            return false;
        }
    }
    char board_row[MAX_FILE_ROW_SIZE] = {0};
    for (int row = 7; row >= 0; row--) {
        if (sprintf(board_row, "%d|", (row + 1)) < 0) {
            fclose(f);
            return false;
        }
        int col;
        for (col = 0; col < 8; col++) {
            if (sprintf(&board_row[2 + col * 2], " %c", game->board->grid[row][col]) < 0) {
                fclose(f);
                return false;
            }
        }
        if (sprintf(&board_row[2 + col * 2], " |\n") < 0) {
            fclose(f);
            return false;
        }
        if (fputs(board_row, f) == EOF) {
            fclose(f);
            return false;
        }
    }
    char *dashes_row = "  -----------------\n";
    if (fputs(dashes_row, f) == EOF) {
        fclose(f);
        return false;
    }
    char *letters_row = "   A B C D E F G H\n";
    if (fputs(letters_row, f) == EOF) {
        fclose(f);
        return false;
    }
    fclose(f);
    return true;
}


bool load_game_from_path(game_t *game, char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        fclose(f);
        return false;
    }
    char color[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(color, MAX_FILE_ROW_SIZE, f) == NULL) {
        fclose(f);
        return false;
    }
    if (strcmp(color, "white\n") == 0)
        game->current_player = WHITE;
    else {
        if (strcmp(color, "black\n") == 0)
            game->current_player = BLACK;
        else {
            fclose(f);
            return false;
        }
    }
    char settings[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(settings, MAX_FILE_ROW_SIZE, f) == NULL) {
        fclose(f);
        return false;
    }
    if (strcmp(settings, "SETTINGS:\n") != 0) {
        fclose(f);
        return false;
    }
    char game_mode[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(game_mode, MAX_FILE_ROW_SIZE, f) == NULL) {
        fclose(f);
        return false;
    }
    if (strcmp(game_mode, "GAME_MODE: 1-player\n") == 0)
        game->game_mode = GAME_MODE_SINGLEPLAYER;
    else {
        if (strcmp(game_mode, "GAME_MODE: 2-player\n") == 0)
            game->game_mode = GAME_MODE_MULTIPLAYER;
        else {
            fclose(f);
            return false;
        }
    }
    if (game->game_mode == GAME_MODE_SINGLEPLAYER) {
        char diff[MAX_FILE_ROW_SIZE] = {0};
        if (fgets(diff, MAX_FILE_ROW_SIZE, f) == NULL) {
            fclose(f);
            return false;
        }
        if (strncmp(diff, "DIFFICULTY: ", 12) != 0) // 12 is the size of string "DIFFICULTY: "
        {
            fclose(f);
            return false;
        }
        switch (diff[12]) { //the character in the location 12 in diff should be 1 of 4 different characters
            case 'a':
                if (strncmp(&diff[12], "amateur\n", MAX_FILE_ROW_SIZE) == 0)
                    game->difficulty = AMATEUR_DIFFICULTY;
                else {
                    fclose(f);
                    return false;
                }
                break;
            case 'e':
                if (strncmp(&diff[12], "easy\n", MAX_FILE_ROW_SIZE) == 0)
                    game->difficulty = EASY_DIFFICULTY;
                else {
                    if (strncmp(&diff[12], "expert\n", MAX_FILE_ROW_SIZE) == 0)
                        game->difficulty = EXPERT_DIFFICULTY;
                    else {
                        fclose(f);
                        return false;
                    }
                    fclose(f);
                    return false;
                }
                break;
            case 'm':
                if (strncmp(&diff[12], "moderate\n", MAX_FILE_ROW_SIZE) == 0)
                    game->difficulty = MODERATE_DIFFICULTY;
                else {
                    fclose(f);
                    return false;
                }
                break;
            case 'h':
                if (strncmp(&diff[12], "hard\n", MAX_FILE_ROW_SIZE) == 0)
                    game->difficulty = HARD_DIFFICULTY;
                else {
                    fclose(f);
                    return false;
                }
                break;
            default:
                fclose(f);
                return false;
        }
        char user_color[MAX_FILE_ROW_SIZE] = {0};
        if (fgets(user_color, MAX_FILE_ROW_SIZE, f) == NULL) {
            fclose(f);
            return false;
        }
        if (strcmp(user_color, "USER_COLOR: white\n") == 0)
            game->user_color = WHITE;
        else {
            if (strcmp(user_color, "USER_COLOR: black\n") == 0)
                game->user_color = BLACK;
            else {
                fclose(f);
                return false;
            }
        }
    }

    char board_row[MAX_FILE_ROW_SIZE] = {0};
    for (int row = 7; row >= 0; row--) {
        if (fgets(board_row, MAX_FILE_ROW_SIZE, f) == NULL) {
            fclose(f);
            return false;
        }
        char expected_row_start[2];
        if (sprintf(expected_row_start, "%d|", (row + 1)) < 0) {
            fclose(f);
            return false;
        }
        if (strncmp(board_row, expected_row_start, 2) != 0) {
            fclose(f);
            return false;
        }
        int col;
        for (col = 0; col < 8; col++) {
            if (board_row[2 + col * 2] != ' ' || !is_piece_or_empty_space(board_row[2 + col * 2 + 1])) {
                fclose(f);
                return false;
            }
            game->board->grid[row][col] = board_row[2 + col * 2 + 1];
        }
        if (strncmp(&board_row[2 + col * 2], " |\n", 3) != 0) {
            fclose(f);
            return false;
        }
    }
    char dashes_row[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(dashes_row, MAX_FILE_ROW_SIZE, f) == NULL) {
        fclose(f);
        return false;
    }
    if (strncmp(dashes_row, "  -----------------\n", MAX_FILE_ROW_SIZE) != 0) {
        fclose(f);
        return false;
    }
    char letters_row[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(letters_row, MAX_FILE_ROW_SIZE, f) == NULL) {
        fclose(f);
        return false;
    }
    if (strncmp(letters_row, "   A B C D E F G H\n", MAX_FILE_ROW_SIZE) != 0) {
        fclose(f);
        return false;
    }
    if (fgets(letters_row, 2, f) != NULL) {
        fclose(f);
        return false;
    } // if its true there is more written to the file we loading.
    fclose(f);
    return true;
}

bool is_file_exists(char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        fclose(f);
        return false;
    }
    fclose(f);
    return true;
}

bool is_file_empty(char *path) {
    FILE *f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    bool is_empty = (ftell(f) == 0);
    fclose(f);
    return is_empty;
}

void reset_default_settings(game_t *game) {
    game->difficulty = 2;
    game->game_mode = GAME_MODE_SINGLEPLAYER;
    game->user_color = WHITE;
    game->state = GAME_STATE_SETTINGS;
    game->is_saved = false;
}

void free_history(History *history) {
    if (history == NULL)
        return;
    for (int i = 0; i < history->count; i++) {
        free_board(history->prev_boards[i]);
    }
    free(history->prev_boards);
    free(history->prev_moves);
    free(history);
}

void free_game(game_t *game) {
    free_board(game->board);
    free_history(game->history);
    free(game);
}

char *difficulty_string(int difficulty_int) {
    switch (difficulty_int) {
        case AMATEUR_DIFFICULTY:
            return "amateur";
        case EASY_DIFFICULTY:
            return "easy";
        case MODERATE_DIFFICULTY:
            return "moderate";
        case HARD_DIFFICULTY:
            return "hard";
        case EXPERT_DIFFICULTY:
            return "expert";
        default:
            println_error("Difficulty is not in range 1 to 5");
            return "plz fix ur input";
    }
}

char *color_string(int color_int) {
    if (color_int == WHITE) {
        return "white";
    } else if (color_int == BLACK) {
        return "black";
    }
    println_error("wrong color int was input into color_string(), should be WHITE or BLACK");
    return "asian";
}