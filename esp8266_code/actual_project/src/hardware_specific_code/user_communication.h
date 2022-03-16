/* 
 * Receive user input strings.
 * Respond to a user.
 */

#ifndef _USER_COMMUNICATION_H_
#define _USER_COMMUNICATION_H_

/********************************** Includes **********************************/
/********************************* Constants **********************************/
#define RECV_DEBUG


/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
/*
 * Setup the user communication.
 * 
 * @return: Success status.
 */
int setup_user_comm();

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