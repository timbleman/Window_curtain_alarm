/********************************** Includes **********************************/
#include <Arduino.h>
#include "motor_controller.h"

/********************************* Constants **********************************/
#define STEPPER_ENABLE_PIN D1
#define STEPPER_DIR_PIN D2
#define PERFORM_STEP_PIN D5
#define END_STOP_PIN D6

#define CHECK_MICROS_OVERFLOW

// Time in ms between steps. Controls the motor speed.
// TODO In mm/s
#define FAST_STEP_TIME 400
#define SLOW_STEP_TIME 800


/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
void make_step(int dir);
void make_step_delay(int dir);
int make_step_no_del(int close, int deltime);
void enable_stepper();
void disable_stepper();
int rollback();


/**************************** Variable definitions ****************************/
#ifndef TESTABLE_MOTOR_CODE
int current_steps = 0;
int target_steps = 40;
#endif // TESTABLE_MOTOR_CODE
bool calibrated = false;
bool stepper_enabled = false;


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
 * Passive breakmode can be enabled using UART.
 * Default current is about 0.5A. This is enough at 5v for curtain operation.
 */

/*
 * Sets up variables of this file. Configures inputs and outputs.
 * 
 * @return: 0 if successful.
 */
int setup_motor_control()
{
    // Setup variables
    current_steps = 0;
    target_steps = 40;

    // Setup pins
    pinMode(STEPPER_ENABLE_PIN, OUTPUT);
    pinMode(STEPPER_DIR_PIN, OUTPUT);
    pinMode(PERFORM_STEP_PIN, OUTPUT);
    // Use an external pulldown and cap for debouncing
    pinMode(END_STOP_PIN, INPUT);

    disable_stepper();

    calibrated = false;

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
#ifdef PRINTS_AT_EACH_STEP
    printf("Opening...\n");
#endif // PRINTS_AT_EACH_STEP
    if (!calibrated)
    {
#ifdef MOTOR_PRINTS
        printf("Cannot open when the system has not been calibrated!\n\r");
#endif // MOTOR_PRINTS
        return 0;
    }

    if (current_steps >= target_steps)
    {
        // TODO Here the passive break mode could be used instead of disabling.
        disable_stepper();
        return 0;
    }
    else
    {
        // TODO A check if already enabled could be employed.
        if (!stepper_enabled)
            enable_stepper();
        make_step(0);
        return 1;
    }
}

/*
 * This function activates the motor. As it is nonblocking, it has to be 
 * called multiple times until the curtain is fully opened.
 * Uses an internal step target to determine fully opened.
 * Rolls back if the end stop has been reached sooner than expected.
 * 
 * @return: 0 if not yet opened, 1 if fully opened.
 */
