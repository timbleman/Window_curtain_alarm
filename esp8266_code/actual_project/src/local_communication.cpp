/********************************** Includes **********************************/
#include <Arduino.h>
#include "local_communication.h"


/********************************* Constants **********************************/
// TODO Find a fitting pin
#define BUTTON1_PIN D3
#define LED_PIN D0

// Presstimes in microseconds, can be adjusted
#define SHORT_PRESS_TIME 50000
#define LONG_PRESS_TIME 700000


/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/**************************** Variable definitions ****************************/
static bool input_available = false;
static bool button1_short_press = false;
static bool button1_long_press = false;


/**************************** Function definitions ****************************/
/* 
 * Setup local inputs and display.
 * Will consist of buttons and an LED at first.
 * 
 * @return: None.
 */
void setup_local_comm()
{
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

/* 
 * Update and check for local inputs.
 * Has to be called repeatedly.
 * Currently using polling. Could be rewritten for interrupts.
 * Returns success if an input is available to be fetched.
 * 
 * @return: 0 if input is available, 1 if not.
 */
int get_local_input()
{
    /* 
     * static rising_edge_micros;
     * if input
     *  if rising_edge_micros == 0
     *      rising_edge_micros = micros()
     *  else 
     *      nothing
     * else
     *  if rising_edge_micros > 0
     *      if micros() - rising_edge_micros > LONG_PRESS_TIME
     *          button1_long_press = true;
     *      else if micros() - rising_edge_micros > SHORT_PRESS_TIME
     *          button1_short_press = true;
     *      else
     *          nothing
     *      rising_edge_micros = 0;
     *  else
     *      nothing
     */

    // TODO Check for micros overflow?

    // Check if an input is available
    if (button1_long_press || button1_short_press)
        return 0;
    else
        return 1;
}

/* 
 * Fetch an action if available. 
 * Pass by value with copying is used, should not be a problem for this small
 * size.
 * 
 * @return: A user_action_t.
 */
user_action_t fetch_local_action()
{
    user_action_t new_act = {};

    if (button1_short_press)
    {
        new_act.act_type = CURTAIN_CONTROL_T;
        new_act.data[0] = CURTAIN_XOR_T;
    }
    else if (button1_long_press)
    {
        new_act.act_type = CURTAIN_CONTROL_T;
        new_act.data[0] = CALIBRATE_T;
    }
    else
    {
        new_act.act_type = NONE_T;
    }

    return new_act;
}
