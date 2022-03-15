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
/********************************** Includes **********************************/
#include <unistd.h> // sleep()
#include <string.h>
#include <stdio.h>  // printf()
#include <stdbool.h>
#include "System.h"
#include "uCUnit-v1.0.h"
#include "Testsuite.h"
#include "command_parser.h"
#include "time_keeper.h"
#include "motor_controller.h"
#include "action_executer.h"
#include "alarm_checker.h"
#include "data_storage.h"
#include "configuration.h"


/********************************* Constants **********************************/
#define MESSAGE_LEN 512
#undef RUN_TIME_INTENSIVE_TESTS


/**************************** Prototype functions *****************************/
int setup_files();
int get_bit_of_error(uint32_t err_code);
int time_plus_secs(int secs, int *h, int *m, int *s);


/**************************** Variable definitions ****************************/
/**************************** Function definitions ****************************/
#ifdef TESTABLE_PARSER_CODE
static void Test_parse_hour(void)
{
    UCUNIT_TestcaseBegin("Checking parsing of hours from user input.");

    int argc = 0;
    const int ARGV_LEN = 20;
    char *argv[20] = {0};
    
    // Regular parsing
    char test_str[50] = "set_wake -d mon,tue -h 22 -m 30 -s 10 \n";
    // Intermediate representation, normally created internally
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    printf("argv in testsuite ptr %p \n", argv);
    UCUNIT_CheckIsEqual(parse_hour(argv, argc), 22);

    // Parsing zero without error
    strcpy(test_str, "set_wake -d mon,tue -h 0 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_hour(argv, argc), 0);
    strcpy(test_str, "set_wake -d mon,tue -h 00 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_hour(argv, argc), 0);
    
    //Invalid options
    int invalid_t_err_pos = get_bit_of_error(INVALID_TIME_ERR);
    printf("invalid_t_err_pos %i \n", invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -h 000 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet(parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -h jks -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet(parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -h -1 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet(parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -h 24 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet(parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "set_wake -d mon,tue -b 24 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet(parse_hour(argv, argc), invalid_t_err_pos);
    strcpy(test_str, "");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsBitSet(parse_hour(argv, argc), invalid_t_err_pos);
    
    /*strcpy(test_str, "set_wake -d mon,tue -h 00 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual( 0, parse_hour(argv, argc));*/
    
    UCUNIT_TestcaseEnd();
}

static void Test_parse_command(void)
{
    UCUNIT_TestcaseBegin("Checking parsing user input.");

    int argc = 0;
    const int ARGV_LEN = 20;
    char *argv[20] = {0};

    setup_files();
    
    // Regular parsing
    char test_str[50] = "set_wake -d mon,tue -h 22 -m 30 -s 10 \n";
    // Intermediate representation, normally created internally
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_action(argv, argc), WAKE_SET_T);

    strcpy(test_str, "set_sleep -d mon,tue -h 22 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_action(argv, argc), SLEEP_SET_T);
    
    strcpy(test_str, "invalid_command -d mon,tue -h 22 -m 30 -s 10 \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_action(argv, argc), NONE_T);
    
    strcpy(test_str, "open \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_action(argv, argc), CURTAIN_CONTROL_T);
    
    strcpy(test_str, "close \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_action(argv, argc), CURTAIN_CONTROL_T);
    
    strcpy(test_str, "help \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_action(argv, argc), HELP_T);
    
    strcpy(test_str, "curtime \n");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_action(argv, argc), CURTIME_T);
    
    strcpy(test_str, "");
    tokenise_to_argc_argv(test_str, &argc, argv, ARGV_LEN);
    UCUNIT_CheckIsEqual(parse_action(argv, argc), NONE_T);
    
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_get_action()
{
    UCUNIT_TestcaseBegin("DEMO: Checking basic action generation for set_sleep.");

    setup_files();

    user_action_t new_act;
    
    // Open
    char test_str[50] = "open \n";
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual(new_act.act_type, CURTAIN_CONTROL_T);
    UCUNIT_CheckIsEqual(new_act.data[0], OPEN_T);
    
    // Close
    strcpy(test_str, "close \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual(new_act.act_type, CURTAIN_CONTROL_T);
    UCUNIT_CheckIsEqual(new_act.data[0], CLOSE_T);
    printf("\n new_act.data[0] %u \n", new_act.data[0]);
    
    // Close without space
    strcpy(test_str, "close\n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual(new_act.act_type, CURTAIN_CONTROL_T);
    printf("new_act.act_type actual %X, expected %X  \n", new_act.act_type, CURTAIN_CONTROL_T);
    UCUNIT_CheckIsEqual(new_act.data[0], CLOSE_T);
    printf("\n new_act.data[0] %u \n", new_act.data[0]);
    
    // Times
    strcpy(test_str, "set_sleep -d mon,tue -h 22 -m 30 -s 10 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual(new_act.data[0], MON_T | TUE_T);
    UCUNIT_CheckIsEqual(new_act.data[1], 22);
    UCUNIT_CheckIsEqual(new_act.data[2], 30);
    UCUNIT_CheckIsEqual(new_act.data[3], 10);
    
    // Times with zeros
    strcpy(test_str, "set_sleep -d mon,tue -h 02 -m 05 -s 00 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual(new_act.data[0], MON_T | TUE_T);
    UCUNIT_CheckIsEqual(new_act.data[1], 2);
    UCUNIT_CheckIsEqual(new_act.data[2], 5);
    UCUNIT_CheckIsEqual(new_act.data[3], 0);

    // Invalid time stuff
    int invalid_t_err_pos = get_bit_of_error(INVALID_TIME_ERR);
    strcpy(test_str, "set_sleep -d mon,tue -h 222 -m 30 -s 10 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsBitSet(new_act.data[0], invalid_t_err_pos);
    strcpy(test_str, "set_sleep -d mon,tue -h 22 -m 300 -s 10 \n");
    new_act = get_action(test_str);
    //printf("new_act.data[0] %u \n", (new_act.data[0]));
    //printf("\n new act data get_time_action %u %u %u %u \n", new_act.data[0], new_act.data[1], new_act.data[2], new_act.data[3]);
    UCUNIT_CheckIsBitSet(new_act.data[0], invalid_t_err_pos);
    
    // Invalid day: No day present
    int invalid_d_err_pos = get_bit_of_error(INVALID_DAY_ERR);
    strcpy(test_str, "set_sleep -h 222 -m 30 -s 10 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsBitSet(new_act.data[0], invalid_d_err_pos);
    
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_get_action_set_incomplete_times(void)
{
    UCUNIT_TestcaseBegin("Checking parsing incomplete times set by user.");

    setup_files();
    
    user_action_t new_act;
    
    // Incomplete times 
    char test_str[50] = "set_sleep -d mon,tue -h 22 -m 30 \n";
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual(new_act.data[0], MON_T | TUE_T);
    printf("\n\r\nactual %X expected %X\n\n\r", new_act.data[0], MON_T | TUE_T);
    UCUNIT_CheckIsEqual(new_act.data[1], 22);
    UCUNIT_CheckIsEqual(new_act.data[2], 30);
    UCUNIT_CheckIsEqual(new_act.data[3], 0);
    printf("Actual day %X \n", new_act.data[0]);
    printf("Actual set time %i:%i:%i \n", new_act.data[1], new_act.data[2], new_act.data[3]);
    
    // Times with zeros
    strcpy(test_str, "set_sleep -d mon,tue -h 02 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual(new_act.data[0], MON_T | TUE_T);
    UCUNIT_CheckIsEqual(new_act.data[1], 2);
    UCUNIT_CheckIsEqual(new_act.data[2], 0);
    UCUNIT_CheckIsEqual(new_act.data[3], 0);
    printf("Actual day %X \n", new_act.data[0]);
    printf("Actual set time %i:%i:%i \n", new_act.data[1], new_act.data[2], new_act.data[3]);
    
    // Week and weekend
    strcpy(test_str, "set_sleep -d week,weekend -h 2 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsEqual(new_act.data[0],
            MON_T | TUE_T | WED_T | THU_T | FRI_T | SAT_T | SUN_T);

    // Invalid time stuff
    int invalid_t_err_pos = get_bit_of_error(INVALID_TIME_ERR);
    strcpy(test_str, "set_sleep -d mon,tue -m 30 -s 10 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsBitSet(new_act.data[0], invalid_t_err_pos);
    
    // Invalid day stuff
    // Only one invalid day
    int invalid_d_err_pos = get_bit_of_error(INVALID_DAY_ERR);
    strcpy(test_str, "set_sleep -d bri -h 11 -m 10 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsBitSet(new_act.data[0], invalid_d_err_pos);
    // One invalid day among valid ones
    strcpy(test_str, "set_sleep -d mon,bri -h 11 -m 10 \n");
    new_act = get_action(test_str);
    UCUNIT_CheckIsBitSet(new_act.data[0], invalid_d_err_pos);
    
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_get_message_from_errors()
{
    UCUNIT_TestcaseBegin("Checking getting message from error codes.");
    
    const int BUF_LEN = 32;
    char buf[32] = {0};
    
    get_message_from_errors(INVALID_DAY_ERR, buf, BUF_LEN);
    UCUNIT_CheckIsEqual(strcmp(buf, "DAY_ERR"), 0);
    
    memset(buf, 0, BUF_LEN);
    get_message_from_errors(INVALID_TIME_ERR, buf, BUF_LEN);
    UCUNIT_CheckIsEqual(strcmp(buf, "TIME_ERR"), 0);
    
    memset(buf, 0, BUF_LEN);
    get_message_from_errors(INVALID_COMMAND_ERR, buf, BUF_LEN);
    UCUNIT_CheckIsEqual(strcmp(buf, "COMMAND_ERR"), 0);
    
    memset(buf, 0, BUF_LEN);
    get_message_from_errors(ALL_ERRS, buf, BUF_LEN);
    printf("all_errs buf %s\n", buf);
    UCUNIT_CheckIsEqual(strcmp(buf, "DAY_ERR, TIME_ERR, COMMAND_ERR"), 0);
    
    // Too small buffer
    memset(buf, 0, BUF_LEN);
    UCUNIT_CheckIsEqual(get_message_from_errors(ALL_ERRS, buf, 5), 1);
    UCUNIT_CheckIsEqual(strcmp(buf, "SLEN"), 0);
    
    UCUNIT_TestcaseEnd();
}
#endif // TESTABLE_PARSER_CODE

#ifdef TESTABLE_TK_CODE
static void Test_set_wake_single_day()
{
    UCUNIT_TestcaseBegin("DEMO: Checking set_wake() for single days.");
    /*
     * Check the basic setup
     */
    setup_files();
    
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_min, 0);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_sec, 0);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_wday, 6);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_hour, DEFAULT_WEEKEND_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_min, 0);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_sec, 0);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_wday, 0);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_hour, DEFAULT_WEEKEND_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_min, 0);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_sec, 0);
    
    
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
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, 1);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_min, 2);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_sec, 3);
    printf("mon wd h m s: %i %i %i %i \n", wake_times.tm_mon.tm_wday, 
            wake_times.tm_mon.tm_hour, wake_times.tm_mon.tm_min, 
            wake_times.tm_mon.tm_sec);
    // Tue
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_wday, 2);
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_hour, 4);
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_min, 5);
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_sec, 6);   
    // Wed
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_wday, 3);
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_hour, 7);
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_min, 8);
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_sec, 9);    
    // Thu
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_wday, 4);
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_hour, 10);
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_min, 11);
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_sec, 12);     
    // Fri
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_wday, 5);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_hour, 13);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_min, 14);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_sec, 15);     
    // Sat
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_wday, 6);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_hour, 16);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_min, 17);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_sec, 18);     
    // Sun
    // Also check upper bounds
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_wday, 0);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_hour, 23);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_min, 59);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_sec, 59);
    
    // Reset
    setup_files();  
    
    UCUNIT_TestcaseEnd();
}

