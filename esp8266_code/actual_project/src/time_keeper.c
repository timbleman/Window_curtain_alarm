#include "time_keeper.h"


/********************************** Includes **********************************/
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h> // strlen
#include <limits.h> // Max int
#include "types_and_enums.h"
#include "data_storage.h"
#ifndef TESTABLE_TK_CODE
#include <time.h>
#endif // TESTABLE_TK_CODE


/********************************* Constants *********************************/


/***************************** Struct definitions *****************************/
#ifndef TESTABLE_TK_CODE
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


/**************************** Prototype functions ****************************/
long only_time_wd_diff(struct tm *tm1, struct tm *tm2);
long only_time_diff(struct tm *tm1, struct tm *tm2);
int set_tm_per_wd(one_tm_per_wd *tmpwd, int wd, int h, int m, int s);
struct tm *get_wdtm(one_tm_per_wd *tmpwd, int wd);
long secs_until_tm_today(one_tm_per_wd *tmpwd);
int get_tm_day_from_day_type(uint32_t dayt);
int set_tm_multiple_days(one_tm_per_wd *time_strct,
                            uint32_t days, int h, int m, int s);
int write_week_times_message(char *str, int max_len, one_tm_per_wd *week_tms);
int format_tm_time_to_str(char *str, int max_len, struct tm *tmstrct);
int format_time_to_str(char *str, int max_len, int h, int m, int s);
int save_times();
int load_times();


/**************************** Variable definitions ****************************/
// Ignore the next wake time.
static bool ignore_once;
// These could be static inside update_ignore(), but this complicates resetting.
static bool already_slept = false;
static bool already_ignored_wake = false;

#ifndef TESTABLE_TK_CODE
one_tm_per_wd wake_times;
one_tm_per_wd sleep_times;
#endif // TESTABLE_TK_CODE


/**************************** Function definitions ****************************/
/*
 * Default wake and sleep times are set for each weekday.
 * 
 * @return: 0 if successful.
 */
int setup_time_keeper()
{
    int status = 0;
    
    // If load times from the memory is unsuccessful set defaults.
    if (load_times())
    {
        // Set default time for weekdays
        for (int i = 1; i < 6; i++)
        {
            status |= set_tm_per_wd(&wake_times, i, DEFAULT_WEEK_WAKE, 0, 0);
            status |= set_tm_per_wd(&sleep_times, i, DEFAULT_SLEEP, 0, 0);
        }
        // Set default time for the weekend
        status |= set_tm_per_wd(&wake_times, 6, DEFAULT_WEEKEND_WAKE, 0, 0);
        status |= set_tm_per_wd(&sleep_times, 6, DEFAULT_SLEEP, 0, 0);
        status |= set_tm_per_wd(&wake_times, 0, DEFAULT_WEEKEND_WAKE, 0, 0);
        status |= set_tm_per_wd(&sleep_times, 0, DEFAULT_SLEEP, 0, 0);
    }
    
    // Reset all the ignore one wake variables.
    ignore_once = false;
    already_slept = false;
    already_ignored_wake = false;
    
    return status;
}

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
int set_wake(uint32_t days, int h, int m, int s)
{
    // To avoid strange bugs.
    unset_ignore();
    return set_tm_multiple_days(&wake_times,
                            days, h, m, s);
}

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
int set_sleep(uint32_t days, int h, int m, int s)
{
    // To avoid strange bugs.
    unset_ignore();
    return set_tm_multiple_days(&sleep_times,
                            days, h, m, s);
}

/*
 * Set the times for multiple days.
 * 
 * @param time_struct: one_tm_per_wd instance that saves a time for each weekday.
 * @param h: The hour to close the curtain.
 * @param m: The minute to close the curtain.
 * @param s: The second to close the curtain.
 * @return: 0 if successful.
 */
int set_tm_multiple_days(one_tm_per_wd *time_strct,
                            uint32_t days,
                            int h, int m, int s)
{
    int status = 0;
    
    // Perform validity checks
    if ((days == 0) || ((days & 0xFFFFFF80) > 0))
        status |= 1;
        
    // Set times for selected days
    for (int i = 0; i < 7; i++)
    {
        // Check if a weekday has been set
        int single_weekday = 1 << i;
        if ((days & single_weekday) != 0)
        {
            int day_to_set = get_tm_day_from_day_type(single_weekday);
            if (day_to_set == INVALID_DAY_ERR)
                return INVALID_DAY_ERR;
            status |= set_tm_per_wd(time_strct, day_to_set, h, m, s);
        }
    }
    return status;
}
  
