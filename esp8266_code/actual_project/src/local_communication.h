#ifndef _USER_COMM_H_
#define _USER_COMM_H_

/********************************** Includes **********************************/
#include "types_and_enums.h"


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
/* 
 * Setup local inputs and display.
 * Will consist of buttons and an LED at first.
 * 
 * @return: None.
 */
void setup_local_comm();

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
