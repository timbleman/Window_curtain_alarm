/*
 * This file is used to configure the project build.
 */

#ifdef __cplusplus
extern "C" {
#endif

// Adjust these
// TODO This should be done using WPS.
#define YOUR_SSID "YourSSID"
#define YOUR_PW "*********"
#define SSID_MAX_LEN 33
#define ENABLE_LOCAL_WEB_SITE true

// Print heap fragmentation stats.
#define PRINT_HEAP_STATS_EVERY_MILLIS 240000

// Enable this if the system should continue without calibration after a reboot.
#define RECOVER_AFTER_RESTART

// TODO Unit tests are not yet available for ESP as they do not fit in memory.
// Run the proof_of_concept if you want to check if new code passes tests.
#undef UNITTESTS_INSTEAD_OF_MAIN

#ifdef UNITTESTS_INSTEAD_OF_MAIN
#undef TESTABLE_TK_CODE
#define TESTABLE_PARSER_CODE
#undef TESTABLE_MOTOR_CODE

// Testing bigger parts of the system requires exposed component internals.
#if defined(TESTABLE_PARSER_CODE) & defined(TESTABLE_MOTOR_CODE) & defined(TESTABLE_TK_CODE)
#define TESTABLE_ACTION_EXEC
#endif

#define TESTABLE_ALARMCHECKER_CODE

#endif // UNITTESTS_INSTEAD_OF_MAIN

#ifdef __cplusplus
}
#endif
