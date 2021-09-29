/********************************** Includes **********************************/
#include "command_parser.h"

#include <stdio.h>  // printf()
#include <ctype.h>  // isprint()
#include <string.h> // strtok_r()
#include <getopt.h>
#include <stdlib.h> // abort()
#include <stdint.h>
#include "configuration.h"


/********************************* Constants *********************************/


/***************************** Struct definitions *****************************/


/**************************** Prototype functions ****************************/
char *parse_arbitrary_arg(char **split_command, int command_num, char option);
char * strtok_space(char **save_ptr);
int getopt_example (int argc, char **argv);
enum CURTAIN_CONTROL_ACT_T parse_open_close(char *user_str);
user_action_t get_time_action(char **split_command, int command_num);
int parse_days(char **split_command, int command_num);
uint32_t check_mins_secs_validity(int m_s_int, char *m_s_str);
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



/**************************** Function definitions ****************************/
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
                            new_act.act_type = parsed_act;
                            break;
        case SLEEP_SET_T:   new_act = get_time_action(argv, argc);
                            new_act.act_type = parsed_act;
                            break;
        case CURTAIN_CONTROL_T: new_act.data[0] = parse_open_close(user_str);
                                break;
        default: break;
    }
    
    return new_act;
}

/*
 * This function extracts time from user input and adds creates a user_action_t.
 * Days are saved in the first data location, hours in the second, minutes in 
 * the third, seconds in the fourth.
 * Errors are appended to the first data location (days).
 * Copy by value is used, but performance should not be critical.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @return: user_action_t, the type being still empty.
 */
user_action_t get_time_action(char **split_command, int command_num)
{
    uint32_t days, h, m, s = 0;
    uint32_t all_errors = 0;
    
    days = parse_days(split_command, command_num);
    h = parse_hour(split_command, command_num);
    m = parse_min(split_command, command_num);
    s = parse_sec(split_command, command_num);
        
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
    /*
    char *hvalue = NULL;
    //int index;
    int c;

    opterr = 0;

    while ((c = getopt (command_num, split_command, "h:")) != -1)
    {
        switch (c)
        {
            case 'h':
                hvalue = optarg;
                break;
            case '?':
                if (optopt == 'h')
                    printf ("Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    printf ("Unknown option `-%c'.\n", optopt);
                else
                    printf ("Unknown option character `\\x%x'.\n",
                        optopt);
                    //return 1;
            default:
                continue;
                //abort ();
        }
    }

    // This creates problems, location may or may not be freed when returning
    return hvalue;
    */
}

/*
 * This function is used to extract day codes from the user input.
 * TODO No error handling has been implemented yet.
 * 
 * @param split_command: Array pointing tokenized user input.
 * @param command_num: Number of substrings.
 * @return: The day codes. Onehot and XORed together.
 */
