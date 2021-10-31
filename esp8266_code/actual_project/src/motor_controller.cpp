/********************************** Includes **********************************/
#include <Arduino.h>
#include "motor_controller.h"

/********************************* Constants **********************************/
#define STEPPER_ENABLE_PIN D1
#define STEPPER_DIR_PIN D2
#define PERFORM_STEP_PIN D5
#define END_STOP_PIN D6

#define CHECK_MICROS_OVERFLOW
// Stop the motor if the endstop is reached sooner than expected.
#define ABORT_AT_ENDSTOP


/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
bool stepper_enabled = false;


/**************************** Variable definitions ****************************/
#ifndef TESTABLE_MOTOR_CODE
int current_steps = 0;
int target_steps = 40;
#endif // TESTABLE_MOTOR_CODE
bool calibrated = false;


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
    // Setup variables
    current_steps = 0;
    target_steps = 40;

    // Setup pins
    pinMode(STEPPER_ENABLE_PIN, OUTPUT);
    pinMode(STEPPER_DIR_PIN, OUTPUT);
    pinMode(PERFORM_STEP_PIN, OUTPUT);
    // Use an external pulldown and cap for debouncing
    pinMode(END_STOP_PIN, INPUT);

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
        make_step_no_del(0);
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

#ifdef ABORT_AT_ENDSTOP
    // Stop the motor if the endstop is reached sooner than expected.
    // Only to prevent damadge, does not roll back.
    int end_stop = digitalRead(END_STOP_PIN);
    if ((current_steps <= 0) || end_stop == LOW)
#else
    if (current_steps <= 0)
#endif // ABORT_AT_ENDSTOP
    {
        return 0;
    }
    else
    {
        if (!stepper_enabled)
            enable_stepper();
        make_step_no_del(1);
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
    static CURTAIN_STATE state_when_started = CURTAIN_UNDEFINED_T;

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
#ifdef PRINTS_AT_EACH_STEP
    printf("Closing and calibrating...\n");
#endif // PRINTS_AT_EACH_STEP
    static bool started_calibrating = false;
    static int counted_steps = 0;

    if (!started_calibrating)
    {
        counted_steps = 0;
        started_calibrating = true;
    }
    
    // TODO Make this work
    // Interrupt or polling? Polling should be fine for now...
    int end_stop = digitalRead(END_STOP_PIN);
    if (end_stop == LOW)
    {
        target_steps = counted_steps;
#ifdef MOTOR_PRINTS
        printf("Set target steps to %i\n", target_steps);
#endif // MOTOR_PRINTS
        current_steps = 0;
        started_calibrating = false;
        calibrated = true;
        return 0;
    }
    
    make_step(1);
    counted_steps++;
    return 1;
}

/*
 * This function is mostly equivalent to close(), the main difference being
 * that the internal step target for fully opening is set.
 * Rolls back a few steps after the endstop has been activated.
 * 
 * TODO This has to be tried and debugged.
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
    static int rolled_back_steps = 0;

    if (!started_calibrating)
    {
        counted_steps = 0;
        rolled_back_steps = 0;
        end_stop_triggered = false;
        started_calibrating = true;
    }
    
    // TODO Make this work
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
        if (rolled_back_steps >= rollback_target)
        {
            // Reset the static variables
            current_steps = 0;
            end_stop_triggered = false;
            rolled_back_steps = 0;
            started_calibrating = false;
            // Set the target steps
            target_steps = counted_steps - rollback_target;
            calibrated = true;
#ifdef MOTOR_PRINTS
            printf("Set target steps to %i\n", target_steps);
#endif // MOTOR_PRINTS
            return 0;
        }

        // Make a step in the opposing direction.
        make_step(0);
        rolled_back_steps++;
    }
    else
    {
        make_step(1);
        counted_steps++;
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
void make_step(int close)
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

// TODO This function is obsolete.
void make_step_no_del_non_refact(int close)
{
    static unsigned long next_step_micros = 0;
    static bool high_step = false; 
    // Keep in mind this is used two times
    const int DEL_TIME = 300;

    if (close)
    {
        if (micros() >= next_step_micros)
        {
            // Close
            if (!high_step)
            {
                // TODO Maybe check if already set
                digitalWrite(STEPPER_DIR_PIN, LOW);
                // TODO Delay is not optimal, but may be short enough to not be noticeable
                digitalWrite(PERFORM_STEP_PIN, LOW);
                high_step = true;
            }
            if (high_step)
            {
                // TODO Try 1, 2, 5
                //delay(DEL_TIME);
                
                digitalWrite(PERFORM_STEP_PIN, HIGH);
                current_steps--;
                high_step = false;
            }
            next_step_micros = micros() + DEL_TIME;
        }
    }
    else
    {
        if (micros() >= next_step_micros)
        {
            // Open
            if (!high_step)
            {
                // TODO Maybe check if already set
                digitalWrite(STEPPER_DIR_PIN, HIGH);
                // TODO Delay is not optimal, but may be short enough to not be noticeable
                digitalWrite(PERFORM_STEP_PIN, LOW);
                next_step_micros = micros() + DEL_TIME;
                high_step = true;
            }
            if (high_step)
            {
                // TODO Try 1, 2, 5
                //delay(DEL_TIME);
                if (micros() >= next_step_micros)
                {
                    digitalWrite(PERFORM_STEP_PIN, HIGH);
                    current_steps++;
                    high_step = false;
                }
            }
            next_step_micros = micros() + DEL_TIME;
        }
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
 * @return: None.
 */
void make_step_no_del(int close)
{
    static unsigned long next_step_micros = 0;
    static bool high_step = false; 
    // Remember this is used two times, generates a square wave 50% duty cycle
    const int DEL_TIME = 300;

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
            //digitalWrite(STEPPER_DIR_PIN, LOW);
            // TODO Delay is not optimal, but may be short enough to not be noticeable
            digitalWrite(PERFORM_STEP_PIN, LOW);
            high_step = true;
        }
        // Write high
        if (high_step)
        {
            // TODO Try 1, 2, 5
            //delay(DEL_TIME);
            
            digitalWrite(PERFORM_STEP_PIN, HIGH);
            if (close)
                current_steps--;
            else
                current_steps++;
            
            high_step = false;
        }

        // Calculate the next time step to act upon.
#ifdef CHECK_MICROS_OVERFLOW
        // Leave approximately 1ms buffer in case of an overflow
        next_step_micros = (micros() < 0xFFFFFFFFFFFFFFD0) ? 
                            (micros() + DEL_TIME) : 0;
#else
        next_step_micros = micros() + DEL_TIME;
#endif // CHECK_MICROS_OVERFLOW
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
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
    stepper_enabled = true;
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
    digitalWrite(STEPPER_ENABLE_PIN, HIGH);
    stepper_enabled = false;
}
