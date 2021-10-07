#ifndef _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Includes **********************************/
#include <stdint.h>
#include "configuration.h"
#include "types_and_enums.h"

/********************************* Constants *********************************/
#define MAX_USER_INPUT_LEN 64
#define CONVERT_USER_IN_TO_LOWER

#define BASIC_DEBUG_PRINTS
#undef PARSE_TIME_DEBUG_PRINTS
#define PARSE_ACTION_DEBUG_PRINTS

/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/

/*
 * This function returns a user_action_t. 
 * Pass by value with copying is used, should not be a problem for this small
 * size.
 * 
 * @param str: String to parse the action from.
 * @return: A user_action_t.
 */
user_action_t get_action(char *str);

/*
 * This function writes an error message in a predefined buffer.
 * Error codes have to be passed (one-hot, those can be XORed together).
 * 
 * @param errs: (Multiple) error code(s).
 * @param buf: Preallocated string buffer to write message into.
 * @param str_max_len: The maximum length of the string.
 * @return: 0 if successful.
 */
int get_message_from_errors(enum TIME_ERRORS errs, char *buf, int str_max_len);


#ifdef TESTABLE_PARSER_CODE
enum ACTION_TYPE parse_action(char **split_command, int command_num);
int parse_day(char **split_command, int command_num);
int parse_hour(char **split_command, int command_num);
int parse_min(char **split_command, int command_num);
int parse_sec(char **split_command, int command_num);
int tokenise_to_argc_argv(
        char *buffer, int *argc, char *argv[], const int argv_length);
#endif // TESTABLE_PARSER_CODE

#ifdef __cplusplus
}
#endif

#endif // _COMMAND_PARSER_H_