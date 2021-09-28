#include "time_keeper.h"


/********************************** Includes **********************************/
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>
#include "command_parser.h"
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
int save_times();
int load_times();


/**************************** Variable definitions ****************************/
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
    
    return status;
}

/*
 * Set the time to wake up for multiple days.
 * Days are onehot encoded as in the DAY_TYPE defined in command_parser.
 * 
 * @param days: The weekdays to set.
 * @param h: The hour to open the curtain.
 * @param m: The minute to open the curtain.
 * @param s: The second to open the curtain.
 * @return: 0 if successful.
 */
int set_wake(uint32_t days, int h, int m, int s)
{
    return set_tm_multiple_days(&wake_times,
                            days, h, m, s);
}

/*
 * Set the time to sleep for multiple days.
 * Days are onehot encoded as in the DAY_TYPE defined in command_parser.
 * 
 * @param days: The weekdays to set.
 * @param h: The hour to close the curtain.
 * @param m: The minute to close the curtain.
 * @param s: The second to close the curtain.
 * @return: 0 if successful.
 */
int set_sleep(uint32_t days, int h, int m, int s)
{
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
                return 1;
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
    // Some validity checks
    if (wd < 0 || wd > 6)
        return 1;
    if (h < 0 || h > 23)
        return 1;
    if (m < 0 || m > 59)
        return 1;
    if (s < 0 || s > 59)
        return 1;
            
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
 * Saves the currently set times into non-volatile memory.
 * For ESP8266: Store in flash. For linux based systems: Some file.
 * 
 * @return: Success status.
 */
int save_times()
{
    return 1;
}

/*
 * Loads wake and sleep times from the memory.
 * 
 * @return: Success status.
 */
int load_times()
{
    return 1;
}
