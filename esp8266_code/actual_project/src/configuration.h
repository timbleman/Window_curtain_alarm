
#ifdef __cplusplus
extern "C" {
#endif

#undef UNITTESTS_INSTEAD_OF_MAIN

#undef TESTABLE_TK_CODE
#define TESTABLE_PARSER_CODE
#undef TESTABLE_MOTOR_CODE

// Testing bigger parts of the system requires exposed component internals.
#if defined(TESTABLE_PARSER_CODE) & defined(TESTABLE_MOTOR_CODE) & defined(TESTABLE_TK_CODE)
#define TESTABLE_ACTION_EXEC
#endif

#define TESTABLE_ALARMCHECKER_CODE

#ifndef UNITTESTS_INSTEAD_OF_MAIN
#define PRINT_HEAP_STATS_EVERY_MILLIS 240000
#endif // UNITTESTS_INSTEAD_OF_MAIN

#ifdef __cplusplus
}
#endif
