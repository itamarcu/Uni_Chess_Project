#include "ChessParser.h"


command_t *cmd_move(command_t *command) {
    // format should be: "move r1,c1 to r2,c2"
    // for example, "move 1,A to 8,H"
    command->game_command = CMD_MOVE;

    char *arg0 = strtok(NULL, " \r\t\n");
    char *arg1 = strtok(NULL, " \r\t\n");
    char *arg2 = strtok(NULL, " \r\t\n");
    if (arg2 == NULL || strcmp(arg1, "to") != 0
        || strlen(arg0) != 3 || strlen(arg2) != 3
        || arg0[1] != ',' || arg2[1] != ',') {
        command->valid_command = false;
        return command;
    }
    command->args[0] = arg0[0] - '1';
    command->args[1] = arg0[2] - 'A';
    command->args[2] = arg2[0] - '1';
    command->args[3] = arg2[2] - 'A';
    if (DEBUG_MODE) //While debugging, lowercase is allowed :)
    {
        if (arg0[2] >= 'a' && arg0[2] < 'z')
            command->args[1] += 'A' - 'a';
        if (arg2[2] >= 'a' && arg2[2] < 'z')
            command->args[3] += 'A' - 'a';
    }

    return command;
}

command_t *cmd_get_moves(command_t *command) {
    command->game_command = CMD_GET_MOVES;

    char *arg0 = strtok(NULL, " \r\t\n");
    if (arg0 == NULL || strlen(arg0) != 3 || arg0[1] != ',') {
        command->valid_command = false;
        return command;
    }

    command->args[0] = arg0[0] - '1';
    command->args[1] = arg0[2] - 'A';
    if (DEBUG_MODE) //While debugging, lowercase is allowed :)
    {
        if (arg0[2] >= 'a' && arg0[2] < 'z')
            command->args[1] += 'A' - 'a';
    }

    return command;
}

command_t *cmd_load(command_t *command) {
    command->settings_command = CMD_LOAD;
    char *arg_string = strtok(NULL, " \r\t\n");
    if (arg_string != NULL) {
        command->path = strdup(arg_string);
        command->valid_path = is_valid_path(command->path);
    } else command->valid_command = false;
    return command;
}

command_t *cmd_save(command_t *command) {
    command->game_command = CMD_SAVE;
    char *arg_string = strtok(NULL, " \r\t\n");
    if (arg_string != NULL) {
        command->path = strdup(arg_string);
        command->valid_path = is_valid_path(command->path);
    } else command->valid_command = false;
    return command;
}

command_t *get_user_input_as_command() {

    char input[1024 + 1];
    fgets(input, 1024 + 1, stdin);
    //remove trailing newline, possibly
    char *pos;
    if ((pos = strchr(input, '\n')) != NULL) {
        *pos = NULL_CHAR;
    }

    //getting the words from the string
    command_t *command = (command_t *) malloc(sizeof(*command));
    if (command == NULL) {
        println_error("ERROR: malloc resulted in NULL pointer!");
        return NULL;
    }
    command->path = NULL; //should be something
    char *command_string = strtok(input, " \r\t\n");
    if (command_string != NULL) {
        command_string = strdup(command_string);
    } else {
        command->valid_command = false;
        return command;
    }

    bool arg_valids[4];
    command->valid_command = true;

    //Special cases for these - need to parse non-integers
    if (strcmp(command_string, "move") == 0) {
        return cmd_move(command);
    } else if (strcmp(command_string, "get_moves") == 0) {
        return cmd_get_moves(command);
    } else if (strcmp(command_string, "load") == 0) {
        return cmd_load(command);
    } else if (strcmp(command_string, "save") == 0) {
        return cmd_save(command);
    } else {
        for (int i = 0; i < 4; i++) {
            command->args[i] = -666;
            arg_valids[i] = false;
            char *arg_string = strtok(NULL, " \r\t\n");
            if (arg_string != NULL) {
                int arg_integer;
                if (cast_str_to_int(arg_string, &arg_integer)) {
                    arg_valids[i] = true;
                    command->args[i] = arg_integer;
                }
            }
        }
    }

    command->settings_command = CMD_NONE_SETTINGS;
    command->game_command = CMD_NONE_GAME;

    if (strcmp(command_string, "game_mode") == 0) {
        command->settings_command = CMD_GAME_MODE;
        command->valid_command = arg_valids[0];
    } else if (strcmp(command_string, "difficulty") == 0) {
        command->settings_command = CMD_DIFFICULTY;
        command->valid_command = arg_valids[0];
    } else if (strcmp(command_string, "user_color") == 0) {
        command->settings_command = CMD_USER_COLOR;
        command->valid_command = arg_valids[0];
    } else if (strcmp(command_string, "default") == 0) {
        command->settings_command = CMD_DEFAULT;
    } else if (strcmp(command_string, "print_settings") == 0) {
        command->settings_command = CMD_PRINT_SETTINGS;
    } else if (strcmp(command_string, "start") == 0) {
        command->settings_command = CMD_START;
    } else if (strcmp(command_string, "quit") == 0) {
        command->settings_command = CMD_QUIT_SETTINGS;
        command->game_command = CMD_QUIT_GAME;
    } else if (strcmp(command_string, "undo") == 0) {
        command->game_command = CMD_UNDO;
    } else if (strcmp(command_string, "reset") == 0) {
        command->game_command = CMD_RESET;
    } else {
        command->valid_command = false;
    }

    return command;
}

bool is_valid_path(char *path) {
    //TODO is_valid_path()
}

void free_command(command_t *command) {
    free(command->path);
    free(command);
}