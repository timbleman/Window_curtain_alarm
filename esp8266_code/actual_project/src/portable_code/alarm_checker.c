/********************************** Includes **********************************/
#include "alarm_checker.h"
#include <stdio.h>


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
#ifndef TESTABLE_ALARMCHECKER_CODE
enum CURTAIN_STATE new_state_ttw_tts_today(long tuw_today, long tus_today);
#endif // TESTABLE_ALARMCHECKER_CODE


/**************************** Variable definitions ****************************/
/**************************** Function definitions ****************************/
/*
 * Checks if an alarm should be triggered.
 * Factors in curtain state, the wake, sleep and current time.
 * In this current iteration only today's wake and sleep times are used.
 * FIXME This overrides other open/close commands. 
 * 
 * @return: 0 if finished, 1 if still working?
 */
int check_and_alarm_non_blocking()
{
    static enum CURTAIN_STATE old_state = CURTAIN_UNDEFINED_T;

    // Update the ignore one wake.
    update_ignore();
    
    // Actual state may not be needed...
    //enum CURTAIN_STATE actual_state = get_curtain_state();
    enum CURTAIN_STATE new_state = new_state_ttw_tts_today(time_until_wake(), 
                                                        time_until_sleep());

    int status = 0;
    if (new_state != old_state)
    {
        switch(new_state)
        {
            case CURTAIN_OPEN_T:    if (!get_ignore())
                                        status =  open_nonblocking();
                                    else
                                        status =  1;
                                    break;
            case CURTAIN_CLOSED_T:  status = close_nonblocking();
                                    break;
            case CURTAIN_UNDEFINED_T:   break;
            default:    break;
        }
    }

    if (status == 0)
        old_state = new_state;
    
    return status;
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

