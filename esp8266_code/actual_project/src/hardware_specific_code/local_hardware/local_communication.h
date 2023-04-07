/* 
 * Local communication using a button and an LED.
 */

#ifndef _USER_COMM_H_
#define _USER_COMM_H_

/********************************** Includes **********************************/
#include "../../portable_code/types_and_enums.h"


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

#endif // _USER_COMM_H_
