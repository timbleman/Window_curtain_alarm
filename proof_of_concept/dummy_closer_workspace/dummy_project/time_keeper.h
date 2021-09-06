#ifndef _TIME_KEEPER_H_
#define _TIME_KEEPER_H_

/********************************** Includes **********************************/
#include <stdint.h>
#include "configuration.h"

#ifdef TESTABLE_TK_CODE
#include <time.h>
#endif // TESTABLE_TK_CODE


/********************************* Constants *********************************/
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
int set_wake(uint32_t days, int h, int m, int s);

int set_sleep(uint32_t days, int h, int m, int s);

long time_until_wake();

long time_until_sleep();

int get_current_h();

int get_current_m();

int get_current_s();



#endif // _TIME_KEEPER_H_