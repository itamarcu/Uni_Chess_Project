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
    MOVE,
    GET_MOVES,
    SAVE,
    UNDO,
    RESET,
    QUIT_GAME
} GAME_COMMAND;

typedef enum {
    GAME_MODE,
    DIFFICULTY,
    USER_COLOR,
    LOAD,
    DEFAULT,
    PRINT_SETTINGS,
    QUIT_SETTINGS,
    START
} SETTINGS_COMMAND;

//args gets maximum 4 numbers in move command - r1,c1,r2,c2 (c1 = 1 for Column A for example)
typedef struct command_type {
    bool isSettingsCommand;
    bool isValidCommand;
    bool isValidArgs;
    bool isValidPath;
    GAME_COMMAND gameCommand;
    SETTINGS_COMMAND settingsCommand;
    int args[4];
    char* path; // will be allocatated and point to a valid string path only if its load or save command. otherwise points to NULL.
} Command;

Command *get_user_input_as_command();

bool is_valid_path(char* path);

void free_command(Command *command)

#endif //UNI_CHESS_PROJECT_PARSER_H
