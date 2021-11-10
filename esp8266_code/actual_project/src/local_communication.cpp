/********************************** Includes **********************************/
#include <Arduino.h>
#include "local_communication.h"


/********************************* Constants **********************************/
// TODO Find a fitting pin
#define BUTTON1_PIN D3
#define LED_PIN D0

// Presstimes in milliseconds, can be adjusted
#define SHORT_PRESS_TIME 50
#define LONG_PRESS_TIME 700

#define BUTTON_DEBUG

// Times for visualization using the LED
#define LONG_HOLD_MILLIS 1000
#define SHORT_HOLD_MILLIS 300


/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
void reset_button_state();
int display_short_press();
int display_long_press();


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
    static unsigned long rising_edge_millis = 0;
    static bool display_finished = false;

    // TODO Verify if internal pullup is sufficient
    if (digitalRead(BUTTON1_PIN) == LOW)
    {
        if (rising_edge_millis == 0)
        {
            rising_edge_millis = millis();
            display_finished = false;
        }
    }
    else
    {
        if (rising_edge_millis > 0)
        {
            long millis_since_rising = millis() - rising_edge_millis;
            if (millis_since_rising > LONG_PRESS_TIME)
            {
#ifdef BUTTON_DEBUG
                printf("Detected a long press!\n\r");
#endif // BUTTON_DEBUG
                button1_long_press = true;
            }
            else if (millis_since_rising > SHORT_PRESS_TIME)
            {
#ifdef BUTTON_DEBUG
                printf("Detected a short press!\n\r");
#endif // BUTTON_DEBUG
                button1_short_press = true;
            }
            // else: do nothing
            rising_edge_millis = 0;
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

    // Display the press stats, negate the success status.
    if (button1_short_press && !display_finished)
    {
        display_finished = !(bool)display_short_press();
    }
    if (button1_long_press && !display_finished)
    {
        display_finished = !(bool)display_long_press();
    }

    // TODO Check for millis() overflow?

    // Check if an input is available, only return success if display finished.
    if ((button1_long_press || button1_short_press) && display_finished)
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


/*
 * Visualize, that a short press has been detected.
 * TODO This can be pressed into a loop when using bit math:
 * TODO Should there be a check for millis() overflow?
 * 
 * @return: Success status.
 */ 
int display_short_press()
{
    static unsigned long first_millis = 0;
    // LED blink state
    static int state = 0;
    int status = 1;

    if (first_millis == 0)
    {
        first_millis = millis();
        digitalWrite(LED_PIN, HIGH);
        state = 1;
    }

    if (state == 1 && (millis() > (first_millis + SHORT_HOLD_MILLIS)))
    {
        digitalWrite(LED_PIN, LOW);
        state = 2;
    }

    if (state == 2 && (millis() > (first_millis + 2 * SHORT_HOLD_MILLIS)))
    {
        digitalWrite(LED_PIN, HIGH);
        state = 3;
    }  

    if (state == 3 && (millis() > (first_millis + 3 * SHORT_HOLD_MILLIS)))
    {
        digitalWrite(LED_PIN, LOW);
        state = 0;
        first_millis = 0;
        // Set success status after blinking two times
        status = 0;
    }  

    return status;
}

/*
 * Visualize, that a long press has been detected.
 * TODO Should there be a check for millis() overflow?
 * 
 * @return: Success status.
 */
int display_long_press()
{
    static unsigned long first_millis = 0;
    int status = 1;

    if (first_millis == 0)
    {
        first_millis = millis();
        digitalWrite(LED_PIN, HIGH);
        status = 1;
    }

    if (millis() > first_millis + LONG_HOLD_MILLIS)
    {
        first_millis = 0;
        digitalWrite(LED_PIN, LOW);
        status = 0;
    }
    
    return status;
}
