#ifndef _WEB_COMMUNICATION_H_
#define _WEB_COMMUNICATION_H_

/********************************** Includes **********************************/
#include <stdint.h>
#include "../../../configuration.h"
#include "../../../portable_code/command_parser.h"

#ifdef ENABLE_LOCAL_WEB_SITE

/********************************* Constants *********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
/* 
 * Setup an input_handler_t. Better way to handle multiple input sources.
 * 
 * @param inh: An input_handler_t to initialize.
 * @retval: Success status.
 */
int setup_web_input_handler_t(input_handler_t *inh);

#endif // ENABLE_LOCAL_WEB_SITE

#endif // _WEB_COMMUNICATION_H_