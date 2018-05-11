#include "ChessParser.h"
#include "FileOperations.h"

char *strdup_2(char *src) {
    char *ret = malloc(strlen(src) + 1);

    if (ret != 0) {
        strcpy(ret, src);
    }

    return ret;
}

Command *cmd_move(Command *command) {
    // format should be: "move <r1,c1> to <r2,c2>"
    // for example, "move <1,A> to <8,H>"
    command->game_command = CMD_MOVE;

    char *arg0 = strtok(NULL, " \r\t\n");
    char *arg1 = strtok(NULL, " \r\t\n");
    char *arg2 = strtok(NULL, " \r\t\n");
    if (arg2 == NULL || strcmp(arg1, "to") != 0
        || strlen(arg0) != 5 || strlen(arg2) != 5
        || arg0[0] != '<' || arg2[0] != '<'
        || arg0[2] != ',' || arg2[2] != ','
        || arg0[4] != '>' || arg2[4] != '>') {
        command->valid_command = false;
        return command;
    }
    command->args[0] = arg0[1] - '1';
    command->args[1] = arg0[3] - 'A';
    command->args[2] = arg2[1] - '1';
    command->args[3] = arg2[3] - 'A';

    return command;
}

/**
 * For quick debugging and less annoying syntax:
 *
 * "mov d2 d4" is equivalent to "move <2,D> to <4,D>"
 */
Command *cmd_mov(Command *command) {
    command->game_command = CMD_MOVE;

    char *arg0 = strtok(NULL, " \r\t\n");
    char *arg1 = strtok(NULL, " \r\t\n");
    if (arg1 == NULL
        || strlen(arg0) != 2 || strlen(arg1) != 2) {
        command->valid_command = false;
        return command;
    }
    command->args[0] = arg0[1] - '1';
    command->args[1] = arg0[0] - 'A';
    command->args[2] = arg1[1] - '1';
    command->args[3] = arg1[0] - 'A';
    if (arg0[0] >= 'a' && arg0[0] < 'z')
        command->args[1] += 'A' - 'a';
    if (arg1[0] >= 'a' && arg1[0] < 'z')
        command->args[3] += 'A' - 'a';

    return command;
}

Command *cmd_get_moves(Command *command) {
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

Command *cmd_auto(Command *command) {
    command->game_command = CMD_MAKE_MY_MOVE;
    return command;
}

Command *cmd_load(Command *command) {
    command->settings_command = CMD_LOAD;
    char *arg_string = strtok(NULL, " \r\t\n");
    if (arg_string != NULL) {
        command->path = strdup_2(arg_string);
        command->valid_path = does_file_exist(command->path);
    } else command->valid_command = false;
    return command;
}

Command *cmd_save(Command *command) {
    command->game_command = CMD_SAVE;
    char *arg_string = strtok(NULL, " \r\t\n");
    if (arg_string != NULL) {
        command->path = strdup_2(arg_string);
        command->valid_path = does_file_exist(command->path);
    } else command->valid_command = false;
    return command;
}

Command *get_user_input_as_command() {

    char input[1024 + 1];
    fgets(input, 1024 + 1, stdin);
    //remove trailing newline, possibly
    char *pos;
    if ((pos = strchr(input, '\n')) != NULL) {
        *pos = NULL_CHAR;
    }

    //getting the words from the string
    Command *command = (Command *) malloc(sizeof(*command));
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

    command->settings_command = CMD_NONE_SETTINGS;
    command->game_command = CMD_NONE_GAME;

    //Special cases for these - need to parse non-integers
    if (strcmp(command_string, "move") == 0) {
        return cmd_move(command);
    } else if (strcmp(command_string, "mov") == 0 && DEBUG_MODE) {
        return cmd_mov(command);
    } else if (strcmp(command_string, "auto") == 0) { // special custom command
        return cmd_auto(command);
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

void free_command(Command *command) {
    free(command->path);
    free(command);
}