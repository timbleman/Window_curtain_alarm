#ifndef _MOTOR_CONTROLLER_H_
#define _MOTOR_CONTROLLER_H_

/********************************** Includes **********************************/
/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions ****************************/
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

int calibrate_nonblocking_rollback();

// These should be private
void make_step(int dir);
void make_step_no_del(int close);
void enable_stepper();
void disable_stepper();

#endif // _MOTOR_CONTROLLER_H_