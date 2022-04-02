/* 
 * Local communication using a button and an LED.
 */

#ifndef _USER_COMM_H_
#define _USER_COMM_H_

/********************************** Includes **********************************/
#include "../portable_code/types_and_enums.h"


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
/* 
 * Setup an input_handler_t. Better way to handle multiple input sources.
 * 
 * @param inh: An input_handler_t to initialize.
 * @retval: Success status.
 */
int setup_local_input_handler_t(input_handler_t *inh);

/* 
 * Setup local inputs and display.
 * Will consist of buttons and an LED at first.
 * 
 * @return: Success status.
 */
int setup_local_comm();

/* 
 * Update and check for local inputs.
 * Has to be called repeatedly.
 * Returns success if an input is available to be fetched.
 * 
 * @return: 0 if input is available, 1 if not.
 */
int get_local_input();

/* 
 * Fetch an action if available. 
 * Pass by value with copying is used, should not be a problem for this small
 * size.
 * 
 * @return: A user_action_t.
 */
user_action_t fetch_local_action();

#endif // _USER_COMM_H_