static void Test_set_wake_invalid()
{
    UCUNIT_TestcaseBegin("DEMO: Checking set_wake() for invalid inputs.");
    // Setup
    setup_files();
    
    // Invalid day
    UCUNIT_CheckIsInRange(set_wake(0, 1, 2, 3), 1, UINT32_MAX);
    UCUNIT_CheckIsInRange(set_wake(0x0080, 1, 2, 3), 1, UINT32_MAX);
    
    // Invalid hour
    UCUNIT_CheckIsInRange(set_wake(MON_T, 24, 2, 3), 1, UINT32_MAX);
    UCUNIT_CheckIsInRange(set_wake(MON_T, -1, 2, 3), 1, UINT32_MAX);
    
    // Invalid minute
    UCUNIT_CheckIsInRange(set_wake(MON_T, 1, 60, 3), 1, UINT32_MAX);
    UCUNIT_CheckIsInRange(set_wake(MON_T, 1, -1, 3), 1, UINT32_MAX);
    
    // Invalid second
    UCUNIT_CheckIsInRange(set_wake(MON_T, 1, 2, 60), 1, UINT32_MAX);
    UCUNIT_CheckIsInRange(set_wake(MON_T, 1, 2, -1), 1, UINT32_MAX);
    
    // Check that monday times have not been changed due to invalid input.
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_min, 0);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_sec, 0);
    
    // Reset
    setup_files();    

    UCUNIT_TestcaseEnd(); 
}

