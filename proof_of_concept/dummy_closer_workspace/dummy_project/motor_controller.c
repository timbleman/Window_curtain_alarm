#include "motor_controller.h"

/********************************** Includes **********************************/
#include <stdio.h>  // printf()

/********************************* Constants *********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions ****************************/
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
    
    current_steps--;
    return 1;
}