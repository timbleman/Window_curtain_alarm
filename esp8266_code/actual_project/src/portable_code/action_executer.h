/*
 * The action executer runs a user_action_t. It interacts with the time keeper
 * and the motor controller.
 */

#ifndef _ACTION_EXECUTER_H_
#define _ACTION_EXECUTER_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Includes **********************************/
#include <stdint.h>
#include "types_and_enums.h"
#include "configuration.h"


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
int execute_action_non_blocking(user_action_t *act,
                                char *message,
                                int message_max_len);

#ifdef __cplusplus
}
#endif

#endif // _ACTION_EXECUTER_H_