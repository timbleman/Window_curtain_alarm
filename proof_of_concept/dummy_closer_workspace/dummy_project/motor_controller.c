#include "motor_controller.h"

/********************************** Includes **********************************/
#include <stdio.h>  // printf()

/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
void make_step(int dir);


/**************************** Variable definitions ****************************/
#ifndef TESTABLE_MOTOR_CODE
int current_steps = 0;
int target_steps = 40;
#endif // TESTABLE_MOTOR_CODE


/**************************** Function definitions ****************************/
int setup_motor_control()
{
    current_steps = 0;
    target_steps = 40;
    return 0;
}

/*
 * This function activates the motor. As it is nonblocking, it has to be 
 * called multiple times until the curtain is fully closed.
 * 
 * @return: 0 if not yet closed, 1 if fully closed.
 */
int open_nonblocking()
{
    printf("Opening...\n");
    if (current_steps >= target_steps)
    {
        return 0;
    }
    else
    {
        make_step(0);
        current_steps++;
        return 1;
    }
}

/*
 * This function activates the motor. As it is nonblocking, it has to be 
 * called multiple times until the curtain is fully opened.
 * Uses an internal step target to determine fully opened.
 * 
 * @return: 0 if not yet opened, 1 if fully opened.
 */
int close_nonblocking()
{
    printf("Closing...\n");
    if (current_steps <= 0)
    {
        return 0;
    }
    else
    {
        make_step(1);
        current_steps--;
        return 1;
    }
}

/*
 * This function is mostly equivalent to close(), the main difference being
 * that the internal step target for fully opening is set.
 * TODO Make endstop work.
 * 
 * @return: 0 if not yet closed, 1 if fully closed.
 */
int calibrate_nonblocking()
{
    printf("Closing and calibrating...\n");
    
    // TODO Make this work
    int end_stop = 0;
    if (end_stop)
    {
        current_steps = 0;
        return 0;
    }
    
    make_step(1);
    current_steps--;
    return 1;
}

/*
 * Returns the current state of the curtain.
 * Can be open, closed or undefined.
 * TODO Maybe add some sort of range?
 * 
 * @return: enum CURTAIN_STATE open, closed or undefined.
 */
enum CURTAIN_STATE get_curtain_state()
{
    if (current_steps == target_steps)
        return CURTAIN_OPEN_T;
    else if (current_steps == 0)
        return CURTAIN_CLOSED_T;
    else
        return CURTAIN_UNDEFINED_T;
}

/*
 * Make a step. Controls the stepper driver.
 * In the first iteration this should be GPIO based.
 * Later on a UART based approach may be chosen.
 * 
 * @param dir: The direction. 1 closes, 0 opens.
 * @return: None.
 */
void make_step(int close)
{
    if (close)
    {
        // Close
        return;
    }
    else
    {
        // Open
        return;
    }
}