/*
 * Returns the first day in tm struct representation found in multiple days.
 * TODO Check for onehot encoding!
 * 
 * @param dayt: A single week day in enum DAY_TYPE representation.
 * @return: The week day in tm struct representation.
 */
int get_tm_day_from_day_type(uint32_t dayt)
{
    switch (dayt)
    {
        case MON_T: return 1;
                    break;
        case TUE_T: return 2;
                    break;
        case WED_T: return 3;
                    break;
        case THU_T: return 4;
                    break;
        case FRI_T: return 5;
                    break;
        case SAT_T: return 6;
                    break;
        case SUN_T: return 0;
                    break;
        default:    return INVALID_DAY_ERR;
                    break;
    }
}

/*
 * The time difference from now until the wake time of the current weekday.
 * Positive if one_tm_per_wd entry is in the future, negative if in the past.
 * 
 * @return: Difference in seconds.
 */
long time_until_wake()
{
    return secs_until_tm_today(&wake_times);
}

/*
 * The time difference from now until the sleep time of the current weekday.
 * Positive if one_tm_per_wd entry is in the future, negative if in the past.
 * 
 * @return: Difference in seconds.
 */
long time_until_sleep()
{
    return secs_until_tm_today(&sleep_times);
}

/*
 * This function calculates the difference in seconds between two weekdays.
 * The year, month, etc are not considered.
 * Does not perform bounds checking yet, as module is expensive.
 * 
 * @param tm1: Weekday 1.
 * @param tm1: Weekday 24.
 * @return: Difference in seconds.
 */
long only_time_wd_diff(struct tm *tm1, struct tm *tm2)
{
    long tdiff = 0;
    tdiff += (tm2->tm_wday - tm1->tm_wday) * 24 * 60 * 60;
    tdiff += (tm2->tm_hour - tm1->tm_hour) * 60 * 60;
    tdiff += (tm2->tm_min - tm1->tm_min) * 60;
    tdiff += (tm2->tm_sec - tm1->tm_sec);
    return tdiff;
}

/*
 * This function calculates the difference in seconds between two struct tm.
 * The weekday, year, month, etc are not considered.
 * Does not perform bounds checking yet, as module is expensive.
 * 
 * @param tm1: Weekday 1.
 * @param tm1: Weekday 24.
 * @return: Difference in seconds.
 */
long only_time_diff(struct tm *tm1, struct tm *tm2)
{
    long tdiff = 0;
    tdiff += (tm2->tm_hour - tm1->tm_hour) * 60 * 60;
    tdiff += (tm2->tm_min - tm1->tm_min) * 60;
    tdiff += (tm2->tm_sec - tm1->tm_sec);
    return tdiff;
}

/*
 * The time difference from now until the one_tm_per_wd selected by weekday.
 * Positive if one_tm_per_wd entry is in the future, negative if in the past.
 * 
 * @param tmpwd: one_tm_per_wd instance that saves a time for each weekday.
 * @return: Difference in seconds.
 */
long secs_until_tm_today(one_tm_per_wd *tmpwd)
{
    time_t rawtime;
    struct tm currenttime;
    time(&rawtime);
    // Copy by value to not be affected by delays
    currenttime = *localtime(&rawtime);
    
    struct tm *tm_to_comp = get_wdtm(tmpwd, currenttime.tm_wday); 
    if (tm_to_comp == NULL)
        return LONG_MAX;
    
    printf("currenttime: hour %i, min %i, sec %i \n", 
            currenttime.tm_hour, currenttime.tm_min, currenttime.tm_sec);
    printf("tm_to_comp: hour %i, min %i, sec %i \n", 
            tm_to_comp->tm_hour, tm_to_comp->tm_min, tm_to_comp->tm_sec);
    
    return only_time_diff(&currenttime, tm_to_comp); 
}

/*
 * Sets a time for a selected weekday in a one_tm_per_wd entity.
 * 
 * @param tmpwd: one_tm_per_wd struct that saves a time for each weekday.
 * @param wd: The weekday in the one_tm_per_wd to set.
 * @param h: The hour to set.
 * @param m: The minute to set.
 * @param s: The second to set.
 * @return: Success status.
 */
