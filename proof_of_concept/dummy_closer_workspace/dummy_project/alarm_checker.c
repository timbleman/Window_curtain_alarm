/********************************** Includes **********************************/
#include "alarm_checker.h"
#include <stdio.h>
#include <string.h>


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
#ifndef TESTABLE_ALARMCHECKER_CODE
enum CURTAIN_STATE new_state_ttw_tts_today(long tuw_today, long tus_today);
#endif // TESTABLE_ALARMCHECKER_CODE
int setup_alarm_checker_comm();
int alarm_checker_input();
user_action_t fetch_alarm_checker_action();
int respond_to_alarm_checker(char *buf, int buf_max_len);


/**************************** Variable definitions ****************************/
static enum CURTAIN_STATE curtain_action = CURTAIN_UNDEFINED_T;


/**************************** Function definitions ****************************/
/*
 * Setup an input_handler_t. Better way to handle multiple input sources.
 * Checks if an alarm should be triggered.
 * Factors in curtain state, the wake, sleep and current time.
 * In this current iteration only today's wake and sleep times are used.
 * 
 * @param inh: An input_handler_t to initialize.
 * @return: Success status.
 */
int setup_alarm_checker_input_handler(input_handler_t *inh)
{
    inh->setup = setup_alarm_checker_comm;
    inh->input_available = alarm_checker_input;
    inh->fetch_action = fetch_alarm_checker_action;
    inh->respond_to_user = respond_to_alarm_checker;
    strncpy(inh->tag, "alarm_checker", TAG_LEN);

    return inh->setup();
}

/* 
 * Setup local inputs and display.
 * Needed for the input_handler_t struct.
 * 
 * @return: Success status.
 */
int setup_alarm_checker_comm()
{
    curtain_action = CURTAIN_UNDEFINED_T;
    
    return 0;
}

/*
 * Update and check if opening or closing is needed at the current time.
 * Has to be called repeatedly.
 * Returns success if an input is available to be fetched.
 * 
 * @return: 0 if input is available, 1 if not.
 */
int alarm_checker_input()
{
    static enum CURTAIN_STATE old_state = CURTAIN_UNDEFINED_T;
    
    // Try to get teh actual curtain_state if undefined
    if (old_state == CURTAIN_UNDEFINED_T)
        old_state = get_curtain_state();

    int status = 1;
    
    // Update the ignore one wake.
    update_ignore();
    
    // Actual state may not be needed...
    enum CURTAIN_STATE new_state = new_state_ttw_tts_today(time_until_wake(), 
                                                        time_until_sleep());

    // Check whether to open or close. 
    if (new_state != old_state)
    {
        switch(new_state)
        {
            case CURTAIN_OPEN_T:    // If ignore, do nothing.
                                    // If ignore is unset, the curtain should open.
                                    if (!get_ignore())
                                    {
                                        curtain_action = OPEN_T;
                                        status =  0;
                                    }
                                    break;
            case CURTAIN_CLOSED_T:  curtain_action = CLOSE_T;
                                    status = 0;
                                    break;
            case CURTAIN_UNDEFINED_T:   break;
            default:    break;
        }
    }

    /* 
     * Update the curtain_state if an OPEN_T or CLOSE_T has been stored.
     * Using a dedicated variable instead of the actual state, allows
     * the user to open curtain manually and not closing it immediately
     * if it is sleeptime. 
     */
    if (status == 0)
        old_state = new_state;
    
    return status;
}

/*
 * Fetch an action if available. alarm_checker_input() has to be called first.
 * The alarm checker is only able to create OPEN or CLOSE actions.
 * Pass by value with copying is used, should not be a problem for this small
 * size. 
 * 
 * @return: A user_action_t.
 */
user_action_t fetch_alarm_checker_action()
{
    user_action_t new_act = {0};
    
    if (curtain_action == CURTAIN_UNDEFINED_T)
    {
        new_act.act_type = NONE_T;
    }
    else
    {
        new_act.act_type = CURTAIN_CONTROL_T;
        new_act.data[0] = curtain_action;
        curtain_action = CURTAIN_UNDEFINED_T;
    }
    
    return new_act;
}

/*
 * Dummy function, needed for the input_handler_t struct.
 * 
 * @param buf: Unused response buffer.
 * @param buf_max_len: Maximum number of chars.
 * @return: Success status.
 */
int respond_to_alarm_checker(__attribute__((unused))char *buf, 
                            __attribute__((unused)) int buf_max_len)
{
    return 0;
}

/*
 * This function checks whether the curtain state should remain unchanged
 * or if the curtain should be opened or closed.
 * Currently only the time_to_wake and time_to_sleep of the current day are
 * used.
 * If undefined is returned, the current state should be kept.
 * 
 * @param ttw_today: time_to_wake of the current day.
 * @param tts_today: time_to_sleep of the current day.
 * @return: Action to perform. Open close or undefined.
 */
enum CURTAIN_STATE new_state_ttw_tts_today(long tuw_today, long tus_today)
{
    if (tuw_today > 0)
    {
        if (tus_today < 0)
        {
            return CURTAIN_CLOSED_T;
        }
        else
        {
            return CURTAIN_UNDEFINED_T;
        }
    }
    else // tuw_today <= 0
    {
        if (tus_today > 0)
        {
            return CURTAIN_OPEN_T;
        }
        else
        {
            if (tuw_today < tus_today)
            {
                return CURTAIN_CLOSED_T;
            }
            else
            {
                return CURTAIN_OPEN_T;
            }
        }
    }
}
