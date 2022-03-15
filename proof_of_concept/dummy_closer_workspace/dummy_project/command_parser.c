/********************************** Includes **********************************/
#include "command_parser.h"

#include <stdio.h>  // printf()
#include <ctype.h>  // isprint()
#include <string.h> // strtok_r()
#include <getopt.h>
#include <stdlib.h> // abort()
#include <stdint.h>
#include <stdbool.h>
#include "configuration.h"


/********************************* Constants *********************************/
#define MAX_DAYS_PATTERNS 12
#define DAY_PATTERN_MAX_LEN 8

#define MAX_ACTS_PATTERNS 16
#define ACT_PATTERN_MAX_LEN 16


/***************************** Struct definitions *****************************/
typedef struct
{
    char match[DAY_PATTERN_MAX_LEN];
    enum DAY_TYPE dayt;
} day_match_t;

typedef struct
{
    char match[ACT_PATTERN_MAX_LEN];
    enum ACTION_TYPE actt;
} act_match_t;


/**************************** Prototype functions ****************************/
char *parse_arbitrary_arg(char **split_command, int command_num, char option);
void replace_last_return_by_space(char *buf);
char * strtok_space(char **save_ptr);
int getopt_example (int argc, char **argv);
bool command_has_arg(char **split_command, int command_num, char option);
enum CURTAIN_CONTROL_ACT_T parse_open_close(char *user_str);
user_action_t get_time_action(char **split_command, int command_num);
int parse_days(char **split_command, int command_num);
uint32_t check_mins_secs_validity(int m_s_int, char *m_s_str);
int add_days_pattern(const char *match, enum DAY_TYPE dayt);
int add_act_pattern(const char *match, enum ACTION_TYPE actt);
void setup_days_to_match();
void setup_acts_to_match();
int parse_days_template();
//static void print_buffer(char *buffer, int size);
//static void print_argc_argv(int argc, char *argv[]);
#ifndef TESTABLE_PARSER_CODE
enum ACTION_TYPE parse_action(char **split_command, int command_num);
int parse_hour(char **split_command, int command_num);
int parse_min(char **split_command, int command_num);
int parse_sec(char **split_command, int command_num);
int tokenise_to_argc_argv(
        char *buffer, int *argc, char *argv[], const int argv_length);
#endif // TESTABLE_PARSER_CODE


/**************************** Variable definitions ****************************/
day_match_t days_to_match[MAX_DAYS_PATTERNS] = {0};
uint16_t days_to_match_num = 0;

act_match_t acts_to_match[MAX_ACTS_PATTERNS] = {0};
uint16_t acts_to_match_num = 0;


/**************************** Function definitions ****************************/
int setup_command_parser()
{
    int status = 0;
    
    memset(days_to_match, 0, (MAX_DAYS_PATTERNS) * sizeof(day_match_t));
    setup_days_to_match();
    
    memset(acts_to_match, 0, (MAX_ACTS_PATTERNS) * sizeof(act_match_t));
    setup_acts_to_match();
    
    return status;
}

/*
 * A user_action_t is created from user input as string.
 * Currently wake set, sleep set and curtain actions are parsed.
 * Copy by value, as the struct is only 6 ints in size it should be ok!
 * 
 * @param user_str: User input to parse.
 * @return: user_action_t containing the action type and extracted data.
 */
user_action_t get_action(char *user_str)
{
    user_action_t new_act = {0};
    
    replace_last_return_by_space(user_str);
    
    int argc = 0;
    char *argv[MAX_USER_INPUT_LEN] = {0};
    
    // Convert to lower case if desired
#ifdef CONVERT_USER_IN_TO_LOWER
    char str_lower[MAX_USER_INPUT_LEN] = {0};
    for (size_t i = 0; i < strlen(user_str); ++i) {
        str_lower[i] = tolower((unsigned char) user_str[i]);
    }
    //printf("\nstr_lower to lower: %s \n", str_lower);
    tokenise_to_argc_argv(str_lower, &argc, argv, MAX_USER_INPUT_LEN);
#else
    // Regular parsing
    tokenise_to_argc_argv(user_str, &argc, argv, MAX_USER_INPUT_LEN);
#endif // CONVERT_USER_IN_TO_LOWER
    
    enum ACTION_TYPE parsed_act = parse_action(argv, argc);
    
    switch (parsed_act)
    {
        case WAKE_SET_T:    new_act = get_time_action(argv, argc);
                            break;
        case SLEEP_SET_T:   new_act = get_time_action(argv, argc);
                            break;
        case CURTAIN_CONTROL_T: new_act.data[0] = parse_open_close(str_lower);
                                break;
        default: break;
    }
    
    // This has to happen afterwards to return the correct type.
    new_act.act_type = parsed_act;
    
    return new_act;
}