int parse_days(char **split_command, int command_num)
{
    // TODO Replace by calloc and free, make size variable.
    char *dvalue = "                                                     ";
    //int errors = 0;
    
    dvalue = parse_arbitrary_arg(split_command, command_num, 'd');
    if (dvalue == NULL)
        dvalue = "";
        
#ifdef PARSE_TIME_DEBUG_PRINTS
    printf("dvalue %s \n", dvalue);
#endif // PARSE_TIME_DEBUG_PRINTS
    
    uint32_t days = 0;
    
    if (strstr(dvalue, "mon") != NULL)
        days = days | MON_T;
    if (strstr(dvalue, "tue") != NULL)
        days = days | TUE_T;
    if (strstr(dvalue, "wed") != NULL)
        days = days | WED_T;
    if (strstr(dvalue, "thu") != NULL)
        days = days | THU_T;
    if (strstr(dvalue, "fri") != NULL)
        days = days | FRI_T;
    if (strstr(dvalue, "sat") != NULL)
        days = days | SAT_T;
    if (strstr(dvalue, "sun") != NULL)
        days = days | SUN_T;
        
    // TODO Find writing errors!
        
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
    char *hvalue = "     ";
    //int index;
    //int c;
    
    int errors = 0;

    //opterr = 0;

    /*
    int str_i = 0;
    while (str_i < command_num)
    {
        printf("str at %i: %s \n", str_i, split_command[str_i]);
        if (!strcmp(split_command[str_i], "-h"))
        {
            str_i++;
            break;
        }
        str_i++;
    }
    if (str_i != command_num)
    {
        hvalue = split_command[str_i];
    }
    */
    
    hvalue = parse_arbitrary_arg(split_command, command_num, 'h');
    if (hvalue == NULL)
        hvalue = "";
    /*
    while ((c = getopt (command_num, split_command, "h:")) != -1)
    {
        switch (c)
        {
            case 'h':
                printf("h option detected!\n");
                hvalue = optarg;
                break;
            case '?':
                if (optopt == 'h')
                {
                    printf ("Option -%c requires an argument.\n", optopt);
                    errors &= INVALID_HOUR_ERR;
                }
                else if (isprint (optopt))
                    printf ("Unknown option `-%c'.\n", optopt);
                else
                    printf ("Unknown option character `\\x%x'.\n",
                        optopt);
                    //return 1;
            default:
                continue;
                //abort ();
        }
    }
    */
    
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
    char *mvalue = "-\n";
    //int index;
    
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
    //int index;
    
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
        
    printf("argv[0] %s \n", split_command[0]);

    if (strcmp(split_command[0], "set_wake") == 0)
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("WAKE_SET_T found command\n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return WAKE_SET_T;
    }
    else if (strcmp(split_command[0], "set_sleep") == 0)
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("SLEEP_SET_T found command\n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return SLEEP_SET_T;
    }
    else if ((strcmp(split_command[0], "open") == 0)
                || (strcmp(split_command[0], "close") == 0)) 
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("CURTAIN_CONTROL_T found command\n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return CURTAIN_CONTROL_T;    
    }
    else if ((strcmp(split_command[0], "help") == 0)
                || (strcmp(split_command[0], "--help") == 0)) 
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("HELP_T found command\n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return HELP_T;    
    }
    else if (strcmp(split_command[0], "curtime") == 0) 
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("CURTIME_T found command\n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return CURTIME_T;    
    }
    else
    {
#ifdef PARSE_ACTION_DEBUG_PRINTS
        printf("No valid command found!\n");
#endif // PARSE_ACTION_DEBUG_PRINTS
        return NONE_T;
    }
    
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
        return CLOSE_T;
        return CLOSE_T;
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
static void demonstrate_tokenise_to_argc_argv(char buffer[], int buffer_size)
{ // This demonstrates usage of tokenise_to_argc_argv 
  int   argc     = 0;
  char *argv[10] = {0};

  printf("* **Initial State**\n");
  //print_buffer(buffer, buffer_size);

  // Tokenise Command Buffer 
  tokenise_to_argc_argv(buffer, &argc, argv, sizeof(argv));

  printf("* **After Tokenizing**\n");
  //print_buffer(buffer, buffer_size);
  //print_argc_argv(argc,argv);
  printf("\n\n");
}
*/

/*
static void print_buffer(char *buffer, int size)
{
  printf(" - Buffer Content `");
  for (int i = 0 ; i < size; i++) printf("%c",isprint(buffer[i])?buffer[i]:'0');
  printf("` | HEX: ");
  for (int i = 0 ; i < size; i++) printf("%02X ", buffer[i]);
  printf("\n");
}
*/

/*
static void print_argc_argv(int argc, char *argv[])
{ // This displays the content of argc and argv 
  printf("* **Argv content** (argc = %d): %s\n", argc, argc ? "":"Argv Is Empty");
  for (int i = 0 ; i < argc ; i++) printf(" - `argv[%d]` = `%s`\n", i, argv[i]);
}
*/

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

int getopt_example (int argc, char **argv)
{
  int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "abc:")) != -1)
    switch (c)
      {
      case 'a':
        aflag = 1;
        break;
      case 'b':
        bflag = 1;
        break;
      case 'c':
        cvalue = optarg;
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }

  printf ("aflag = %d, bflag = %d, cvalue = %s\n",
          aflag, bflag, cvalue);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  return 0;
}