/********************************** Includes **********************************/
#include "motor_controller.h"

/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/


/**************************** Variable definitions ****************************/
#ifndef TESTABLE_MOTOR_CODE
int current_steps = 0;
int target_steps = 40;
#endif // TESTABLE_MOTOR_CODE


/**************************** Function definitions ****************************/
/*
 * Notes concerning the ESP8266:
 * ESP8266 console used UART0 by default (RX and TX pins).
 * Other UARTs: RXD2 (D7, GPIO16), TXD2 (D8, GPIO17), RXD1 (SD1, SD_DATA_1), 
 * TXD1 (D4, GPIO2)
 * 
 * Pins that are most likely not relevant for the project:
 * D1 GPIO5 (SCL), D2 GPIO4 (SDA), D5 GPIO14 (SCLK), D6 GPIO12 (MISO)
 */

/*
 * Notes concerning the TMC2209:
 *
 */

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
