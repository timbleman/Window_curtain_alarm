/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : Testsuite.h                                                *
 *  Description : Sample testsuite                                           *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */

#include <string.h>
#include <stdio.h>  // printf()
#include <stdbool.h>
#include "System.h"
#include "uCUnit-v1.0.h"
#include "Testsuite.h"
#include "command_parser.h"
#include "time_keeper.h"
#include "configuration.h"

int get_bit_of_error(uint32_t err_code);

#ifdef TESTABLE_PARSER_CODE
static void Test_parse_hour(void)
{
    UCUNIT_TestcaseBegin("DEMO: Checking parsing of hours from user input.");

    int argc = 0;
    const int ARGV_LEN = 20;
    char *argv[20] = {0};
    
    // Regular parsing
    char test_str[50] = "set_wake -d mon,tue -h 22 -m 30 -s 10 \n";
    // Intermediate representation, normally created internally
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    printf("argv in testsuite ptr %p \n", argv);
    UCUNIT_CheckIsEqual( 22, parse_hour(argv, argc));

    // Parsing zero without error
    strcpy(test_str, "set_wake -d mon,tue -h 0 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( 0, parse_hour(argv, argc));
    strcpy(test_str, "set_wake -d mon,tue -h 00 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( 0, parse_hour(argv, argc));
    
    //Invalid options
    int invalid_t_err_pos = get_bit_of_error(INVALID_TIME_ERR);
    printf("invalid_t_err_pos %i \n", invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -h 000 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet( parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -h jks -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet( parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -h -1 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet( parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -h 24 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet( parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -b 24 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet( parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet( parse_hour(argv, argc), invalid_t_err_pos);
    
    /*strcpy(test_str, "set_wake -d mon,tue -h 00 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( 0, parse_hour(argv, argc));*/
}

static void Test_parse_command(void)
{
    UCUNIT_TestcaseBegin("DEMO: Checking parsing user input.");

    int argc = 0;
    const int ARGV_LEN = 20;
    char *argv[20] = {0};
    
    // Regular parsing
    char test_str[50] = "set_wake -d mon,tue -h 22 -m 30 -s 10 \n";
    // Intermediate representation, normally created internally
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( WAKE_SET_T, parse_action(argv, argc));

    strcpy(test_str, "set_sleep -d mon,tue -h 22 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( SLEEP_SET_T, parse_action(argv, argc));
    
    strcpy(test_str, "invalid_command -d mon,tue -h 22 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( NONE_T, parse_action(argv, argc));
    
    strcpy(test_str, "open \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( CURTAIN_CONTROL_T, parse_action(argv, argc));
    
    strcpy(test_str, "close \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( CURTAIN_CONTROL_T, parse_action(argv, argc));
    
    strcpy(test_str, "");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( NONE_T, parse_action(argv, argc));
}

static void Test_get_action()
{
    UCUNIT_TestcaseBegin("DEMO: Checking basic action generation for set_sleep.");

    user_action_t new_act;
    
    // Open
    char test_str[50] = "open \n";
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual( OPEN_T, new_act.data[0]);
    
    // Close
    strcpy(test_str, "close \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual( CLOSE_T, new_act.data[0]);
    printf("\n new_act.data[0] %u \n", new_act.data[0]);
    
    // Times
    strcpy(test_str, "set_sleep -d mon,tue -h 22 -m 30 -s 10 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual( MON_T | TUE_T, new_act.data[0]);
    UCUNIT_CheckIsEqual( 22, new_act.data[1]);
    UCUNIT_CheckIsEqual( 30, new_act.data[2]);
    UCUNIT_CheckIsEqual( 10, new_act.data[3]);

    // Invalid time stuff
    int invalid_t_err_pos = get_bit_of_error(INVALID_TIME_ERR);
    strcpy(test_str, "set_sleep -d mon,tue -h 222 -m 30 -s 10 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsBitSet( new_act.data[0], invalid_t_err_pos);
    strcpy(test_str, "set_sleep -d mon,tue -h 22 -m 300 -s 10 \n");
    new_act = get_action(test_str);
    //printf("new_act.data[0] %u \n", (new_act.data[0]));
    //printf("\n new act data get_time_action %u %u %u %u \n", new_act.data[0], new_act.data[1], new_act.data[2], new_act.data[3]);
    UCUNIT_CheckIsBitSet( new_act.data[0], invalid_t_err_pos);
}
#endif // TESTABLE_PARSER_CODE

#ifdef TESTABLE_TK_CODE
static void Test_set_wake_single_day()
{
    UCUNIT_TestcaseBegin("DEMO: Checking set_wake() for single days.");
    /*
     * Check the basic setup
     */
    setup_time_keeper();
    
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_min, 0);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_sec, 0);
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_wday, 6);
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_hour, DEFAULT_WEEKEND_WAKE);
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_min, 0);
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_sec, 0);
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_wday, 0);
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_hour, DEFAULT_WEEKEND_WAKE);
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_min, 0);
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_sec, 0);
    
    
    /*
     * Try setting single days
     */
    set_wake(MON_T, 1, 2, 3);
    set_wake(TUE_T, 4, 5, 6);
    set_wake(WED_T, 7, 8, 9);
    set_wake(THU_T, 10, 11, 12);
    set_wake(FRI_T, 13, 14, 15);
    set_wake(SAT_T, 16, 17, 18);
    set_wake(SUN_T, 23, 59, 59);
    
    // Check whether times are set correctly
    // Mon
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_hour, 1);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_min, 2);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_sec, 3);
    printf("mon wd h m s: %i %i %i %i \n", wake_times.tm_mon.tm_wday, 
            wake_times.tm_mon.tm_hour, wake_times.tm_mon.tm_min, 
            wake_times.tm_mon.tm_sec);
    // Tue
    UCUNIT_CheckIsEqual( wake_times.tm_tue.tm_wday, 2);
    UCUNIT_CheckIsEqual( wake_times.tm_tue.tm_hour, 4);
    UCUNIT_CheckIsEqual( wake_times.tm_tue.tm_min, 5);
    UCUNIT_CheckIsEqual( wake_times.tm_tue.tm_sec, 6);   
    // Wed
    UCUNIT_CheckIsEqual( wake_times.tm_wed.tm_wday, 3);
    UCUNIT_CheckIsEqual( wake_times.tm_wed.tm_hour, 7);
    UCUNIT_CheckIsEqual( wake_times.tm_wed.tm_min, 8);
    UCUNIT_CheckIsEqual( wake_times.tm_wed.tm_sec, 9);    
    // Thu
    UCUNIT_CheckIsEqual( wake_times.tm_thu.tm_wday, 4);
    UCUNIT_CheckIsEqual( wake_times.tm_thu.tm_hour, 10);
    UCUNIT_CheckIsEqual( wake_times.tm_thu.tm_min, 11);
    UCUNIT_CheckIsEqual( wake_times.tm_thu.tm_sec, 12);     
    // Fri
    UCUNIT_CheckIsEqual( wake_times.tm_fri.tm_wday, 5);
    UCUNIT_CheckIsEqual( wake_times.tm_fri.tm_hour, 13);
    UCUNIT_CheckIsEqual( wake_times.tm_fri.tm_min, 14);
    UCUNIT_CheckIsEqual( wake_times.tm_fri.tm_sec, 15);     
    // Sat
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_wday, 6);
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_hour, 16);
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_min, 17);
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_sec, 18);     
    // Sun
    // Also check upper bounds
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_wday, 0);
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_hour, 23);
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_min, 59);
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_sec, 59);

    // Reset
    setup_time_keeper();   
}

