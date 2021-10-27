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

// These should be private
void make_step(int dir);
void make_step_no_del(int close);
void enable_stepper();
void disable_stepper();

#ifdef __cplusplus
}
#endif

#endif // _MOTOR_CONTROLLER_H_