/*
 * The alarm checker needs to called in a loop.
 * It checks, whether the curtain should be open and closed based on the
 * wake/sleep times saved by the time keeper.
 */

#ifndef _ALARM_CHECKER_H_
#define _ALARM_CHECKER_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Includes **********************************/
#include <stdint.h>
#include "hardware_specific_code/motor_controller.h"
#include "configuration.h"
#include "types_and_enums.h"


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
enum CURTAIN_STATE new_state_ttw_tts_today(long tuw_today, long tus_today);
#endif // TESTABLE_ALARMCHECKER_CODE

#ifdef __cplusplus
}
#endif

#endif // _ALARM_CHECKER_H_