static void Test_set_wake_multiple_days()
{
    UCUNIT_TestcaseBegin("DEMO: Checking set_wake() for multiple days at once.");
    // Setup
    setup_files();
    
    set_wake(MON_T | TUE_T | SAT_T, 11, 2, 3);
    
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, 11);
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_hour, 11);
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_hour, 11);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_hour, DEFAULT_WEEKEND_WAKE);
    
    // Reset
    setup_files();

    UCUNIT_TestcaseEnd();  
}

static void Test_set_sleep_single_day()
{
    UCUNIT_TestcaseBegin("DEMO: Checking set_sleep().");
    /*
     * Check the basic setup
     */
    setup_files();
    
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_min, 0);
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_sec, 0);
    UCUNIT_CheckIsEqual(sleep_times.tm_sat.tm_wday, 6);
    UCUNIT_CheckIsEqual(sleep_times.tm_sat.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual(sleep_times.tm_sat.tm_min, 0);
    UCUNIT_CheckIsEqual(sleep_times.tm_sat.tm_sec, 0);
    UCUNIT_CheckIsEqual(sleep_times.tm_sun.tm_wday, 0);
    UCUNIT_CheckIsEqual(sleep_times.tm_sun.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual(sleep_times.tm_sun.tm_min, 0);
    UCUNIT_CheckIsEqual(sleep_times.tm_sun.tm_sec, 0);
    
    
    /*
     * Try setting single days
     */
    set_sleep(MON_T, 11, 22, 33);
    
    // Check whether times are set correctly
    // Mon
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_wday, 1);
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_hour, 11);
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_min, 22);
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_sec, 33);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_time_until_wake_today()
{
    UCUNIT_TestcaseBegin("Checking time_until_wake() for current time.");
    // Setup
    setup_files();
    
    bool min_prev = get_current_m() > 30;
    
    const uint32_t ALL_DAYS = MON_T | TUE_T | WED_T | THU_T | FRI_T 
                                | SAT_T | SUN_T;
    const int MIN_OFFSET = 25;
    const int TARGET_DIFF_SECS = MIN_OFFSET * 60;
    
    // Set a wake_time not far from now and check whether the seconds match.
    int NEW_MIN = 0;
    long time_till_wake = 0;
    if (min_prev)
    {
        NEW_MIN = get_current_m() - MIN_OFFSET;
        set_wake(ALL_DAYS,  get_current_h(),
                            NEW_MIN,
                            get_current_s());
        time_till_wake = time_until_wake();
        UCUNIT_CheckIsInRange(time_till_wake, -TARGET_DIFF_SECS - 1, 
                                                -TARGET_DIFF_SECS + 1);
    }
    else
    {
        NEW_MIN = get_current_m() + MIN_OFFSET;
        set_wake(ALL_DAYS,  get_current_h(),
                            NEW_MIN,
                            get_current_s());
        time_till_wake = time_until_wake();
        UCUNIT_CheckIsInRange(time_till_wake, TARGET_DIFF_SECS - 1, 
                                                TARGET_DIFF_SECS + 1);     
    }
    
    //printf("time_till_wake %ld target_diff %i \n", time_till_wake, TARGET_DIFF_SECS);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_store_time()
{
    UCUNIT_TestcaseBegin("Checking storing times.");
    
    // Setup
    setup_files();
    
    set_wake(MON_T | TUE_T | WED_T |THU_T | SAT_T, 7, 7, 7);
    set_wake(FRI_T | SUN_T, 9, 9, 9);
    set_sleep(MON_T | TUE_T | WED_T |THU_T | FRI_T | SAT_T | SUN_T, 14, 15, 16);
    
    save_times();
    
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, 7);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_min, 7);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_sec, 7);
    
    set_wake(MON_T | TUE_T | WED_T |THU_T | FRI_T | SAT_T | SUN_T, 3, 3, 3);
    set_sleep(MON_T | TUE_T | WED_T |THU_T | FRI_T | SAT_T | SUN_T, 4, 4, 4);
    
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_min, 3);

    // Check what has been set
    load_times();
    /*
    char message_buf[512] = {0};
    write_wake_times_message(message_buf, 512);
    printf("wake times %s \n\r", message_buf);
    write_sleep_times_message(message_buf, 512);
    printf("sleep times %s \n\r", message_buf);
    */

    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, 7);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_min, 7);
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_sec, 7);
    
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_hour, 9);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_min, 9);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_sec, 9);

    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_hour, 14);
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_min, 15);
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_sec, 16);

    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_write_times_message()
{
    UCUNIT_TestcaseBegin("Checking time_until_wake() for current time.");
    // Setup
    setup_files();
    
    char message[10] = {0};
    
    int status = format_time_to_str(message, 10, 1, 2, 3);
    UCUNIT_CheckIsEqual(status, 0);
    UCUNIT_CheckIsEqual(strcmp(message, "01:02:03"), 0);
    
    memset(message, 0, 10);
    status = format_time_to_str(message, 10, 11, 22, 33);
    UCUNIT_CheckIsEqual(status, 0);
    UCUNIT_CheckIsEqual(strcmp(message, "11:22:33"), 0);
    
    // Invalid stuff
    memset(message, 0, 10);
    status = format_time_to_str(message, 10, 110, 22, 33);
    UCUNIT_CheckIsEqual(status, 1);
    UCUNIT_CheckIsEqual(strcmp(message, "-1:-1:-1"), 0);
    memset(message, 0, 10);
    status = format_time_to_str(message, 10, 11, 220, 33);
    UCUNIT_CheckIsEqual(status, 1);
    UCUNIT_CheckIsEqual(strcmp(message, "-1:-1:-1"), 0);
    memset(message, 0, 10);
    status = format_time_to_str(message, 10, 11, 22, 330);
    UCUNIT_CheckIsEqual(status, 1);
    UCUNIT_CheckIsEqual(strcmp(message, "-1:-1:-1"), 0);
    memset(message, 0, 10);
    status = format_time_to_str(message, 10, -1, 22, 33);
    UCUNIT_CheckIsEqual(status, 1);
    UCUNIT_CheckIsEqual(strcmp(message, "-1:-1:-1"), 0);
    memset(message, 0, 10);
    status = format_time_to_str(message, 10, 11, -1, 33);
    UCUNIT_CheckIsEqual(status, 1);
    UCUNIT_CheckIsEqual(strcmp(message, "-1:-1:-1"), 0);
    memset(message, 0, 10);
    status = format_time_to_str(message, 10, 11, 22, -1);
    UCUNIT_CheckIsEqual(status, 1);
    UCUNIT_CheckIsEqual(strcmp(message, "-1:-1:-1"), 0);
    // No space
    memset(message, 0, 10);
    status = format_time_to_str(message, 3, 110, 22, 33);
    UCUNIT_CheckIsEqual(status, 1);
    UCUNIT_CheckIsEqual(strcmp(message, ""), 0);
    
    
    // Just visualize the actual formatted message in the console.
    // Check for success.
    char formatted_times[128] = {0};
    UCUNIT_CheckIsEqual(write_wake_times_message(formatted_times, 128), 0 );
    printf("Wake times print:\n%s \n", formatted_times);
    memset(formatted_times, 0, 128);
    UCUNIT_CheckIsEqual(write_sleep_times_message(formatted_times, 128), 0);
    printf("Sleep times print:\n%s \n", formatted_times);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

#ifdef RUN_TIME_INTENSIVE_TESTS
/*
 * Tests the ignoring of a single wake.
 * Requires some time to run, therefore should not be run everytime.
 */
static void Test_ignore_wake_when_wake_before_sleep()
{
    UCUNIT_TestcaseBegin("Checking ignore of a single wake. Wake before sleep.");
    // Setup
    setup_files();
    
    int testh = get_current_h();
    int testm = get_current_m();
    int tests = get_current_s();
    // Do not run test if its to late, avoid stupid overflows.
    if (testh == 23 && testm > 55)
    {
        printf("Run this test later, waking and sleeping at the very end"
                "of a day won't occur.\n");
        return;
    }
    
    // Set wake and sleep in the very near future.
    time_plus_secs(2, &testh, &testm, &tests);
    set_wake((MON_T | TUE_T | WED_T | THU_T | FRI_T | SAT_T | SUN_T),
                testh, testm, tests);
    time_plus_secs(3, &testh, &testm, &tests);
    set_sleep((MON_T | TUE_T | WED_T | THU_T | FRI_T | SAT_T | SUN_T),
                testh, testm, tests);
    
    // Check whether wake is ignored as expected.
    // Before wake
    set_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    // After wake
    sleep(3);
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    // After sleep
    sleep(3);
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 0 );
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

/*
 * Tests the ignoring of a single wake.
 * Requires some time to run, therefore should not be run everytime.
 */
static void Test_ignore_wake_when_wake_before_sleep_reset()
{
    UCUNIT_TestcaseBegin("Checking ignore of a single wake. Wake before sleep. "
                         "Unset ignore while wake.");
    // Setup
    setup_files();
    
    int testh = get_current_h();
    int testm = get_current_m();
    int tests = get_current_s();
    // Do not run test if its to late, avoid stupid overflows.
    if (testh == 23 && testm > 55)
    {
        printf("Run this test later, waking and sleeping at the very end"
                "of a day won't occur.\n");
        return;
    }
    
    // Set wake and sleep in the very near future.
    time_plus_secs(2, &testh, &testm, &tests);
    set_wake((MON_T | TUE_T | WED_T | THU_T | FRI_T | SAT_T | SUN_T),
                testh, testm, tests);
    time_plus_secs(3, &testh, &testm, &tests);
    set_sleep((MON_T | TUE_T | WED_T | THU_T | FRI_T | SAT_T | SUN_T),
                testh, testm, tests);
    
    // Check whether wake is ignored as expected.
    // Before wake
    set_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    // After wake
    sleep(3);
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    unset_ignore();
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 0 );
    // After sleep
    sleep(3);
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 0 );
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

