#include "FileOperations.h"
#include "Auxiliary.h"
#include "ChessGameLogic.h"

bool save_game_to_path(Game *game, char *path) {

    FILE *f = fopen(path, "w");
    if (f == NULL) {
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


bool load_game_from_path(Game *game, char *path) {
    struct game_t *game_before_load = malloc(sizeof(struct game_t));
    if (game_before_load == NULL) {
        return false;
    }
    memcpy((void *) game_before_load, (void *) game, sizeof(struct game_t));
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return false;
    }

    if (start_game(game) < 0) {
        println_error_weak("ERROR: problem occurred when trying to start a new game, try again");
        return false;
    }

    char color[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(color, MAX_FILE_ROW_SIZE, f) == NULL) {
        goto HANDLE_ERROR;
    }
    if (strcmp(color, "white\n") == 0)
        game->current_player = WHITE;
    else {

        if (strcmp(color, "black\n") == 0)
            game->current_player = BLACK;
        else {
            goto HANDLE_ERROR;
        }
    }
    char settings[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(settings, MAX_FILE_ROW_SIZE, f) == NULL) {
        goto HANDLE_ERROR;
    }
    if (strcmp(settings, "SETTINGS:\n") != 0) {
        goto HANDLE_ERROR;
    }
    char game_mode[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(game_mode, MAX_FILE_ROW_SIZE, f) == NULL) {
        goto HANDLE_ERROR;
    }
    if (strcmp(game_mode, "GAME_MODE: 1-player\n") == 0)
        game->game_mode = GAME_MODE_SINGLEPLAYER;
    else {
        if (strcmp(game_mode, "GAME_MODE: 2-player\n") == 0)
            game->game_mode = GAME_MODE_MULTIPLAYER;
        else {
            goto HANDLE_ERROR;
        }
    }
    if (game->game_mode == GAME_MODE_SINGLEPLAYER) {
        char diff[MAX_FILE_ROW_SIZE] = {0};
        if (fgets(diff, MAX_FILE_ROW_SIZE, f) == NULL) {
            goto HANDLE_ERROR;
        }
        if (strncmp(diff, "DIFFICULTY: ", 12) != 0) // 12 is the size of string "DIFFICULTY: "
        {
            goto HANDLE_ERROR;
        }
        switch (diff[12]) { //the character in the location 12 in diff should be 1 of 4 different characters
            case 'a':
                if (strncmp(&diff[12], "amateur\n", MAX_FILE_ROW_SIZE) == 0)
                    game->difficulty = AMATEUR_DIFFICULTY;
                else {
                    goto HANDLE_ERROR;
                }
                break;
            case 'e':
                if (strncmp(&diff[12], "easy\n", MAX_FILE_ROW_SIZE) == 0)
                    game->difficulty = EASY_DIFFICULTY;
                else {
                    if (strncmp(&diff[12], "expert\n", MAX_FILE_ROW_SIZE) == 0)
                        game->difficulty = EXPERT_DIFFICULTY;
                    else {
                        goto HANDLE_ERROR;
                    }
                    goto HANDLE_ERROR;
                }
                break;
            case 'm':
                if (strncmp(&diff[12], "moderate\n", MAX_FILE_ROW_SIZE) == 0)
                    game->difficulty = MODERATE_DIFFICULTY;
                else {
                    goto HANDLE_ERROR;
                }
                break;
            case 'h':
                if (strncmp(&diff[12], "hard\n", MAX_FILE_ROW_SIZE) == 0)
                    game->difficulty = HARD_DIFFICULTY;
                else {
                    goto HANDLE_ERROR;
                }
                break;
            default:
                goto HANDLE_ERROR;
        }
        char user_color[MAX_FILE_ROW_SIZE] = {0};
        if (fgets(user_color, MAX_FILE_ROW_SIZE, f) == NULL) {
            goto HANDLE_ERROR;
        }
        if (strcmp(user_color, "USER_COLOR: white\n") == 0)
            game->user_color = WHITE;
        else {
            if (strcmp(user_color, "USER_COLOR: black\n") == 0)
                game->user_color = BLACK;
            else {
                goto HANDLE_ERROR;
            }
        }
    }

    char board_row[MAX_FILE_ROW_SIZE] = {0};
    for (int row = 7; row >= 0; row--) {
        if (fgets(board_row, MAX_FILE_ROW_SIZE, f) == NULL) {
            goto HANDLE_ERROR;
        }
        char expected_row_start[2];
        if (sprintf(expected_row_start, "%d|", (row + 1)) < 0) {
            goto HANDLE_ERROR;
        }
        if (strncmp(board_row, expected_row_start, 2) != 0) {
            goto HANDLE_ERROR;
        }
        int col;
        for (col = 0; col < 8; col++) {
            if (board_row[2 + col * 2] != ' ' || !is_piece_or_empty_space(board_row[2 + col * 2 + 1])) {
                goto HANDLE_ERROR;
            }
            game->board->grid[row][col] = board_row[2 + col * 2 + 1];
        }
        if (strncmp(&board_row[2 + col * 2], " |\n", 3) != 0) {
            goto HANDLE_ERROR;
        }
    }
    char dashes_row[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(dashes_row, MAX_FILE_ROW_SIZE, f) == NULL) {
        goto HANDLE_ERROR;
    }
    if (strncmp(dashes_row, "  -----------------\n", MAX_FILE_ROW_SIZE) != 0) {
        goto HANDLE_ERROR;
    }
    char letters_row[MAX_FILE_ROW_SIZE] = {0};
    if (fgets(letters_row, MAX_FILE_ROW_SIZE, f) == NULL) {
        goto HANDLE_ERROR;
    }
    if (strncmp(letters_row, "   A B C D E F G H\n", MAX_FILE_ROW_SIZE) != 0) {
        goto HANDLE_ERROR;
    }
    if (fgets(letters_row, 2, f) != NULL) {
        goto HANDLE_ERROR;
    }

    fclose(f);
    return true;

    HANDLE_ERROR:
    fclose(f);
    memcpy((void *) game, (void *) game_before_load, sizeof(struct game_t));
    free(game_before_load);
    return false;

}

bool does_file_exist(char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
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
