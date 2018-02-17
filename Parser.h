#ifndef UNI_CHESS_PROJECT_PARSER_H
#define UNI_CHESS_PROJECT_PARSER_H

#include "Structs.h"

/**
 * Read user input in the form of "string arg1 arg2 arg3...".
 * Returns NULL if invalid input
 */
Command *get_user_input_as_command();

#endif //UNI_CHESS_PROJECT_PARSER_H