/*
 * Tests the ignoring of a single wake.
 * Requires some time to run, therefore should not be run everytime.
 */
static void Test_ignore_wake_when_sleep_before_wake()
{
    UCUNIT_TestcaseBegin("Checking ignore of a single wake. Sleep before wake.");
    // Setup
    setup_files();
    
    int testh = get_current_h();
    int testm = get_current_m();
    int tests = get_current_s();
    // Do not run test if its to late, avoid stupid overflows.
    if (testh == 23 && testm > 55)
    {
        printf("Run this test later, waking and sleeping at the very end"
                "of a day won't occur.\n");
        return;
    }
    
    // Set wake and sleep in the very near future.
    time_plus_secs(2, &testh, &testm, &tests);
    set_sleep((MON_T | TUE_T | WED_T | THU_T | FRI_T | SAT_T | SUN_T),
                testh, testm, tests);
    time_plus_secs(3, &testh, &testm, &tests);
    set_wake((MON_T | TUE_T | WED_T | THU_T | FRI_T | SAT_T | SUN_T),
                testh, testm, tests);
    
    // Check whether wake is ignored as expected.
    // Before sleep
    set_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    // After sleep
    sleep(3);
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    // After wake
    sleep(3);
    update_ignore();
    UCUNIT_CheckIsEqual(get_ignore(), 1 );
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}
#endif // RUN_TIME_INTENSIVE_TESTS
#endif // TESTABLE_TK_CODE


