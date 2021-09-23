#ifndef _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_

/********************************** Includes **********************************/
#include <stdint.h>
#include "configuration.h"

/********************************* Constants *********************************/
#define MAX_USER_INPUT_LEN 64
#define CONVERT_USER_IN_TO_LOWER

#define BASIC_DEBUG_PRINTS
#undef PARSE_TIME_DEBUG_PRINTS
#undef PARSE_ACTION_DEBUG_PRINTS

enum PARSER_ERRORS
{
    INVALID_DAY_ERR = 0x1000,
    INVALID_TIME_ERR = 0x2000,
    INVALID_COMMAND_ERR = 0x4000,
    ALL_ERRS = (INVALID_DAY_ERR | INVALID_TIME_ERR | INVALID_COMMAND_ERR)
};

enum ACTION_TYPE
{
    NONE_T = 0x0000,
    WAKE_SET_T = 0x0001,
    SLEEP_SET_T = 0x0002,
    CURTAIN_CONTROL_T = 0x0004,
    ERROR_T = 0x0008
};

// These should be able to be combined
enum DAY_TYPE
{
    MON_T = 0x0001,
    TUE_T = 0x0002,
    WED_T = 0x0004,
    THU_T = 0x0008,
    FRI_T = 0x0010,
    SAT_T = 0x0020,
    SUN_T = 0x0040
};

enum CURTAIN_CONTROL_ACT_T
{
    OPEN_T = 1,
    CLOSE_T = 2,
    CALIBRATE_T = 4,
    CURTAIN_ERROR_T = 8
};

/***************************** Struct definitions *****************************/
typedef struct 
{
    enum ACTION_TYPE act_type;
    uint32_t data[5];
} user_action_t;

/**************************** Variable definitions ****************************/
/**************************** Prototype functions ****************************/

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
 * @param str: Preallocated string buffer to write message into.
 * @param str_max_len: The maximum length of the string.
 * @return: None.
 */
void get_message_from_errors(enum PARSER_ERRORS errs, char *str, int str_max_len);


#ifdef TESTABLE_PARSER_CODE
enum ACTION_TYPE parse_action(char **split_command, int command_num);
int parse_day(char **split_command, int command_num);
int parse_hour(char **split_command, int command_num);
int parse_min(char **split_command, int command_num);
int parse_sec(char **split_command, int command_num);
int tokenise_to_argc_argv(
        char *buffer, int *argc, char *argv[], const int argv_length);
#endif // TESTABLE_PARSER_CODE

#endif // _COMMAND_PARSER_H_