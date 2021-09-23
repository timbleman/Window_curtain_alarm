
#define UNITTESTS_INSTEAD_OF_MAIN

#define TESTABLE_TK_CODE
#define TESTABLE_PARSER_CODE
#define TESTABLE_MOTOR_CODE

// Testing bigger parts of the system requires exposed component internals.
#if defined(TESTABLE_PARSER_CODE) & defined(TESTABLE_MOTOR_CODE)
#define TESTABLE_ACTION_EXEC
#endif

#define TESTABLE_ALARMCHECKER_CODE