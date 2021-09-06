#ifndef _TIME_KEEPER_H_
#define _TIME_KEEPER_H_

/********************************** Includes **********************************/
#include <stdint.h>
#include "configuration.h"

/********************************* Constants *********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions ****************************/
int set_wake(uint32_t days, int h, int m, int s);

int set_sleep(uint32_t days, int h, int m, int s);

long time_until_wake();

long time_until_sleep();

int get_current_h();

int get_current_m();

int get_current_s();

#ifdef TESTABLE_TK_CODE
#endif // TESTABLE_TK_CODE

#endif // _TIME_KEEPER_H_