int close_nonblocking()
{
    static bool rollback_necessary = false;
#ifdef PRINTS_AT_EACH_STEP
    printf("Closing...\n");
#endif // PRINTS_AT_EACH_STEP
    if (!calibrated)
    {
#ifdef MOTOR_PRINTS
        printf("Cannot close when the system has not been calibrated!\n\r");
#endif // MOTOR_PRINTS
        return 0;
    }

    // Roll back the motor if the endstop is reached sooner than expected.
    int end_stop = digitalRead(END_STOP_PIN);
    if (end_stop == LOW)
    {
        rollback_necessary = true;
    }

    int motor_status = 1;
    if (rollback_necessary)
    {
        motor_status = rollback();
        if (motor_status == 0)
        {
            rollback_necessary = false;
            disable_stepper();
            // TODO Does this make sense?
            current_steps = 0;
        }
    }
    else if ((current_steps <= 0))
    {
        disable_stepper();
        motor_status = 0;
    }
    else
    {
        if (!stepper_enabled)
            enable_stepper();
        make_step(1);
        motor_status = 1;
    }

    return motor_status;
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
 * Rolls back a few steps after the endstop has been activated.
 * 
 * @return: 0 if not yet closed, 1 if fully closed.
 */
int calibrate_nonblocking_rollback()
{
#ifdef PRINTS_AT_EACH_STEP
    printf("Closing and calibrating...\n");
#endif // PRINTS_AT_EACH_STEP
    static bool started_calibrating = false;
    static bool end_stop_triggered = false;
    static int counted_steps = 0;

    if (!started_calibrating)
    {
        counted_steps = 0;
        end_stop_triggered = false;
        started_calibrating = true;

        if (!stepper_enabled)
            enable_stepper();
    }
    
    // Interrupt or polling? Polling should be fine for now...
    int end_stop = digitalRead(END_STOP_PIN);
    if (end_stop == LOW)
    {
        //current_steps = 0;
        end_stop_triggered = true;
    }
    /*
     * Roll back a bit to avoid constant end stop activation.
     * Make sure to not roll back too far.
     */
    if (end_stop_triggered)
    {
        int rollback_target = counted_steps > ROLLBACK_STEPS ? ROLLBACK_STEPS : 0;
        int rollback_status = rollback();
        if (rollback_status == 0 || rollback_target == 0)
        {
            // Reset the static variables
            current_steps = 0;
            end_stop_triggered = false;
            started_calibrating = false;
            calibrated = true;
            // Set the target steps
            target_steps = counted_steps - rollback_target;
#ifdef MOTOR_PRINTS
            printf("Set target steps to %i\n", target_steps);
#endif // MOTOR_PRINTS
            disable_stepper();
            return 0;
        }
    }
    else
    {
        counted_steps += make_step_no_del(1, SLOW_STEP_TIME);
    }
    
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

int rollback()
{
    static int rolled_back_steps = 0;

    // Ensure that you don't roll back further than the maximum steps
    //int rollback_target = target_steps > ROLLBACK_STEPS ? ROLLBACK_STEPS : 0;
    if (rolled_back_steps >= ROLLBACK_STEPS)
    {
            // Reset the static variables
            rolled_back_steps = 0;
            return 0;
    }

    // Make a step in the opposing direction.
    rolled_back_steps += make_step_no_del(0, SLOW_STEP_TIME);
    return 1;
}

void make_step(int close)
{
    make_step_no_del(close, FAST_STEP_TIME);
}

/*
 * Make a step. Controls the stepper driver.
 * In the first iteration this should be GPIO based.
 * Later on a UART based approach may be chosen.
 * This function uses a delay to pull the step pin low. This delay is not 
 * optimal, but as it is at 2 ms or lower it should not be noticeable.
 * 
 * @param dir: The direction. 1 closes, 0 opens.
 * @return: None.
 */
void make_step_delay(int close)
{
    const int DEL_TIME = 1;

    if (close)
    {
        // Close
        // TODO Maybe check if already set
        digitalWrite(STEPPER_DIR_PIN, LOW);
        // TODO Delay is not optimal, but may be short enough to not be noticeable
        digitalWrite(PERFORM_STEP_PIN, LOW);
        // TODO Try 1, 2, 5
        delay(DEL_TIME);
        digitalWrite(PERFORM_STEP_PIN, HIGH);
        current_steps--;
        return;
    }
    else
    {
        // Open
        digitalWrite(STEPPER_DIR_PIN, HIGH);
        digitalWrite(PERFORM_STEP_PIN, LOW);
        delay(DEL_TIME);
        digitalWrite(PERFORM_STEP_PIN, HIGH);
        current_steps++;
        return;
    }
}

/*
 * Make a step. Controls the stepper driver.
 * In the first iteration this should be GPIO based.
 * Later on a UART based approach may be chosen.
 * No delay is used. This means the function has to be called repeatedly and
 * a step will not be performed each call.
 * 
 * @param dir: The direction. 1 closes, 0 opens.
 * @return: How many steps have been performed (1 or 0).
 */
int make_step_no_del(int close, int deltime)
{
    static unsigned long next_step_micros = 0;
    static bool high_step = false; 

    int step_performed = 0;

    // Only act if the delay time has been reached
    if (micros() >= next_step_micros)
    {
        // Write the direction pin
        if (close)
            digitalWrite(STEPPER_DIR_PIN, LOW);
        else
            digitalWrite(STEPPER_DIR_PIN, HIGH);

        // Write low
        if (!high_step)
        {
            // TODO Maybe check if already set
            digitalWrite(PERFORM_STEP_PIN, LOW);
            high_step = true;
        }
        // Write high
        if (high_step)
        {            
            digitalWrite(PERFORM_STEP_PIN, HIGH);
            if (close)
                current_steps--;
            else
                current_steps++;
            
            high_step = false;

            step_performed = 1;
        }

        // Calculate the next time step to act upon.
#ifdef CHECK_MICROS_OVERFLOW
        // Leave approximately 1ms buffer in case of an overflow
        next_step_micros = (micros() < 0xFFFFFFFFFFFFFFD0) ? 
                            (micros() + deltime) : 0;
#else
        next_step_micros = micros() + DEL_TIME;
#endif // CHECK_MICROS_OVERFLOW
    }

    return step_performed;
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
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
    stepper_enabled = true;
}

/*
 * Reduces power draw or noise of the stepper during a standstill.
 * This could be acchieved by either completely disabling the driver
 * or by enabling the passive breakmode.
 * 
 * @retun: None.
 */
void disable_stepper()
{
    // Some GPIO stuff that disables the stepper driver.
    // Could also enable passive breakmode of the TMC2209.
    digitalWrite(STEPPER_ENABLE_PIN, HIGH);
    stepper_enabled = false;
}