#ifdef TESTABLE_MOTOR_CODE
static void Test_basic_open_close()
{
    UCUNIT_TestcaseBegin("Checking basic open and close.");
    // Ensure this number is low enough to not cause any troubles.
    
    const int STEPS_TO_MAKE = 4;
    
    int status = 0;
    
    status |= setup_motor_control();
    // Set custom step limit.
    current_steps = 0;
    target_steps = STEPS_TO_MAKE;
    
    // Check for successful init
    UCUNIT_CheckIsEqual(status, 0);
    
    while(open_nonblocking());
    UCUNIT_CheckIsEqual(current_steps, target_steps);
    UCUNIT_CheckIsEqual(get_curtain_state(), CURTAIN_OPEN_T);
    while(close_nonblocking());
    UCUNIT_CheckIsEqual(current_steps, 0);
    UCUNIT_CheckIsEqual(get_curtain_state(), CURTAIN_CLOSED_T);
    
    UCUNIT_TestcaseEnd();
}

static void Test_current_state()
{
    current_steps = 0;
    target_steps = 10;
    
    UCUNIT_CheckIsEqual(get_curtain_state(), CURTAIN_CLOSED_T);
    
    current_steps = 10;
    UCUNIT_CheckIsEqual(get_curtain_state(), CURTAIN_OPEN_T);
    
    current_steps = 5;
    UCUNIT_CheckIsEqual(get_curtain_state(), CURTAIN_UNDEFINED_T);
    current_steps = -5;
    UCUNIT_CheckIsEqual(get_curtain_state(), CURTAIN_UNDEFINED_T);
    current_steps = 15;
    UCUNIT_CheckIsEqual(get_curtain_state(), CURTAIN_UNDEFINED_T);
    
    UCUNIT_TestcaseEnd();
}

