#include "time_keeper.h"


/********************************** Includes **********************************/
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
//#include "command_parser.h"


/********************************* Constants *********************************/


/***************************** Struct definitions *****************************/
typedef struct {
    struct tm tm_mon;
    struct tm tm_tue;
    struct tm tm_wed;
    struct tm tm_thu;
    struct tm tm_fri;
    struct tm tm_sat;
    struct tm tm_sun;
} one_tm_per_wd;


/**************************** Prototype functions ****************************/
long only_time_wd_diff(struct tm *tm1, struct tm *tm2);
long only_time_diff(struct tm *tm1, struct tm *tm2);
int set_tm_per_wd(one_tm_per_wd *tmpwd, int wd, int h, int m, int s);
struct tm *get_wdtm(one_tm_per_wd *tmpwd, int wd);
long secs_until_tm_today(one_tm_per_wd *tmpwd);


/**************************** Variable definitions ****************************/
one_tm_per_wd wake_times;
one_tm_per_wd sleep_times;


/**************************** Function definitions ****************************/
int main_1()
{
    time_t rawtime;
    struct tm timeinfo1;
    struct tm timeinfo2;
    
    set_tm_per_wd(&wake_times, 3, 21, 12, 0);
    printf("mon: hour %i, min %i, sec %i \n", wake_times.tm_mon.tm_hour, 
            wake_times.tm_mon.tm_min, wake_times.tm_mon.tm_sec);
    
    time(&rawtime);
    timeinfo1 = *gmtime(&rawtime);
    printf("1: hour %i, min %i, sec %i \n", timeinfo1.tm_hour, timeinfo1.tm_min, timeinfo1.tm_sec);
    
    time(&rawtime);
    timeinfo2 = *gmtime(&rawtime);
    printf("1: hour %i, min %i, sec %i \n", timeinfo1.tm_hour, timeinfo1.tm_min, timeinfo1.tm_sec);
    printf("2: hour %i, min %i, sec %i \n", timeinfo2.tm_hour, timeinfo2.tm_min, timeinfo2.tm_sec);
    
    printf("Own diff: %ld\n", only_time_wd_diff(&timeinfo1, &timeinfo2));
    
    printf("Current wake diff: %ld\n", secs_until_tm_today(&wake_times));

    return 0;
}

/*
int set_wake(uint32_t days, int h, int m, int s)
{
    return 0;
}

int set_times_multiple_days(one_tm_per_wd *time_strct,
                            uint32_t days,
                            int h, int m, int s)
{
    
}
*/

/*
 * Returns the first day in tm struct representation found in multiple days.
 * 
 * 
 */
 /*
int get_tm_day_from_day_type(enum DAY_TYPE dayt)
{
    switch (dayt)
    {
        case MON_T: return 1;
                    break;
    TUE_T = 0x0002,
    WED_T = 0x0004,
    THU_T = 0x0008,
    FRI_T = 0x0010,
    SAT_T = 0x0020,
    SUN_T = 0x0040
    }
}
*/

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
 * @param tmpwd: one_tm_per_wd struct that saves a time for each weekday.
 * @return: Difference in seconds.
 */
long secs_until_tm_today(one_tm_per_wd *tmpwd)
{
    time_t rawtime;
    struct tm currenttime;
    time(&rawtime);
    // Copy by value to not be affected by delays
    currenttime = *gmtime(&rawtime);
    
    struct tm *tm_to_comp = get_wdtm(tmpwd, currenttime.tm_wday); 
    
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
    if (wd < 0 || h > 23)
        return 1;
    if (wd < 0 || wd > 59)
        return 1;
    if (wd < 0 || wd > 59)
        return 1;
            
    // Set the time of the selected weekday
    struct tm *tm_to_set = get_wdtm(tmpwd, wd);
    // TODO May uncomment.
    //tm_to_set->tm_wday = wd;
    tm_to_set->tm_hour = h;
    tm_to_set->tm_min = m;
    tm_to_set->tm_sec = s;
    
    // 0 if successful
    return 0;
}

/*
 * Get a selected weekday from a one_tm_per_wd entity.
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
        default: return &tmpwd->tm_sun;
                break;
    }
}