static void Test_set_wake_invalid()
{
    UCUNIT_TestcaseBegin("DEMO: Checking set_wake() for invalid inputs.");
    // Setup
    setup_time_keeper();
    
    // Invalid day
    UCUNIT_CheckIsInRange( set_wake(0, 1, 2, 3), 1, UINT32_MAX);
    UCUNIT_CheckIsInRange( set_wake(0x0080, 1, 2, 3), 1, UINT32_MAX);
    
    // Invalid hour
    UCUNIT_CheckIsInRange( set_wake(MON_T, 24, 2, 3), 1, UINT32_MAX);
    UCUNIT_CheckIsInRange( set_wake(MON_T, -1, 2, 3), 1, UINT32_MAX);
    
    // Invalid minute
    UCUNIT_CheckIsInRange( set_wake(MON_T, 1, 60, 3), 1, UINT32_MAX);
    UCUNIT_CheckIsInRange( set_wake(MON_T, 1, -1, 3), 1, UINT32_MAX);
    
    // Invalid second
    UCUNIT_CheckIsInRange( set_wake(MON_T, 1, 2, 60), 1, UINT32_MAX);
    UCUNIT_CheckIsInRange( set_wake(MON_T, 1, 2, -1), 1, UINT32_MAX);
    
    // Check that monday times have not been changed due to invalid input.
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_min, 0);
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_sec, 0);
    
    // Reset
    setup_time_keeper();     
}

