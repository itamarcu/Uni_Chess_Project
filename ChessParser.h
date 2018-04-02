#ifndef UNI_CHESS_PROJECT_PARSER_H
#define UNI_CHESS_PROJECT_PARSER_H

#include "Auxiliary.h"
#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include <stdbool.h>
/**

 */
typedef enum {
    CMD_NONE_GAME,
    CMD_MOVE,
    CMD_GET_MOVES,
    CMD_SAVE,
    CMD_UNDO,
    CMD_RESET,
    CMD_QUIT_GAME
} GAME_COMMAND;

typedef enum {
    CMD_NONE_SETTINGS,
    CMD_GAME_MODE,
    CMD_DIFFICULTY,
    CMD_USER_COLOR,
    CMD_LOAD,
    CMD_DEFAULT,
    CMD_PRINT_SETTINGS,
    CMD_QUIT_SETTINGS,
    CMD_START
} SETTINGS_COMMAND;

//args gets maximum 4 numbers in move command - r1,c1,r2,c2 (c1 = 0 for Column A for example)
typedef struct command_t {
    bool valid_command;
    bool valid_path;
    GAME_COMMAND game_command;
    SETTINGS_COMMAND settings_command;
    int args[4];
    char *path; // will be allocated and point to a valid string path only if its load or save command. otherwise points to NULL.
} command_t;

command_t *get_user_input_as_command();

bool is_valid_path(char* path);

void free_command(command_t *command);

#endif //UNI_CHESS_PROJECT_PARSER_H
