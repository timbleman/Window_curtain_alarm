#ifndef _WEB_COMMUNICATION_H_
#define _WEB_COMMUNICATION_H_

/********************************** Includes **********************************/
#include <stdint.h>
#include "../../portable_code/command_parser.h"


/********************************* Constants *********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
/* 
 * Setup the web server.
 * 
 * return:  Success status.
 */
int setup_web_comm();

/* 
 * Update and check for web inputs.
 * Has to be called repeatedly.
 * Returns success if an input is available to be fetched.
 * 
 * @return: 0 if input is available, 1 if not.
 */
int get_web_in();

/* 
 * Fetch an action if available. 
 * Pass by value with copying is used, should not be a problem for this small
 * size.
 * 
 * @return: A user_action_t.
 */
user_action_t fetch_web_action();

/*
 * Check for web input.
 * Stores the input in the buffer and returns 0 if successful.
 * 
 * @param buf: Buffer to store the input into.
 * @param buf_max_len: The size of the buffer.
 * @return: 0 if success and new input, else 1.
 */
int get_web_in(char *buf, int buf_max_len);

/* 
 * Respond to all connected clients.
 * 
 * @param buf:          Chars to send.
 * @param buf_max_len:  Buffer size.
 * @return:             Success status.
 */
int respond_to_web_user(char *buf, int buf_max_len);

#endif // _WEB_COMMUNICATION_H_