/*
 * Project wide used types and enums are defined in this file.
 */

#ifndef _TYPES_AND_ENUMS_H_
#define _TYPES_AND_ENUMS_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Includes **********************************/
#include <stdint.h>


/********************************** Defines ***********************************/
#define TAG_LEN 16


/********************************* Constants **********************************/
enum TIME_ERRORS
{
    INVALID_DAY_ERR = 0x1000,
    INVALID_TIME_ERR = 0x2000,
    INVALID_COMMAND_ERR = 0x4000,
    ALL_ERRS = (INVALID_DAY_ERR | INVALID_TIME_ERR | INVALID_COMMAND_ERR)
};

enum ACTION_TYPE
{
    NONE_T = 0x0000,
    WAKE_SET_T = 0x0001,
    SLEEP_SET_T = 0x0002,
    CURTAIN_CONTROL_T = 0x0004,
    CURTIME_T = 0x0008,
    HELP_T = 0x0010,
    IGNORE_ONCE_T = 0x0020,
    WAKE_TIMES_T = 0x0040,
    SLEEP_TIMES_T = 0x0080,
    ERROR_T = 0x0100
};

// These should be able to be combined
enum DAY_TYPE
{
    MON_T = 0x0001,
    TUE_T = 0x0002,
    WED_T = 0x0004,
    THU_T = 0x0008,
    FRI_T = 0x0010,
    SAT_T = 0x0020,
    SUN_T = 0x0040
};

enum CURTAIN_CONTROL_ACT_T
{
    OPEN_T = 0x0001,
    CLOSE_T = 0x0002,
    CALIBRATE_T = 0x0004,
    CURTAIN_XOR_T = 0x0008,
    CURTAIN_ERROR_T = 0x0010
};


/***************************** Struct definitions *****************************/
typedef struct 
{
    enum ACTION_TYPE act_type;
    uint32_t data[5];
} user_action_t;

typedef struct
{
    char tag[TAG_LEN];
    int (*setup)();
    int (*input_available)();
    user_action_t (*fetch_action)();
    int (*respond_to_user)(char *buf, int buf_max_len);
} input_handler_t;


#ifdef __cplusplus
}
#endif

#endif // _TYPES_AND_ENUMS_H_