#ifndef _MOTOR_CONTROLLER_H_
#define _MOTOR_CONTROLLER_H_

/********************************** Includes **********************************/
#include <stdint.h>
#include "configuration.h"

/********************************* Constants **********************************/
enum CURTAIN_STATE
{
    CURTAIN_OPEN_T = 0x0001,
    CURTAIN_CLOSED_T = 0x0002,
    CURTAIN_UNDEFINED_T = 0x0004
};


/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
#ifdef TESTABLE_MOTOR_CODE
// For test case compilation these have to be initialized first.
int current_steps;
int target_steps;
#endif // TESTABLE_MOTOR_CODE


/**************************** Prototype functions *****************************/
/*
 * Sets up variables of this file. Configures inputs and outputs.
 * 
 * @return: 0 if successful.
 */
int setup_motor_control();

/*
 * This function activates the motor. As it is nonblocking, it has to be 
 * called multiple times until the curtain is fully closed.
 * 
 * @return: 0 if not yet closed, 1 if fully closed.
 */
int open_nonblocking();

/*
 * This function activates the motor. As it is nonblocking, it has to be 
 * called multiple times until the curtain is fully opened.
 * Uses an internal step target to determine fully opened.
 * 
 * @return: 0 if not yet opened, 1 if fully opened.
 */
int close_nonblocking();

/*
 * This function is mostly equivalent to close(), the main difference being
 * that the internal step target for fully opening is set.
 * 
 * @return: 0 if not yet closed, 1 if fully closed.
 */
int calibrate_nonblocking();

/*
 * Returns the current state of the curtain.
 * Can be open, closed or undefined.
 * 
 * @return: enum CURTAIN_STATE open, closed or undefined.
 */
enum CURTAIN_STATE get_curtain_state();

#endif // _MOTOR_CONTROLLER_H_