/*
 * This function extracts time from user input and adds creates a user_action_t.
 * Days are saved in the first data location, hours in the second, minutes in 
 * the third, seconds in the fourth.
 * Requires -d and -h. -m and -s are optional
 * Errors are appended to the first data location (days).
 * Copy by value is used, but performance should not be critical.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @return: user_action_t, the type being still empty.
 */
user_action_t get_time_action(char **split_command, int command_num)
{
    uint32_t days = 0, h = 0, m = 0, s = 0;
    uint32_t all_errors = 0;
    
    // Invalid day if day not present
    if (command_has_arg(split_command, command_num, 'd'))
        days = parse_days(split_command, command_num);
    else
        days = INVALID_DAY_ERR;
    
    h = parse_hour(split_command, command_num);
    // Check whether -m is present, parse if so 
    if (command_has_arg(split_command, command_num, 'm'))
    {
        m = parse_min(split_command, command_num);
        
        // Only check for -s if -m has been passed
        if (command_has_arg(split_command, command_num, 's'))
        {
            s = parse_sec(split_command, command_num);
        }
    }
        
    // Check if errors occured
    all_errors = (days & ALL_ERRS) | (h & ALL_ERRS) | (m & ALL_ERRS)
                    | (s & ALL_ERRS);
    if (all_errors != 0)
    {
#ifdef BASIC_DEBUG_PRINTS
        printf("Errors have been detected! %X \n", all_errors);
#endif // BASIC_DEBUG_PRINTS
        days = days | all_errors;
    }
    
    // TODO copy by value
    user_action_t new_act = {0};
    // TODO define offsets
    new_act.data[0] = days;
    new_act.data[1] = h;
    new_act.data[2] = m;
    new_act.data[3] = s;
    
    return new_act;
}

/*
 * Taken from: Brain Khuu
 * https://stackoverflow.com/questions/1706551/parse-string-into-argv-argc
 * @brief Tokenise a string buffer into argc and argv format
 *
 * Tokenise string buffer to argc and argv form via strtok_r()
 * Warning: Using strtok_r will modify the string buffer
 *
 * Returns: Number of tokens extracted
 *
 */
int tokenise_to_argc_argv(
        char     *buffer,     ///< In/Out : Modifiable String Buffer To Tokenise
        int      *argc,       ///< Out    : Argument Count
        char     *argv[],     ///< Out    : Argument String Vector Array
        const int argv_length ///< In     : Maximum Count For `*argv[]`
      )
{ /* Tokenise string buffer into argc and argv format (req: string.h) */
  int i = 0;
  for (i = 0 ; i < argv_length ; i++)
  { /* Fill argv via strtok_r() */
    if ( NULL == (argv[i] = strtok_space(&buffer)) ) break;
  }
  *argc = i;
  return i; // Argument Count
}

/*
 * Checks whether an option is present in a command chain. 
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @param option: The char for the option to extract.
 * @return: Whether the option is present in the command.
 */
bool command_has_arg(char **split_command, int command_num, char option)
{
    bool contains = false;
    
    // The option to search for
    char matching_str[3] = "-\n";
    matching_str[1] = option;
    
    // Search the option
    int str_i = 0;
    while (str_i < command_num)
    {
        //printf("str at %i: %s \n", str_i, split_command[str_i]);
        if (!strcmp(split_command[str_i], matching_str))
        {
            str_i++;
            contains = true;
            break;
        }
        str_i++;
    }    
    
    return contains;
}

/*
 * Extracts the command value of an option (e.g.: -d, -h, -m, -s).
 * Should have used optarg, but because of some problems an own implementation
 * has been used.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @param option: The char for the option to extract.
 * @return: The value belonging to the option.
 */
char *parse_arbitrary_arg(char **split_command, int command_num, char option)
{
    //char **split_command = *split_com;
    // The option to search for
    char matching_str[3] = "-\n";
    matching_str[1] = option;
#ifdef BASIC_DEBUG_PRINTS
    printf("matching_str %s \n", matching_str);   
#endif // BASIC_DEBUG_PRINTS    

    // Search the option
    int str_i = 0;
    while (str_i < command_num)
    {
        //printf("str at %i: %s \n", str_i, split_command[str_i]);
        if (!strcmp(split_command[str_i], matching_str))
        {
            str_i++;
            break;
        }
        str_i++;
    }
    
    // Return value belonging to the option or NULL.
    if (str_i != command_num)
    {
        //printf("split_command[str_i] %p \n", &split_command[str_i]);
        // Check that the next string is not another option.
        if (split_command[str_i][0] == '-')
        {
            return NULL;
        }
        else
        {
            return split_command[str_i];
        }
    } 
    else
    {
        return NULL;
    }
}