int set_tm_per_wd(one_tm_per_wd *tmpwd, int wd, int h, int m, int s)
{
    int errs = 0;
    // Some validity checks
    if (wd < 0 || wd > 6)
        errs |= INVALID_DAY_ERR;
    if (h < 0 || h > 23)
        errs |= INVALID_TIME_ERR;
    if (m < 0 || m > 59)
        errs |= INVALID_TIME_ERR;
    if (s < 0 || s > 59)
        errs |= INVALID_TIME_ERR;
    if (errs != 0)
        return errs;
            
    // Set the time of the selected weekday
    struct tm *tm_to_set = get_wdtm(tmpwd, wd);
    // TODO May uncomment.
    tm_to_set->tm_wday = wd;
    tm_to_set->tm_hour = h;
    tm_to_set->tm_min = m;
    tm_to_set->tm_sec = s;
    
    // 0 if successful
    return 0;
}

/*
 * Get a selected weekday from a one_tm_per_wd entity.
 * Returning NULL in case of an invalid wd is a design decission, resulting
 * in more necessary checks to avoid memory segmentation errors.
 * 
 * @param tmpwd: one_tm_per_wd struct that saves a time for each weekday.
 * @param wd: The weekday in the one_tm_per_wd to select.
 * @return: struct tm entity.
 */
struct tm *get_wdtm(one_tm_per_wd *tmpwd, int wd)
{
    switch(wd)
    {
        case 0: return &tmpwd->tm_sun;
                break;
        case 1: return &tmpwd->tm_mon;
                break;
        case 2: return &tmpwd->tm_tue;
                break;
        case 3: return &tmpwd->tm_wed;
                break;
        case 4: return &tmpwd->tm_thu;
                break;
        case 5: return &tmpwd->tm_fri;
                break;
        case 6: return &tmpwd->tm_sat;
                break;
        default: return NULL;
                break;
    }
}

/*
 * To check the current system time.
 * Copy by value is performed, should not be called all the time.
 * 
 * @return: The current day in the C tm struct format.
 */
int get_current_d_tm()
{
    time_t rawtime;
    struct tm currenttime;
    time(&rawtime);
    // Copy by value to not be affected by delays
    currenttime = *localtime(&rawtime);
    return currenttime.tm_wday;
}

/*
 * To check the current system time.
 * Copy by value is performed, should not be called all the time.
 * 
 * @return: The current hour.
 */
int get_current_h()
{
    time_t rawtime;
    struct tm currenttime;
    time(&rawtime);
    // Copy by value to not be affected by delays
    currenttime = *localtime(&rawtime);
    return currenttime.tm_hour;
}

/*
 * To check the current system time.
 * Copy by value is performed, should not be called all the time.
 * 
 * @return: The current minute.
 */
int get_current_m()
{
    time_t rawtime;
    struct tm currenttime;
    time(&rawtime);
    // Copy by value to not be affected by delays
    currenttime = *localtime(&rawtime);
    return currenttime.tm_min;
}

/*
 * To check the current system time.
 * Copy by value is performed, should not be called all the time.
 * 
 * @return: The current second.
 */
int get_current_s()
{
    time_t rawtime;
    struct tm currenttime;
    time(&rawtime);
    // Copy by value to not be affected by delays
    currenttime = *localtime(&rawtime);
    return currenttime.tm_sec;
}

/*
 * Get all the wake times in a user readable format.
 * 
 * @param buf: String buffer to write into.
 * @param max_len: The maximum length of the string.
 * @return: Success if 0.
 */
int write_wake_times_message(char *buf, int max_len)
{
    int status = write_week_times_message(buf, max_len, &wake_times);
    memcpy(buf, "wake times:", 11);
    return status;
}

/*
 * Get all the sleep times in a user readable format.
 * 
 * @param buf: String buffer to write into.
 * @param max_len: The maximum length of the string.
 * @return: Success if 0.
 */
int write_sleep_times_message(char *buf, int max_len)
{
    int status = write_week_times_message(buf, max_len, &sleep_times);
    memcpy(buf, "sleep times:", 12);
    return status;
}

/*
 * Writes times of a one_tm_per_wd struct into a user readable message.
 * Not easily adjustable code, heavily dependent on the message format.
 * 
 * @param str: The buffer to write into.
 * @param max_len: The maximum depth of the supplied buffer.
 * @param week_tms: one_tm_per_wd containg times for a whole week.
 * @return: 0 if successful.
 */
