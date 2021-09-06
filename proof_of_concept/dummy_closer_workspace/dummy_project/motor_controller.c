#include "motor_controller.h"

/********************************** Includes **********************************/
#include <stdio.h>  // printf()


/********************************* Constants *********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions ****************************/
/**************************** Variable definitions ****************************/
int current_steps = 0;
int target_steps = 40;

/**************************** Function definitions ****************************/
int setup_motor_control()
{
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
    current_steps++;
    if (current_steps >= target_steps)
        return 1;
    else
        return 0;
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
    current_steps--;
    if (current_steps <= 0)
        return 1;
    else
        return 0;
}

/*
 * This function is mostly equivalent to close(), the main difference being
 * that the internal step target for fully opening is set.
 * 
 * @return: 0 if not yet closed, 1 if fully closed.
 */
int calibrate_nonblocking()
{
    printf("Closing and calibrating...\n");
    current_steps--;
    if (current_steps <= 0)
        return 1;
    else
        return 0;
}