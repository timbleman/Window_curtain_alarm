/********************************** Includes **********************************/
#include "motor_controller.h"

#include <stdio.h>  // printf()


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
void make_step(int dir);
void enable_stepper();
void disable_stepper();


/**************************** Variable definitions ****************************/
#ifndef TESTABLE_MOTOR_CODE
int current_steps = 0;
int target_steps = 40;
#endif // TESTABLE_MOTOR_CODE


/**************************** Function definitions ****************************/
/*
 * Sets up variables of this file. Configures inputs and outputs.
 * 
 * @return: 0 if successful.
 */
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
        // TODO Here the passive break mode could be used instead of disabling.
        disable_stepper();
        return 0;
    }
    else
    {
        // TODO A check if already enabled could be employed.
        enable_stepper();
        make_step(0);
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
        return 1;
    }
}

/*
 * This function moves the curtain in the opposite position, I call it xor.
 * This function activates the motor. As it is nonblocking, it has to be 
 * called multiple times until the curtain is fully xored.
 * 
 * @return: 0 if not yet xored, 1 if fully xored.
 */
int curtain_xor()
{
    static enum CURTAIN_STATE state_when_started = CURTAIN_UNDEFINED_T;

    // Start the curtain XOR
    if (state_when_started == CURTAIN_UNDEFINED_T)
    {
        // When in doubt use OPENED. Use the end stop to abort when closing.
        if (get_curtain_state() == CURTAIN_CLOSED_T)
            state_when_started = CURTAIN_CLOSED_T;
        else // CURTAIN_OPENED_T   *or*   CURTAIN_UNDEFINED_T
            state_when_started = CURTAIN_OPEN_T;
    }

    // Open or close
    int status = 1;
    if (state_when_started == CURTAIN_OPEN_T)
        status = close_nonblocking();
    else
        status = open_nonblocking();

    // Reset the static variable
    if (status == 0)
        state_when_started = CURTAIN_UNDEFINED_T;

    return status;
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
    // Interrupt or polling? Polling should be fine for now...
    int end_stop = 0;
    if (end_stop)
    {
        current_steps = 0;
        return 0;
    }
    
    make_step(1);
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
        current_steps--;
        return;
    }
    else
    {
        // Open
        current_steps++;
        return;
    }
}

/*
 * Enables the stepper driver.
 * 
 * @return: None.
 */
void enable_stepper()
{
    // TODO check if already active.
    // Some GPIO stuff that disables the stepper driver.
    return;
}

/*
 * Reduces power draw or noise of the stepper during a standstill.
 * This could be acchieved by either completely disabling the driver
 * or by enabling the passive breakmode.
 * 
 * @retun: Mone.
 */
void disable_stepper()
{
    // Some GPIO stuff that disables the stepper driver.
    // Could also enable passive breakmode of the TMC2209.
    return;
}
