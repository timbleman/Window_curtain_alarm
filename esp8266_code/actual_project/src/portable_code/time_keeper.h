/*
 * The time keeper keeps track of the current time and sleep and wake times.
 * To be able to achieve multiple sleep/wake times a day, a major refactoring
 * would be required.
 */

#ifndef _TIME_KEEPER_H_
#define _TIME_KEEPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Includes **********************************/
#include <stdint.h>
#include "configuration.h"

#ifdef TESTABLE_TK_CODE
#include <time.h>
#endif // TESTABLE_TK_CODE


/********************************* Constants *********************************/
#define DEFAULT_WEEKEND_WAKE 9
#define DEFAULT_WEEK_WAKE 8
#define DEFAULT_SLEEP 23


/***************************** Struct definitions *****************************/
#ifdef TESTABLE_TK_CODE
typedef struct {
    struct tm tm_mon;
    struct tm tm_tue;
    struct tm tm_wed;
    struct tm tm_thu;
    struct tm tm_fri;
    struct tm tm_sat;
    struct tm tm_sun;
} one_tm_per_wd;
#endif // TESTABLE_TK_CODE


/**************************** Variable definitions ****************************/
#ifdef TESTABLE_TK_CODE
one_tm_per_wd wake_times;
one_tm_per_wd sleep_times;
#endif // TESTABLE_TK_CODE


/**************************** Prototype functions ****************************/
/*
 * Default wake and sleep times are set for each weekday.
 * 
 * @return: 0 if successful.
 */
int setup_time_keeper();

/*
 * Set the time to wake up for multiple days.
 * Days are onehot encoded as in the DAY_TYPE defined in command_parser.
 * Unsets ignore of a single wake.
 * 
 * @param days: The weekdays to set.
 * @param h: The hour to open the curtain.
 * @param m: The minute to open the curtain.
 * @param s: The second to open the curtain.
 * @return: 0 if successful.
 */
int set_wake(uint32_t days, int h, int m, int s);

/*
 * Set the time to sleep for multiple days.
 * Days are onehot encoded as in the DAY_TYPE defined in command_parser.
 * Unsets ignore of a single wake.
 * 
 * @param days: The weekdays to set.
 * @param h: The hour to close the curtain.
 * @param m: The minute to close the curtain.
 * @param s: The second to close the curtain.
 * @return: 0 if successful.
 */
int set_sleep(uint32_t days, int h, int m, int s);

/*
 * The time difference from now until the wake time of the current weekday.
 * Positive if one_tm_per_wd entry is in the future, negative if in the past.
 * 
 * @return: Difference in seconds.
 */
long time_until_wake();

/*
 * The time difference from now until the sleep time of the current weekday.
 * Positive if one_tm_per_wd entry is in the future, negative if in the past.
 * 
 * @return: Difference in seconds.
 */
long time_until_sleep();

/*
 * To check the current system time.
 * Copy by value is performed, should not be called all the time.
 * 
 * @return: The current day in the C tm struct format.
 */
int get_current_d_tm();

/*
 * To check the current system time.
 * Copy by value is performed, should not be called all the time.
 * 
 * @return: The current hour.
 */
int get_current_h();

/*
 * To check the current system time.
 * Copy by value is performed, should not be called all the time.
 * 
 * @return: The current minute.
 */
int get_current_m();

/*
 * To check the current system time.
 * Copy by value is performed, should not be called all the time.
 * 
 * @return: The current second.
 */
int get_current_s();


/*
 * The time keeper is able to ignore the next wake.
 * Call this if the next wake should be ignored.
 * 
 * @return: None.
 */
void set_ignore();

/*
 * The time keeper is able to ignore the next wake.
 * Call this if the next wake should no longer be ignored.
 * 
 * @return: None.
 */
void unset_ignore();

/*
 * The time keeper is able to ignore the next wake.
 * Check if the next wake should be ignored.
 * 
 * @return: 1 if to be ignored, 0 if not.
 */
int get_ignore();

/*
 * The time keeper is able to ignore the next wake.
 * For the ignore to be correctly updated, this function nedds to be called 
 * repeatedly.
 * 
 * @return: None.
 */
void update_ignore();

/*
 * Get all the wake times in a user readable format.
 * 
 * @param buf: String buffer to write into.
 * @param max_len: The maximum length of the string.
 * @return: Success if 0.
 */
int write_wake_times_message(char *buf, int max_len);

/*
 * Get all the sleep times in a user readable format.
 * 
 * @param buf: String buffer to write into.
 * @param max_len: The maximum length of the string.
 * @return: Success if 0.
 */
int write_sleep_times_message(char *buf, int max_len);

#ifdef TESTABLE_TK_CODE
int format_time_to_str(char *str, int max_len, int h, int m, int s);
#endif // TESTABLE_TK_CODE

#ifdef __cplusplus
}
#endif

#endif // _TIME_KEEPER_H_