static void Test_set_wake_multiple_days()
{
    UCUNIT_TestcaseBegin("DEMO: Checking set_wake() for multiple days at once.");
    // Setup
    setup_time_keeper();
    
    set_wake(MON_T | TUE_T | SAT_T, 11, 2, 3);
    
    UCUNIT_CheckIsEqual( wake_times.tm_mon.tm_hour, 11);
    UCUNIT_CheckIsEqual( wake_times.tm_tue.tm_hour, 11);
    UCUNIT_CheckIsEqual( wake_times.tm_wed.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual( wake_times.tm_thu.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual( wake_times.tm_fri.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual( wake_times.tm_sat.tm_hour, 11);
    UCUNIT_CheckIsEqual( wake_times.tm_sun.tm_hour, DEFAULT_WEEKEND_WAKE);
    
    // Reset
    setup_time_keeper();    
}

static void Test_set_sleep_single_day()
{
    UCUNIT_TestcaseBegin("DEMO: Checking set_sleep().");
    /*
     * Check the basic setup
     */
    setup_time_keeper();
    
    UCUNIT_CheckIsEqual( sleep_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual( sleep_times.tm_mon.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual( sleep_times.tm_mon.tm_min, 0);
    UCUNIT_CheckIsEqual( sleep_times.tm_mon.tm_sec, 0);
    UCUNIT_CheckIsEqual( sleep_times.tm_sat.tm_wday, 6);
    UCUNIT_CheckIsEqual( sleep_times.tm_sat.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual( sleep_times.tm_sat.tm_min, 0);
    UCUNIT_CheckIsEqual( sleep_times.tm_sat.tm_sec, 0);
    UCUNIT_CheckIsEqual( sleep_times.tm_sun.tm_wday, 0);
    UCUNIT_CheckIsEqual( sleep_times.tm_sun.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual( sleep_times.tm_sun.tm_min, 0);
    UCUNIT_CheckIsEqual( sleep_times.tm_sun.tm_sec, 0);
    
    
    /*
     * Try setting single days
     */
    set_sleep(MON_T, 11, 22, 33);
    
    // Check whether times are set correctly
    // Mon
    UCUNIT_CheckIsEqual( sleep_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual( sleep_times.tm_mon.tm_hour, 11);
    UCUNIT_CheckIsEqual( sleep_times.tm_mon.tm_min, 22);
    UCUNIT_CheckIsEqual( sleep_times.tm_mon.tm_sec, 33);
    
    // Reset
    setup_time_keeper();
}

static void Test_time_until_wake_today()
{
    UCUNIT_TestcaseBegin("Checking time_until_wake() for current time.");
    // Setup
    setup_time_keeper();
    
    bool min_prev = get_current_m() > 30;
    
    const uint32_t ALL_DAYS = MON_T | TUE_T | WED_T | THU_T | FRI_T 
                                | SAT_T | SUN_T;
    const int MIN_OFFSET = 25;
    const int TARGET_DIFF_SECS = MIN_OFFSET * 60;
    
    // Set a wake_time not far from now and check whether the seconds match.
    int NEW_MIN = 0;
    if (min_prev)
    {
        NEW_MIN = get_current_m() - MIN_OFFSET;
        set_wake(ALL_DAYS,  get_current_h(),
                            NEW_MIN,
                            get_current_s());
        UCUNIT_CheckIsInRange( time_until_wake(), TARGET_DIFF_SECS - 1, 
                                                TARGET_DIFF_SECS + 1);
    }
    else
    {
        NEW_MIN = get_current_m() + MIN_OFFSET;
        set_wake(ALL_DAYS,  get_current_h(),
                            NEW_MIN,
                            get_current_s());
        UCUNIT_CheckIsInRange( time_until_wake(), TARGET_DIFF_SECS - 1, 
                                                TARGET_DIFF_SECS + 1);     
    }
    
    // Reset
    setup_time_keeper();
}
#endif // TESTABLE_TK_CODE

int get_bit_of_error(uint32_t err_code)
{
    int i;
    uint32_t mask = 1;
    for (i = 0; i < 32; i++)
    {
        if ((err_code & (mask << i)) != 0)
            break;
    }
    return i;
}

/*
static void Test_BasicChecksDemo(void)
{
    char * s = NULL;
    int a = 0;
    int b = 0;
    int c = 0;

    UCUNIT_TestcaseBegin("DEMO:Simple Checks");
    UCUNIT_CheckIsEqual( 0, a ); // Pass
    UCUNIT_CheckIsEqual( 0, b ); // Pass
    UCUNIT_CheckIsEqual( 0, c ); // Pass

    UCUNIT_CheckIsEqual( 0, (1-1) ); // Pass
    UCUNIT_CheckIsEqual( 0, (-1+1) ); // Pass
    UCUNIT_CheckIsEqual( -1, (-2+1) ); // Pass

    UCUNIT_CheckIsNull(s); // Pass
    UCUNIT_TestcaseEnd();
}

static void Test_PointersDemo(void)
{
    char * s = NULL;

    UCUNIT_TestcaseBegin("DEMO:Checking pointers");

    UCUNIT_CheckIsNull(s); // Pass

    // Empty string has a 0 byte/word as end of string
    s = "";

    UCUNIT_CheckIsNotNull(s); // Pass
    UCUNIT_Check( (*s)=='\00' , "No end-of-string found","s" ); // Pass

    // Check if first character of string "Hello World!\n" is a 'H'
    s = "Hello World!\n";
    UCUNIT_CheckIsNotNull(s); // Pass
    UCUNIT_Check( (*s)=='H' , "String starts not with 'H'","s" ); // Pass

    UCUNIT_TestcaseEnd(); // Pass
}

static void Test_ChecklistDemo(void)
{
    int a = 0;
    int b = 0;
    int c = 0;

    UCUNIT_TestcaseBegin("DEMO:Checklists");

    // Check if calculation works
    a = 10;
    b = -20;
    c = a + b;

    UCUNIT_ChecklistBegin(UCUNIT_ACTION_WARNING);
    UCUNIT_CheckIsEqual( -10, c ); // Pass
    UCUNIT_CheckIsInRange(a,0,10); // Pass
    UCUNIT_CheckIsInRange(a,11,20); // Fail
    UCUNIT_CheckIsInRange(a,0,9); // Fail
    UCUNIT_ChecklistEnd();

    UCUNIT_TestcaseEnd(); // Fail
}

static void Test_BitChecksDemo(void)
{
    UCUNIT_TestcaseBegin("DEMO:Checking Bits");

    UCUNIT_CheckIsBitSet(0x0001, 0); // Pass
    UCUNIT_CheckIsBitSet(0x0000, 0); // Fail
    UCUNIT_CheckIsBitSet(0x0002, 1); // Pass
    UCUNIT_CheckIsBitSet(0x0000, 6); // Fail
    UCUNIT_CheckIsBitSet(0xFFFF, 6); // Pass

    UCUNIT_CheckIsBitClear(0x0001, 0); // Fail
    UCUNIT_CheckIsBitClear(0x0001, 1); // Pass

    UCUNIT_TestcaseEnd(); // Fail
}

static void Test_CheckTracepointsDemo(void)
{

    int found = FALSE;
    int i = 0;
    unsigned short a = 0;

    UCUNIT_TestcaseBegin("DEMO:Tracepoints");

    a = 0x1234;
    UCUNIT_ResetTracepointCoverage();
    UCUNIT_Tracepoint(0); // Pass
    UCUNIT_CheckIs8Bit(a); // Fail
    UCUNIT_CheckIs16Bit(a); // Pass
    UCUNIT_CheckIs32Bit(a); // Pass
    UCUNIT_Tracepoint(1); // Pass

    UCUNIT_CheckTracepointCoverage(0); // Pass
    UCUNIT_CheckTracepointCoverage(1); // Pass

    UCUNIT_ResetTracepointCoverage();
    for (i = 0; (i < 50) && (!found); i++)
    {
        if (i == 25)
        {
            UCUNIT_Tracepoint(0); // Never executed
            break;
        }

        if (i == 10)
        {
            UCUNIT_Tracepoint(1); // Executed
            found = TRUE;
        }

        if (i == 15)
        {
            UCUNIT_Tracepoint(2); // Never executed
            found = TRUE;
        }
    }

    UCUNIT_CheckTracepointCoverage(0); // Fail
    UCUNIT_CheckTracepointCoverage(1); // Pass
    UCUNIT_CheckTracepointCoverage(2); // Fail

    UCUNIT_TestcaseEnd(); // Fail 
}
*/

void Testsuite_RunTests(void)
{
#ifdef TESTABLE_PARSER_CODE
    Test_parse_hour();
    Test_parse_command();
    Test_get_action();
#endif // TESTABLE_PARSER_CODE
    
#ifdef TESTABLE_TK_CODE
    Test_set_wake_single_day();
    Test_set_wake_multiple_days();
    Test_set_wake_invalid();
    Test_set_sleep_single_day();
    Test_time_until_wake_today();
#endif // TESTABLE_TK_CODE

    UCUNIT_WriteSummary();
}

#ifdef UNITTESTS_INSTEAD_OF_MAIN
int main(void)
#else
int main_ucunit(void)
#endif // UNITTESTS_INSTEAD_OF_MAIN
{
    UCUNIT_Init();
    UCUNIT_WriteString("\n**************************************");
    UCUNIT_WriteString("\nName:     ");
    UCUNIT_WriteString("uCUnit demo application");
    UCUNIT_WriteString("\nCompiled: ");
    UCUNIT_WriteString(__DATE__);
    UCUNIT_WriteString("\nTime:     ");
    UCUNIT_WriteString(__TIME__);
    UCUNIT_WriteString("\nVersion:  ");
    UCUNIT_WriteString(UCUNIT_VERSION);
    UCUNIT_WriteString("\n**************************************");
    Testsuite_RunTests();
    UCUNIT_Shutdown();

    return 0;
}