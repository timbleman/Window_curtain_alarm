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
/*
 * Execute an user_action_t entity.
 * Nonblocking: Return 0 if finished, 1 otherwise.
 * If finished, a message is copied into the supplied location.
 * 
 * @param act: user_action_t to execute.
 * @param message: String location to copy into.
 * @param message_max_len: Max string length.
 * @return: 0 if finished, 1 if still working.
 */
int execute_action_non_blocking(user_action_t *act,
                                char *message,
                                int message_max_len);

#ifdef __cplusplus
}
#endif

#endif // _ACTION_EXECUTER_H_