// TODO Should the calibrate also be tested? requires hardware...
#endif // TESTABLE_MOTOR_CODE


#ifdef TESTABLE_ACTION_EXEC
static void Test_basic_curtain_control_actions()
{
    UCUNIT_TestcaseBegin("Checking basic open and close by action.");
    
    user_action_t open_act = {0};
    open_act.act_type = CURTAIN_CONTROL_T;
    open_act.data[0] = OPEN_T;
    
    user_action_t close_act = {0};
    close_act.act_type = CURTAIN_CONTROL_T;
    close_act.data[0] = CLOSE_T;
    
    char message[MESSAGE_LEN] = {0};
    
    // Ensure this number is low enough to not cause any troubles.
    const int STEPS_TO_MAKE = 4;
    
    int status = 0;
    
    status |= setup_motor_control();
    // Set custom step limit.
    current_steps = 0;
    target_steps = STEPS_TO_MAKE;
    
    // Check for successful init
    UCUNIT_CheckIsEqual(status, 0);
    printf("Actual current %i \n", current_steps);
    
    while(execute_action_non_blocking(&open_act,
                                        message,
                                        MESSAGE_LEN));
    UCUNIT_CheckIsEqual(current_steps, target_steps);
    printf("Actual current %i \n", current_steps);
    while(execute_action_non_blocking(&close_act,
                                        message,
                                        MESSAGE_LEN));
    UCUNIT_CheckIsEqual(current_steps, 0);
    printf("Actual current %i \n", current_steps);
    
    UCUNIT_TestcaseEnd();
}

