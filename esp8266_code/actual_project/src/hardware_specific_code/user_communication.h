/* 
 * Receive user input strings.
 * Respond to a user.
 */

#ifndef _USER_COMMUNICATION_H_
#define _USER_COMMUNICATION_H_

/********************************** Includes **********************************/
#include "../portable_code/types_and_enums.h"


/********************************* Constants **********************************/
#define RECV_DEBUG


/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
/* 
 * Setup an input_handler_t. Better way to handle multiple input sources.
 * 
 * @param inh: An input_handler_t to initialize.
 * @retval: Success status.
 */
int setup_user_input_handler_t(input_handler_t *inh);

/*
 * Setup the user communication.
 * 
 * @return: Success status.
 */
int setup_user_comm();

/* 
 * Update and check for user inputs.
 * Has to be called repeatedly.
 * Returns success if an input is available to be fetched.
 * 
 * @return: 0 if input is available, 1 if not.
 */
int get_user_in1();

/* 
 * Fetch an action if available. 
 * Pass by value with copying is used, should not be a problem for this small
 * size.
 * 
 * @return: A user_action_t.
 */
user_action_t fetch_user_action();

/*
 * Check for user input.
 * Stores the input in the buffer and returns 0 if successful.
 * 
 * @param buf: Buffer to store the input into.
 * @param buf_max_len: The size of the buffer.
 * @return: 0 if success and new input, else 1.
 */
int get_user_in(char *buf, int buf_max_len);

/* 
 * Send text back to the user. 
 * Does not send if the user is no longer connected.
 *
 * @param buf: The string to send back.
 * @param buf_max_len: The maximum number of chars to send.
 * @return: Success status.
 */
int respond_to_user(char *buf, int buf_max_len);

#endif // _USER_COMMUNICATION_H_