/*
 * This function is used to extract day codes from the user input.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @return: The day codes. Onehot and XORed together.
 */
int parse_days(char **split_command, int command_num)
{
    char *dvalue = NULL;
    //int errors = 0;
    
    dvalue = parse_arbitrary_arg(split_command, command_num, 'd');
    if (dvalue == NULL)
        dvalue = "";
        
#ifdef PARSE_TIME_DEBUG_PRINTS
    printf("dvalue %s \n", dvalue);
#endif // PARSE_TIME_DEBUG_PRINTS
    
    uint32_t days = parse_days_template(dvalue, strlen(dvalue));
    
    if (days == 0)
        days |= INVALID_DAY_ERR;
        
    return days;
}

/*
 * Add a single day pattern.
 * @return: Success status.
 */
int add_pattern(const char *match, enum DAY_TYPE dayt)
{
    if (days_to_match_num < MAX_DAYS_PATTERNS)
    {
        strncpy(days_to_match[days_to_match_num].match, match, DAY_PATTERN_MAX_LEN);
        days_to_match[days_to_match_num].dayt = dayt;
        days_to_match_num++;
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
 * Setup patterns for day matching.
 * @return: None.
 */
void setup_days_to_match()
{
    days_to_match_num = 0;
    add_pattern("mon,", MON_T);
    add_pattern("tue", TUE_T);
    add_pattern("wed", WED_T);
    add_pattern("thu", THU_T);
    add_pattern("fri", FRI_T);
    add_pattern("sat", SAT_T);
    add_pattern("sun", SUN_T);
    add_pattern("weekend", SAT_T | SUN_T);
    add_pattern("week", MON_T | TUE_T | WED_T | THU_T | FRI_T);
}

/*
 * Parse days using multiple template.
 * Requires more memory than a static approach.
 * 
 * @param message: String containing days, separated by ','.
 * @param message_max_len:
 * @return: Parsed days, potentially including error.
 */
int parse_days_template(char *message, int message_max_len)
{
    // Alternative: Use malloc
    // Bad practice magic numbers
    char days_cpy[MAX_DAYS_PATTERNS * DAY_PATTERN_MAX_LEN] = {0};
    const int patlen = MAX_DAYS_PATTERNS * DAY_PATTERN_MAX_LEN;
    int min_len = message_max_len < patlen ? message_max_len : patlen;
    strncpy(days_cpy, message, min_len);
    
    int days = 0;
    
    /*
     * Remove matching days
     */
    for (int i = 0; i < days_to_match_num; i++)
    {
        char *strpos = strstr(days_cpy, days_to_match[i].match);
        if (strpos != NULL)
        {
            days |= days_to_match[i].dayt;
            memset(strpos, ' ', strlen(days_to_match[i].match));
        }
    }
        
    /* 
     * Check if there are still invalid chars left. 
     */
    for (int i = 0; i < strlen(days_cpy); i++)
    {
        char chr = days_cpy[i];
        if (chr != ' ' && chr != '\n' && chr != '\r' && chr != '\0' && chr != ',' )
        {
            days |= INVALID_DAY_ERR;
            break;
        }
    }
        
    return days;
}

/*
 * Parses hours from user input. Performs bounds and validity checking.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @return: The selected hour or error code.
 */
int parse_hour(char **split_command, int command_num)
{
    char *hvalue = NULL;
    
    int errors = 0;
    
    hvalue = parse_arbitrary_arg(split_command, command_num, 'h');
    if (hvalue == NULL)
        hvalue = "";
    
    uint32_t parsed_hour = atoi(hvalue);
    
#ifdef PARSE_TIME_DEBUG_PRINTS
    printf("hour value as str: %s \n", hvalue);
    printf("parsed hour value: %i \n", parsed_hour);
    
    
    printf("(strcmp(hvalue, 0) != 0) %i \n", (strcmp(hvalue, "0") != 0));
    printf("(strcmp(hvalue, 00) != 0) %i \n", (strcmp(hvalue, "00") != 0));
#endif // PARSE_TIME_DEBUG_PRINTS
    // TODO Check if string is parsed correctly
    if ((parsed_hour == 0) && 
            ((strcmp(hvalue, "0") != 0) && (strcmp(hvalue, "00") != 0)))
    {
#ifdef PARSE_TIME_DEBUG_PRINTS
        printf("Invalid, lol \n");
#endif // PARSE_TIME_DEBUG_PRINTS
        errors |= INVALID_TIME_ERR;
    }
    
    if ((parsed_hour < 0) || (parsed_hour > 23))
    {
#ifdef PARSE_TIME_DEBUG_PRINTS
        printf("Out of range, lol \n");
#endif // PARSE_TIME_DEBUG_PRINTS
        errors |= INVALID_TIME_ERR;
    }

    if (errors != 0)
        return errors;
        
    return parsed_hour;
}

/*
 * Parses minutes from user input. Performs bounds and validity checking.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @return: The selected min or error code.
 */
int parse_min(char **split_command, int command_num)
{
    char *mvalue = NULL;
    
    int errors = 0;

    mvalue = parse_arbitrary_arg(split_command, command_num, 'm');
    if (mvalue == NULL)
        mvalue = "";
        
    uint32_t parsed_min = atoi(mvalue);
    
    errors |= check_mins_secs_validity(parsed_min, mvalue);

    if (errors != 0)
        return errors;

    return parsed_min;
}

/*
 * Parses seconds from user input. Performs bounds and validity checking.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @return: The selected sec or error code.
 */
int parse_sec(char **split_command, int command_num)
{
    char *svalue = "-\n";
    
    int errors = 0;

    svalue = parse_arbitrary_arg(split_command, command_num, 's');
    if (svalue == NULL)
        svalue = "";
        
    uint32_t parsed_sec = atoi(svalue);
    
    errors |= check_mins_secs_validity(parsed_sec, svalue);

    if (errors != 0)
        return errors;

    return parsed_sec;
}

/*
 * Checks whether a minutes or seconds value has been parsed correctly from 
 * a string. This is necessary as "0", "00" or arbitrary strings are parsed
 * to the integer 0.
 * 
 * @param m_s_int: Parsed minutes or seconds integer.
 * @param m_s_str: String containing a number to be parsed.
 * @param: 0 if valid, INVALID_TIME_ERR otherwise.
 */
uint32_t check_mins_secs_validity(int m_s_int, char *m_s_str)
{
    int errors = 0;
    // TODO Check if string is parsed correctly
    if ((m_s_int == 0) && 
            ((strcmp(m_s_str, "0") != 0) && (strcmp(m_s_str, "00") != 0)))
    {
#ifdef PARSE_TIME_DEBUG_PRINTS
        printf("Invalid, lol \n");
#endif // PARSE_TIME_DEBUG_PRINTS
        errors |= INVALID_TIME_ERR;
    }
    
    if ((m_s_int < 0) || (m_s_int > 59))
    {
#ifdef PARSE_TIME_DEBUG_PRINTS
        printf("Out of range, lol \n");
#endif // PARSE_TIME_DEBUG_PRINTS
        errors |= INVALID_TIME_ERR;
    }
    
    return errors;
}

/*
 * Parses action type from user input. NONE_T if nothing found.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @return: The parsed ACTION_TYPE.
 */
enum ACTION_TYPE parse_action(char **split_command, int command_num)
{    
    if (command_num == 0)
        return NONE_T;
        
    for (int i = 0; i < acts_to_match_num; i++)
    {
        if (strcmp(split_command[0], acts_to_match[i].match) == 0)
        {
#ifdef PARSE_ACTION_DEBUG_PRINTS
            printf("Found %s command!\n", acts_to_match[i].match);
#endif // PARSE_ACTION_DEBUG_PRINTS
            return acts_to_match[i].actt;
        }
    }
    
#ifdef PARSE_ACTION_DEBUG_PRINTS
    printf("Found no matching command (NONE_T)!\n");
#endif // PARSE_ACTION_DEBUG_PRINTS
    
    return NONE_T;
}

/*
 * Add a single action pattern.
 * @return: Success status.
 */
int add_act_pattern(const char *match, enum ACTION_TYPE actt)
{
    if (acts_to_match_num < MAX_ACTS_PATTERNS)
    {
        strncpy(acts_to_match[acts_to_match_num].match, match, ACT_PATTERN_MAX_LEN);
        acts_to_match[acts_to_match_num].actt = actt;
        acts_to_match_num++;
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
 * Setup patterns for command recognition.
 * @return: None.
 */
void setup_acts_to_match()
{
    acts_to_match_num = 0;
    
    add_act_pattern("set_wake", WAKE_SET_T);
    add_act_pattern("set_sleep", SLEEP_SET_T);
    
    add_act_pattern("open", CURTAIN_CONTROL_T);
    add_act_pattern("close", CURTAIN_CONTROL_T);
    add_act_pattern("calibrate", CURTAIN_CONTROL_T);
    add_act_pattern("curtainxor", CURTAIN_CONTROL_T);
    
    add_act_pattern("help", HELP_T);
    add_act_pattern("--help", HELP_T);
    
    add_act_pattern("curtime", CURTIME_T);
    
    add_act_pattern("ignore", IGNORE_ONCE_T);
    
    add_act_pattern("waketimes", WAKE_TIMES_T);
    
    add_act_pattern("sleeptimes", SLEEP_TIMES_T);
}

/*
 * Parses whether a user input contains open or close commands.
 * 
 * @param user_str: The user string to parse.
 * @return: CURTAIN_CONTROL_ACT_T
 */
enum CURTAIN_CONTROL_ACT_T parse_open_close(char *user_str)
{
    if (strstr(user_str, "open") != NULL)
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("open type \n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return OPEN_T;
    }
    else if (strstr(user_str, "close") != NULL)
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("close type \n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return CLOSE_T;
    }
    else if (strstr(user_str, "calibrate") != NULL)
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("calibrate type \n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return CALIBRATE_T;
    }
    else if (strstr(user_str, "curtainxor") != NULL)
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("curtainxor type \n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return CURTAIN_XOR_T;
    }
    else
    {
        return CURTAIN_ERROR_T;
    }
}

/*
 * This function writes an error message in a predefined buffer.
 * Error codes have to be passed (one-hot, those can be XORed together).
 * Keep in mind "DAY_ERR, TIME_ERR, COMMAND_ERR" needs almost 32 bytes.
 * 
 * @param errs: (Multiple) error code(s).
 * @param buf: Preallocated string buffer to write message into.
 * @param str_max_len: The maximum length of the string.
 * @return: 0 if successful.
 */
int get_message_from_errors(enum TIME_ERRORS errs, char *buf, int str_max_len)
{
    // These errors will be written
    const char d_err[] = "DAY_ERR";
    const char t_err[] = "TIME_ERR";
    const char com_err[] = "COMMAND_ERR"; 
    
    // 6 to leave some room
    if (str_max_len < (strlen(d_err) + strlen(t_err) + strlen(com_err) + 6))
    {
        if (str_max_len >= 5)
            strcpy(buf, "SLEN");
        return 1;
    }
    
    /*
     * Write error strings to buffer. 
     */
    int index = 0;
    
    if (errs & INVALID_DAY_ERR)
    {
        strcpy(&buf[index], d_err);
        index += strlen(d_err);
    }

    if (errs & INVALID_TIME_ERR)
    {
        if (index != 0)
        {
            buf[index] = ',';
            buf[index + 1] = ' ';
            index += 2;
        }
        strcpy(&buf[index], t_err);
        index += strlen(t_err);
    }

    if (errs & INVALID_COMMAND_ERR)
    {
        if (index != 0)
        {
            buf[index] = ',';
            buf[index + 1] = ' ';
            index += 2;
        }
        strcpy(&buf[index], com_err);
        index += strlen(com_err);
    }    
        
    //INVALID_DAY_ERR | INVALID_TIME_ERR | INVALID_COMMAND_ERR
    return 0;
}

/*
 * Replace the last char in a string by a termination \0.
 */
void replace_last_return_by_space(char *buf)
{
    buf[strlen(buf) - 1] = '\0';
}

/*
 * Replacement for GNU C implementation of strtok_r(), supports only space.
 * Taken from: Brain Khuu
 * https://stackoverflow.com/questions/1706551/parse-string-into-argv-argc 
 */
char * strtok_space(char **save_ptr)
{ /* strtok_space is slightly modified from GNU C Library `strtok_r()`  implementation. 
      Thus this function is also licenced as GNU Lesser General Public License*/
  char *start = *save_ptr;
  char *end = 0;

  if (*start == '\0') {
    *save_ptr = start;
    return NULL;
  }

  /* Scan leading delimiters.  */
  while(*start == ' ') start++;
  if (*start == '\0') {
    *save_ptr = start;
    return NULL;
  }

  /* Find the end of the token.  */
  end = start;
  while((*end != '\0') && (*end != ' ')) end++;
  if (*end == '\0') {
    *save_ptr = end;
    return start;
  }

  /* Terminate the token and make *SAVE_PTR point past it.  */
  *end = '\0';
  *save_ptr = end + 1;
  return start;
}