int write_week_times_message(char *str, int max_len, one_tm_per_wd *week_tms)
{
    char msg[128] = "             \n\r"
                "mon: xx:xx:xx\n\r"
                "tue: xx:xx:xx\n\r"
                "wed: xx:xx:xx\n\r"
                "thu: xx:xx:xx\n\r"
                "fri: xx:xx:xx\n\r"
                "sat: xx:xx:xx\n\r"
                "sun: xx:xx:xx\n\r";
                
    // Abort if the buffer is too small
    if (max_len < strlen(msg))
        return 1;

    /*
     * Pulling these magic number out of nowhere is not good practice.
     * Instead of this approach some sprintf() could be used, however,
     * the function may not be as predicatable (memory) as this approach.
     */
    const int line_offset = 15;
    const int time_offset = 5;
    const int str_term_off = time_offset + 8;
    const int mon_offset =  time_offset + line_offset * 1;
    const int tue_offset =  time_offset + line_offset * 2;
    const int wed_offset =  time_offset + line_offset * 3;
    const int thu_offset =  time_offset + line_offset * 4;
    const int fri_offset =  time_offset + line_offset * 5;
    const int sat_offset =  time_offset + line_offset * 6;
    const int sun_offset =  time_offset + line_offset * 7;
    
    /*
     * Write times into string.
     * Remember, each of the written strings has terminating '\0', which needs
     * to be removed.
     */
    int status = 0;
    status |= format_tm_time_to_str(&msg[mon_offset], 9, &(week_tms->tm_mon));
    status |= format_tm_time_to_str(&msg[tue_offset], 9, &(week_tms->tm_tue));
    status |= format_tm_time_to_str(&msg[wed_offset], 9, &(week_tms->tm_wed));
    status |= format_tm_time_to_str(&msg[thu_offset], 9, &(week_tms->tm_thu));
    status |= format_tm_time_to_str(&msg[fri_offset], 9, &(week_tms->tm_fri));
    status |= format_tm_time_to_str(&msg[sat_offset], 9, &(week_tms->tm_sat));
    status |= format_tm_time_to_str(&msg[sun_offset], 9, &(week_tms->tm_sun));
    //status |= format_time_to_str(msg, 10, 1, 1, 1);
    
    /*
     * Replace every '\0' by '\n', except the last one.
     * This is not portable, heavily tied to message format.
     */
    for (int i = 0; i < 8; ++i)
    {
        int offs = str_term_off + line_offset * i;
        msg[offs] = '\n';
    }
    
    // Copy message into buffer.
    memcpy(str, msg, strlen(msg));
    //printf("msg %s\n", msg);
    //printf("str %s\n", str);
    return status;
}

/*
 * Formats a c time struct tm into a string of the format "hh:mm:ss".
 * Modified in place.
 * 
 * @param str: Buffer to write into.
 * @param max_len: The maximum depth of the supplied buffer.
 * @param tmstrct: C struct tm containg h, m and s.
 * @return: 0 if successful.
 */
int format_tm_time_to_str(char *str, int max_len, struct tm *tmstrct)
{
    return format_time_to_str(str, max_len, tmstrct->tm_hour, 
                                            tmstrct->tm_min,
                                            tmstrct->tm_sec);
}

/*
 * Formats a hour, min and second into a string of the format "hh:mm:ss".
 * Modified in place.
 * 
 * @param str: Buffer to write into.
 * @param max_len: The maximum depth of the supplied buffer.
 * @param h: Hour.
 * @param m: Minute.
 * @param s: Second.
 * @return: 0 if successful.
 */
int format_time_to_str(char *str, int max_len, int h, int m, int s)
{
    // "12:12:12\n" --> 9 chars
    if (max_len < 9)
        return 1;
        
    if (h > 99 || m > 99 || s > 99
        || h < 0 || m < 0 || s < 0)
    {
        strcpy(str, "-1:-1:-1");
        return 1;
    }
    
    // This may not work on an embedded platform, if so adjust test lel.
    sprintf(str, "%02d:%02d:%02d", h, m, s);
    return 0;
}

/*
 * Saves the currently set times into non-volatile memory.
 * For ESP8266: Store in flash. For linux based systems: Some file.
 * 
 * @return: Success status.
 */
