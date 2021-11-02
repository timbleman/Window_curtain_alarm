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

#define BUTTON_DEBUG


/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
void reset_button_state();


/**************************** Variable definitions ****************************/
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
    pinMode(BUTTON1_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    reset_button_state();
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
    static unsigned long rising_edge_micros = 0;

    // TODO Verify if internal pullup is sufficient
    if (digitalRead(BUTTON1_PIN) == LOW)
    {
        if (rising_edge_micros == 0)
        {
            rising_edge_micros = micros();
        }
    }
    else
    {
        if (rising_edge_micros > 0)
        {
            long micros_since_rising = micros() - rising_edge_micros;
            if (micros_since_rising > LONG_PRESS_TIME)
            {
#ifdef BUTTON_DEBUG
                printf("Detected a long press!\n\r");
#endif // BUTTON_DEBUG
                button1_long_press = true;
            }
            else if (micros_since_rising > SHORT_PRESS_TIME)
            {
#ifdef BUTTON_DEBUG
                printf("Detected a short press!\n\r");
#endif // BUTTON_DEBUG
                button1_short_press = true;
            }
            // else: do nothing
            rising_edge_micros = 0;
        }
    }
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
 * TODO Decide what to do at a long or short press.
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
        reset_button_state();
    }
    else if (button1_long_press)
    {
        new_act.act_type = IGNORE_ONCE_T;
        reset_button_state();
    }
    else
    {
        new_act.act_type = NONE_T;
    }

#ifdef BUTTON_DEBUG
    printf("Fetched button action!\n\r");
#endif // BUTTON_DEBUG

    return new_act;
}

/*
 * Resets the button activation states. Has to be called when fetching an 
 * action. Function to be easily expandable.
 * 
 * @return: None.
 */
void reset_button_state()
{
    button1_short_press = false;
    button1_long_press = false;
}
