#ifndef _ALARM_CHECKER_H_
#define _ALARM_CHECKER_H_

/********************************** Includes **********************************/
#include "time_keeper.h"
#include "motor_controller.h"
#include "configuration.h"


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions ****************************/
/*
 * Checks if an alarm should be triggered.
 * Factors in curtain state, the wake, sleep and current time.
 * In this current iteration only today's wake and sleep times are used.
 * 
 * @return: Success status or something?
 */
int check_and_alarm_non_blocking();

#ifdef TESTABLE_ALARMCHECKER_CODE
enum CURTAIN_STATE new_state_ttw_tts_today(long ttw_today, long tts_today);
#endif // TESTABLE_ALARMCHECKER_CODE

#endif // _ALARM_CHECKER_H_