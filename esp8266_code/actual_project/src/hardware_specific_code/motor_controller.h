/*
 * The motor controller is responsible for openening and closing the curtain.
 * The implementation is highly hardware specific and needs to be adjusted.
 */

#ifndef _MOTOR_CONTROLLER_H_
#define _MOTOR_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Includes **********************************/
/********************************* Constants **********************************/
// Debugging prints to activate/deactivate
#define MOTOR_PRINTS
#undef PRINTS_AT_EACH_STEP

/*
 * TODO Configure these.
 * These are the number of steps in the opposite direction after activating
 * the end stop.
 */
#define ROLLBACK_STEPS 100

enum CURTAIN_STATE
{
    CURTAIN_OPEN_T = 0x0001,
    CURTAIN_CLOSED_T = 0x0002,
    CURTAIN_UNDEFINED_T = 0x0004
};


/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
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
 * Rolls back if the end stop has been reached sooner than expected.
 * 
 * @return: 0 if not yet opened, 1 if fully opened.
 */
int close_nonblocking();

/*
 * This function moves the curtain in the opposite position, I call it xor.
 * This function activates the motor. As it is nonblocking, it has to be 
 * called multiple times until the curtain is fully xored.
 * 
 * @return: 0 if not yet xored, 1 if fully xored.
 */
int curtain_xor();

/*
 * This function is mostly equivalent to close(), the main difference being
 * that the internal step target for fully opening is set.
 * Rolls back a few steps after the endstop has been activated.
 * 
 * TODO This has to be tried and debugged.
 * 
 * @return: 0 if not yet closed, 1 if fully closed.
 */
int calibrate_nonblocking_rollback();

/*
 * Returns the current state of the curtain.
 * Can be open, closed or undefined.
 * 
 * @return: enum CURTAIN_STATE open, closed or undefined.
 */
enum CURTAIN_STATE get_curtain_state();

// These should be private
//void make_step(int dir);
//void enable_stepper();
//void disable_stepper();

#ifdef __cplusplus
}
#endif

#endif // _MOTOR_CONTROLLER_H_