int save_times()
{
    struct tm *one_day;
    int status = 0;
    
    // Wake at first
    for (int i = 0; i < 7; i++)
    {
        one_day = get_wdtm(&wake_times, i);
        store_time(i, i, one_day->tm_hour, one_day->tm_min, one_day->tm_sec);
    }
    // Then sleep
    for (int i = 0; i < 7; i++)
    {
        one_day = get_wdtm(&sleep_times, i);
        store_time(i + 7, i, one_day->tm_hour, one_day->tm_min, one_day->tm_sec);
    }
    
    return status;
}

/*
 * Loads wake and sleep times from the memory.
 * 
 * @return: Success status.
 */
int load_times()
{
    int status = 0;
    
    uint8_t day = 0, h = 0, m = 0, s = 0;
    
    // Wake at first
    for (int i = 0; i < 7; i++)
    {
        load_time(i, &day, &h, &m, &s);
        status |= set_tm_per_wd(&wake_times, day, h, m, s);
        //store_time(i, i, one_day->tm_hour, one_day->tm_min, one_day->tm_sec);
    }
    // Then sleep
    for (int i = 0; i < 7; i++)
    {
        load_time(i + 7, &day, &h, &m, &s);
        status |= set_tm_per_wd(&sleep_times, day, h, m, s);
        //store_time(i + 7, i, one_day->tm_hour, one_day->tm_min, one_day->tm_sec);
    }
    
#ifdef TESTABLE_TK_CODE
    return 1;
#else
    return status;
#endif // TESTABLE_TK_CODE
}

/*
 * The time keeper is able to ignore the next wake.
 * Call this if the next wake should be ignored.
 * 
 * @return: None.
 */
void set_ignore()
{
    ignore_once = true;
}

/*
 * The time keeper is able to ignore the next wake.
 * Call this if the next wake should no longer be ignored.
 * 
 * @return: None.
 */
void unset_ignore()
{
    ignore_once = false;
}

/*
 * The time keeper is able to ignore the next wake.
 * Check if the next wake should be ignored.
 * No bool as this code may need to interact with C++ code.
 * 
 * @return: 1 if to be ignored, 0 if not.
 */
int get_ignore()
{
    return (int)ignore_once;
}

/*
 * The time keeper is able to ignore the next wake.
 * For the ignore to be correctly updated, this function nedds to be called 
 * repeatedly.
 * 
 * @return: None.
 */
void update_ignore()
{
    /*
     * Keep these cases in mind: 
     * ... |wake| ... |sleep| ...
     * ... |sleep| ... |wake| ...
     */
     long tuw = time_until_wake();
     long tus = time_until_sleep();
     
    /*
     * This should mean sleep.
     * However this is also as sleep:
     * Previous day: ... |sleep| ... |wake| ...
     * Current day: now |wake| ... |sleep| ...
     * For this reason the next wake is ignored, even if the curtain is
     * already opened!
     * 
     * now |wake| ... |sleep| ...
     * now |sleep| ... |wake| ...
     * ... |sleep| now |wake| ...
     * Also ((tuw < 0 && tus < 0) && (tuw < tus))? ... |wake| ... |sleep| now
     */
    if (((tuw > 0 && tus > 0) || (tuw > 0 && tus < 0))
            && ignore_once)
    {
        already_slept = true;
    }
    
    /*
     * This should mean wake.
     * 
     * ... |wake| now |sleep| ...
     * ... |sleep| ... |wake| now
     */
    if (((tuw < 0 && tus > 0) || (((tuw < 0 && tus < 0)) && (tus < tuw)))
        && already_slept && ignore_once)
    {
        already_ignored_wake = true;
    }
    else if (!ignore_once)
    {
        // Reset if ignore_once has been disabled
        already_slept = false;
        already_ignored_wake = false;
        ignore_once = false;
    }
    
    /*
     * The same sleep stuff.
     */
    if (((tuw > 0 && tus > 0) || (tuw > 0 && tus < 0) 
        || ((tuw < 0 && tus < 0) && (tuw < tus)))
        && already_ignored_wake)
    {
        already_slept = false;
        already_ignored_wake = false;
        ignore_once = false;
    }
    //printf("ignore_once %i, already_slept %i, already_ignored_wake %i\n", 
    //        ignore_once, already_slept, already_ignored_wake);
}