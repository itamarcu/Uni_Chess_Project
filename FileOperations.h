#ifndef UNI_CHESS_PROJECT_FILEOPERATIONS_H
#define UNI_CHESS_PROJECT_FILEOPERATIONS_H

#include <stdbool.h>
#include "ChessGameSettings.h"
#include "ChessBoard.h"

/**
 * Saves the game in a given path.
 * @return true if the game saved successfully, false otherwise.
 */
bool save_game_to_path(Game *game, char *path);

/**
 * Loads the game from a given path.
 * Assumes the file exists in path and contains a saved game
 * @return true if the game loaded successfully, false otherwise.
 */
bool load_game_from_path(Game *game, char *path);

/**
 * Returns true if the file exists in path.
 */
bool does_file_exist(char *path);

/**
 * Returns true if the file is empty.
 * Assumes the file exists in path
 */
bool is_file_empty(char *path);

#endif //UNI_CHESS_PROJECT_FILEOPERATIONS_H
