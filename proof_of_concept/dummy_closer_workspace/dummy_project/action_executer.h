#ifndef _ACTION_EXECUTER_H_
#define _ACTION_EXECUTER_H_

/********************************** Includes **********************************/
#include <stdint.h>
#include "command_parser.h"
#include "configuration.h"

/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
int execute_action_non_blocking(user_action_t *act,
                                char *message,
                                int message_max_len);

#endif // _ACTION_EXECUTER_H_