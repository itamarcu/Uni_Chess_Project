#include "ChessParser.h"

//TO-DO - fix according to new Command type with using checks that was coded in gameLogic and GameSettings.
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

    command->isValidCommand = false;
    command->isValidArgs = false;
    command->isValidPath = false;
    command->path = NULL;

    char *command_string = strtok(input, " \r\t\n");
    if (command_string != NULL) {
        command_string = strdup(command_string);
    } else return command;

    bool arg_valids[4];

    for (int i = 0; i < 4; i++) {
        command->args[i] = -666;
        arg_valids[i] = false;

        char *arg_string = strtok(NULL, " \r\t\n");
        if (arg_string != NULL) {
            if (i == 0) {
                arg_valids[i] = true;
                command->path = strdup(arg_string);
                command->isValidPath = is_valid_path(command->path);
            }

            int arg_integer;
            if (cast_str_to_int(arg_string, &arg_integer)) {
                arg_valids[i] = true;
                command->args[i] = arg_integer;
            }
        }

    }

    return command;


}

bool is_valid_path(char *path) {

}

void free_command(Command *command) {
    free(command->path);
    free(command);
}