static void Test_basic_wake_actions()
{
    UCUNIT_TestcaseBegin("Setting wake for multiple days using an action.");
    // Setup
    setup_files();
    
    // Create the action
    user_action_t wake_set_act = {0};
    wake_set_act.act_type = WAKE_SET_T;
    wake_set_act.data[0] = (MON_T | TUE_T | SAT_T);
    wake_set_act.data[1] = 11;
    wake_set_act.data[2] = 2;
    wake_set_act.data[3] = 3;
    
    char message[MESSAGE_LEN] = {0};
    
    // Execute the action
    while(execute_action_non_blocking(&wake_set_act,
                                message,
                                MESSAGE_LEN));
                                
    printf("Message %s \n", message);
    
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, 11);
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_hour, 11);
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_hour, 11);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_hour, DEFAULT_WEEKEND_WAKE);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_invalid_time_in_wake_actions()
{
    UCUNIT_TestcaseBegin("Trying to set wake for multiple days using an invalid"
                            " action.\nThe time is not valid.");
    // Setup
    setup_files();
    
    // Create the action
    user_action_t wake_set_act = {0};
    wake_set_act.act_type = WAKE_SET_T;
    wake_set_act.data[0] = (MON_T | TUE_T | SAT_T);
    wake_set_act.data[1] = 110;
    wake_set_act.data[2] = 2;
    wake_set_act.data[3] = 3;
    
    char message[MESSAGE_LEN] = {0};
    
    // Execute the action
    while(execute_action_non_blocking(&wake_set_act,
                                message,
                                MESSAGE_LEN));
                                
    printf("Message invalid wake: \n%s \n", message);
    
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_hour, DEFAULT_WEEKEND_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_hour, DEFAULT_WEEKEND_WAKE);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_invalid_command_in_wake_actions()
{
    UCUNIT_TestcaseBegin("Trying to set wake for multiple days using an invalid"
                            " action.\nThe command is invalid.");
    // Setup
    setup_files();
    
    // Create the action
    user_action_t wake_set_act = {0};
    // Invalid action type:
    wake_set_act.act_type = 123;
    wake_set_act.data[0] = (MON_T | TUE_T | SAT_T);
    wake_set_act.data[1] = 11;
    wake_set_act.data[2] = 2;
    wake_set_act.data[3] = 3;
    
    char message[MESSAGE_LEN] = {0};
    
    // Execute the action
    while(execute_action_non_blocking(&wake_set_act,
                                message,
                                MESSAGE_LEN));
                                
    printf("Message invalid wake: \n%s \n", message);
    
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_hour, DEFAULT_WEEKEND_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_hour, DEFAULT_WEEKEND_WAKE);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_parser_errors_in_wake_actions()
{
    UCUNIT_TestcaseBegin("Trying to set wake for multiple days using an invalid"
                            "action.\nThe parser appended errors.");
    // Setup
    setup_files();
    
    // Create the action
    user_action_t wake_set_act = {0};
    wake_set_act.act_type = WAKE_SET_T;
    wake_set_act.data[0] = (MON_T | TUE_T | SAT_T) 
                                | (INVALID_TIME_ERR | INVALID_COMMAND_ERR);
    wake_set_act.data[1] = 11;
    wake_set_act.data[2] = 2;
    wake_set_act.data[3] = 3;
    
    char message[MESSAGE_LEN] = {0};
    
    // Execute the action
    while(execute_action_non_blocking(&wake_set_act,
                                message,
                                MESSAGE_LEN));
                                
    printf("Message invalid wake: \n%s \n", message);
    
    UCUNIT_CheckIsEqual(wake_times.tm_mon.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_tue.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_wed.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_thu.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_fri.tm_hour, DEFAULT_WEEK_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sat.tm_hour, DEFAULT_WEEKEND_WAKE);
    UCUNIT_CheckIsEqual(wake_times.tm_sun.tm_hour, DEFAULT_WEEKEND_WAKE);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

// TODO Maybe vary these checks a litte, make them harder.
static void Test_basic_sleep_actions()
{
    UCUNIT_TestcaseBegin("Setting sleep for multiple days using an action.");
    // Setup
    setup_files();
    
    // Create the action
    user_action_t sleep_set_act = {0};
    sleep_set_act.act_type = SLEEP_SET_T;
    sleep_set_act.data[0] = (WED_T | THU_T | FRI_T | SUN_T);
    sleep_set_act.data[1] = 18;
    sleep_set_act.data[2] = 2;
    sleep_set_act.data[3] = 3;
    
    char message[MESSAGE_LEN] = {0};
    
    // Execute the action
    while(execute_action_non_blocking(&sleep_set_act,
                                message,
                                MESSAGE_LEN));
                                
    printf("Message %s \n", message);
    
    UCUNIT_CheckIsEqual(sleep_times.tm_mon.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual(sleep_times.tm_tue.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual(sleep_times.tm_wed.tm_hour, 18);
    UCUNIT_CheckIsEqual(sleep_times.tm_thu.tm_hour, 18);
    UCUNIT_CheckIsEqual(sleep_times.tm_fri.tm_hour, 18);
    UCUNIT_CheckIsEqual(sleep_times.tm_sat.tm_hour, DEFAULT_SLEEP);
    UCUNIT_CheckIsEqual(sleep_times.tm_sun.tm_hour, 18);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}

static void Test_other_actions()
{
    UCUNIT_TestcaseBegin("Checking other actions.");
    // Setup
    setup_files();
    
    // Help string
    // Create the action
    user_action_t help_act = {0};
    help_act.act_type = HELP_T;
    
    char message[MESSAGE_LEN] = {0};

    // Execute the action
    while(execute_action_non_blocking(&help_act,
                                message,
                                MESSAGE_LEN));
                                
    printf("Help string: \n%s\n", message);
    
    
    // Current time
    // Create the action
    user_action_t curtime_act = {0};
    curtime_act.act_type = CURTIME_T;
    
    memset(message, 0, MESSAGE_LEN);

    // Execute the action
    while(execute_action_non_blocking(&curtime_act,
                                message,
                                MESSAGE_LEN));
                                
    printf("Curtime string: \n%s", message);
    
    // Reset
    setup_files();
    
    UCUNIT_TestcaseEnd();
}
#endif // TESTABLE_ACTION_EXEC


#ifdef TESTABLE_ALARMCHECKER_CODE
static void Test_new_state_ttw_tts_today()
{
    UCUNIT_TestcaseBegin("Checking new state based on today's ttw and tts.");
    
    /*
     * TODO
     * Jooo check if this makes sense.
     * 
     * now is the current time, |wake| today's wake_time,
     * |sleep| today's sleep_time.
     * Only the current day is considered.
     */
    
    // now |wake| ... |sleep| ...
    UCUNIT_CheckIsEqual(new_state_ttw_tts_today(5, 10), CURTAIN_UNDEFINED_T);
    // ... |wake| now |sleep| ...
    UCUNIT_CheckIsEqual(new_state_ttw_tts_today(-5, 5), CURTAIN_OPEN_T);
    // ... |wake| ... |sleep| now
    UCUNIT_CheckIsEqual(new_state_ttw_tts_today(-10, -5), CURTAIN_CLOSED_T);
    
    // now |sleep| ... |wake| ...
    UCUNIT_CheckIsEqual(new_state_ttw_tts_today(10, 5), CURTAIN_UNDEFINED_T);
    // ... |sleep| now |wake| ...
    UCUNIT_CheckIsEqual(new_state_ttw_tts_today(5, -5), CURTAIN_CLOSED_T);
    // ... |sleep| ... |wake| now
    UCUNIT_CheckIsEqual(new_state_ttw_tts_today(-5, -10), CURTAIN_OPEN_T);
    
    UCUNIT_TestcaseEnd();
}
#endif // TESTABLE_ALARMCHECKER_CODE

#ifdef TESTABLE_STORAGE_CODE
static void Test_time_storage()
{
    UCUNIT_TestcaseBegin("Checking storing and loading some times.");
    
    store_time(0, 1, 2, 3, 4);
    store_time(1, 0, 0, 0, 0);
    store_time(2, 9, 23, 59, 59);
    
    uint8_t d, h, m, s;
    load_time(0, &d, &h, &m, &s );
    UCUNIT_CheckIsEqual(d, 1);
    UCUNIT_CheckIsEqual(h, 2);
    UCUNIT_CheckIsEqual(m, 3);
    UCUNIT_CheckIsEqual(s, 4);
    load_time(1, &d, &h, &m, &s );
    UCUNIT_CheckIsEqual(d, 0);
    UCUNIT_CheckIsEqual(h, 0);
    UCUNIT_CheckIsEqual(m, 0);
    UCUNIT_CheckIsEqual(s, 0);
    load_time(2, &d, &h, &m, &s );
    UCUNIT_CheckIsEqual(d, 9);
    UCUNIT_CheckIsEqual(h, 23);
    UCUNIT_CheckIsEqual(m, 59);
    UCUNIT_CheckIsEqual(s, 59);
    
    UCUNIT_TestcaseEnd();
}

static void Test_ssid_storage()
{
    UCUNIT_TestcaseBegin("Checking storing and loading an ssid.");
    
    char ssid1[] = "Test123";
    char ssid2[33] = {0};
    int test_len = 0;
    
    store_ssid(ssid1, strlen(ssid1));
    load_ssid(ssid2, 33, &test_len);
    UCUNIT_CheckIsEqual(strcmp(ssid1, ssid2), 0);
    UCUNIT_CheckIsEqual(strlen(ssid1), test_len);
    
    UCUNIT_TestcaseEnd();
}

static void Test_pw_storage()
{
    UCUNIT_TestcaseBegin("Checking storing and loading a password.");
    
    char pw1[] = "Test123";
    char pw2[33] = {0};
    int test_len = 0;
    
    store_ssid(pw1, strlen(pw1));
    load_ssid(pw2, 33, &test_len);
    UCUNIT_CheckIsEqual(strcmp(pw1, pw2), 0);
    UCUNIT_CheckIsEqual(strlen(pw1), test_len);
    
    UCUNIT_TestcaseEnd();
}
#endif // TESTABLE_STORAGE_CODE

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

int time_plus_secs(int secs, int *h, int *m, int *s)
{
    if (secs > 59)
    {
        *h = 0;
        *m = 0;
        *s = 0;
        return 1;
    }
    if ((*h == 23) && (*m >= 58))
    {
        *h = 0;
        *m = 0;
        *s = 0;
        return 1;
    }
    
    int inh = *h;
    int inm = *m;
    int ins = *s;
    *s = (ins + secs) % 60;
    *m = (inm + ((ins + secs) / 60)) % 60;
    *h = inh + ((inm + ((ins + secs) / 60)) / 60);
    
    printf("prev %i:%i:%i; plus secs %i:%i:%i \n", inh, inm, ins, *h, *m, *s);
    return 0;
}

int setup_files()
{
    int status = 0;
    
    status = setup_time_keeper();
    status = setup_command_parser();
    status = setup_motor_control();
    status = setup_storage();

    return status;
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
    Test_get_action_set_incomplete_times();
    Test_get_message_from_errors();
#endif // TESTABLE_PARSER_CODE
    
#ifdef TESTABLE_TK_CODE
    Test_set_wake_single_day();
    Test_set_wake_multiple_days();
    Test_set_wake_invalid();
    Test_set_sleep_single_day();
    Test_time_until_wake_today();
    Test_store_time();
    Test_write_times_message();
    
    // These take some time, do not run always.
#ifdef RUN_TIME_INTENSIVE_TESTS
    Test_ignore_wake_when_wake_before_sleep();
    Test_ignore_wake_when_sleep_before_wake();
    Test_ignore_wake_when_wake_before_sleep_reset();
#endif // RUN_TIME_INTENSIVE_TESTS
#endif // TESTABLE_TK_CODE

#ifdef TESTABLE_MOTOR_CODE
    Test_basic_open_close();
    Test_current_state();
#endif // TESTABLE_MOTOR_CODE

#ifdef TESTABLE_ACTION_EXEC
    Test_basic_curtain_control_actions();
    Test_basic_wake_actions();
    Test_invalid_time_in_wake_actions();
    Test_invalid_command_in_wake_actions();
    Test_parser_errors_in_wake_actions();
    Test_basic_sleep_actions();
    Test_other_actions();
#endif // TESTABLE_ACTION_EXEC

#ifdef TESTABLE_ALARMCHECKER_CODE
    Test_new_state_ttw_tts_today();
#endif // TESTABLE_ALARMCHECKER_CODE

#ifdef TESTABLE_STORAGE_CODE
    Test_time_storage();
    Test_ssid_storage();
    Test_pw_storage();
#endif // TESTABLE_STORAGE_CODE

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