[1mdiff --git a/esp8266_code/actual_project/README.md b/esp8266_code/actual_project/README.md[m
[1mindex 48dfe5b..d4f0a70 100644[m
[1m--- a/esp8266_code/actual_project/README.md[m
[1m+++ b/esp8266_code/actual_project/README.md[m
[36m@@ -3,17 +3,20 @@[m [mThis directory contains the proof_of_concept curtain closer code adapted to the[m
 [m
 ## Files of interest:[m
 To get a quick overview, I will list which files do what. To keep it compact, only the headers are listed:[m
[31m-* /src/portable_code/action_executer.h: Executes an action created by the command_parser.[m
[31m-* /src/portable_code/alarm_checker.h: Checks whether the curtain should be opened or closed now. Has to be called in a loop. [m
[31m-* /src/portable_code/command_parser.h: Here user input strings are parsed into an action type.[m
[32m+[m[32m* /src/hardware_specific_code/: Code specifically written for the ESP.[m
[32m+[m[32m    * /src/hardware_specific_code/web_server/: Local website, webserver and websocket. Needs arduinoWebSockets library.[m
[32m+[m[32m    * /src/hardware_specific_code/data_storage.h: Stores the configuration. Uses the EEPROM library.[m
[32m+[m[32m    * /src/hardware_specific_code/local_communication.h: Local (hardware, button) input and display.[m
[32m+[m[32m    * /src/hardware_specific_code/motor_controller.h: This file handles the calibration and operation of the motor.[m
[32m+[m[32m    * /src/hardware_specific_code/user_communication.h: Handles TCP socket setup and communication.[m
[32m+[m[32m* /src/portable_code/: Portable code.[m
[32m+[m[32m    * /src/portable_code/action_executer.h: Executes an action created by the command_parser.[m
[32m+[m[32m    * /src/portable_code/alarm_checker.h: Checks whether the curtain should be opened or closed now. Has to be called in a loop.[m[41m [m
[32m+[m[32m    * /src/portable_code/command_parser.h: Here user input strings are parsed into an action type.[m
[32m+[m[32m    * /src/portable_code/time_keeper.h: Saves wake and sleep times for each weekday. Keeps track of time.[m
[32m+[m[32m    * /src/portable_code/types_and_enums.h: Type and enum definitions that are shared between multiple components.[m
 * /src/configuration.h: Configures project build and testsuite execution.[m
[31m-* /src/hardware_specific_code/data_storage.h: Stores the configuration. Uses the EEPROM library.[m
[31m-* /src/local_communication.h: Local (hardware, button) input and display.[m
 * /src/main.cpp: This is the main file running the main loop.[m
[31m-* /src/hardware_specific_code/motor_controller.h: This file handles the calibration and operation of the motor.[m
[31m-* /src/portable_code/time_keeper.h: Saves wake and sleep times for each weekday. Keeps track of time.[m
[31m-* /src/portable_code/types_and_enums.h: Type and enum definitions that are shared between multiple components.[m
[31m-* /src/hardware_specific_code/user_communication.h: Handles TCP socket setup and communication.[m
 [m
 ## Configuration[m
 There are some configuration parameters.[m
[1mdiff --git a/esp8266_code/actual_project/src/configuration.h b/esp8266_code/actual_project/src/configuration.h[m
[1mindex edec030..2c38259 100644[m
[1m--- a/esp8266_code/actual_project/src/configuration.h[m
[1m+++ b/esp8266_code/actual_project/src/configuration.h[m
[36m@@ -6,6 +6,7 @@[m
 extern "C" {[m
 #endif[m
 [m
[32m+[m[32m/********************************** Defines **********************************/[m
 // TODO Unit tests are not yet available for ESP as they do not fit in memory.[m
 // Run the proof_of_concept if you want to check if new code passes tests.[m
 #undef UNITTESTS_INSTEAD_OF_MAIN[m
[36m@@ -24,6 +25,15 @@[m [mextern "C" {[m
 [m
 #endif // UNITTESTS_INSTEAD_OF_MAIN[m
 [m
[32m+[m[32m// Adjust these[m
[32m+[m[32m#define YOUR_SSID "FRITZ!Box 7430 FC"[m
[32m+[m[32m#define YOUR_PASSWORD "94044782303556147675"[m
[32m+[m[32m//"YourSSID";[m
[32m+[m[32m//"************";[m
[32m+[m
[32m+[m[32m// Enable a local webserver and websockets. Uses heap.[m
[32m+[m[32m#undef ENABLE_LOCAL_WEB_SITE[m
[32m+[m
 // Print heap fragmentation stats.[m
 #define PRINT_HEAP_STATS_EVERY_MILLIS 240000[m
 [m
[1mdiff --git a/esp8266_code/actual_project/src/hardware_specific_code/web_server/web_communication.cpp b/esp8266_code/actual_project/src/hardware_specific_code/web_server/web_communication.cpp[m
[1mindex 4c8b095..3271f78 100644[m
[1m--- a/esp8266_code/actual_project/src/hardware_specific_code/web_server/web_communication.cpp[m
[1m+++ b/esp8266_code/actual_project/src/hardware_specific_code/web_server/web_communication.cpp[m
[36m@@ -1,5 +1,6 @@[m
 /********************************** Includes **********************************/[m
 #include "web_communication.h"[m
[32m+[m[32m#ifdef ENABLE_LOCAL_WEB_SITE[m
 #include <ESP8266WiFi.h>[m
 #include <ESP8266WiFiMulti.h>[m
 #include "arduinoWebSockets/src/WebSocketsServer.h"[m
[36m@@ -265,4 +266,6 @@[m [mint respond_to_web_user(char *buf, int buf_max_len)[m
     bool status = webSocket.broadcastTXT(buf, buf_max_len);[m
 [m
     return (int)!status;[m
[31m-}[m
\ No newline at end of file[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m#endif // ENABLE_LOCAL_WEB_SITE[m
[1mdiff --git a/esp8266_code/actual_project/src/hardware_specific_code/web_server/web_communication.h b/esp8266_code/actual_project/src/hardware_specific_code/web_server/web_communication.h[m
[1mindex e5f78b6..618c870 100644[m
[1m--- a/esp8266_code/actual_project/src/hardware_specific_code/web_server/web_communication.h[m
[1m+++ b/esp8266_code/actual_project/src/hardware_specific_code/web_server/web_communication.h[m
[36m@@ -3,8 +3,10 @@[m
 [m
 /********************************** Includes **********************************/[m
 #include <stdint.h>[m
[32m+[m[32m#include "../../configuration.h"[m
 #include "../../portable_code/command_parser.h"[m
 [m
[32m+[m[32m#ifdef ENABLE_LOCAL_WEB_SITE[m
 [m
 /********************************* Constants *********************************/[m
 /***************************** Struct definitions *****************************/[m
[36m@@ -18,4 +20,6 @@[m
  */[m
 int setup_web_input_handler_t(input_handler_t *inh);[m
 [m
[32m+[m[32m#endif // ENABLE_LOCAL_WEB_SITE[m
[32m+[m
 #endif // _WEB_COMMUNICATION_H_[m
\ No newline at end of file[m
[1mdiff --git a/esp8266_code/actual_project/src/main.cpp b/esp8266_code/actual_project/src/main.cpp[m
[1mindex 49f3593..8c0723c 100644[m
[1m--- a/esp8266_code/actual_project/src/main.cpp[m
[1m+++ b/esp8266_code/actual_project/src/main.cpp[m
[36m@@ -21,21 +21,17 @@[m
 #include "time.h" // setenv(), do not worry about IDE warning, compiles fine[m
 [m
 [m
[31m-/********************************* Constants **********************************/[m
[32m+[m[32m/********************************** Defines **********************************/[m
 #define RESPOND_BUF_SIZE 256[m
 #define SSID_MAX_LEN 33[m
[32m+[m[32m#ifndef ENABLE_LOCAL_WEB_SITE[m
[32m+[m[32m#define NUM_INPUTS 2[m
[32m+[m[32m#else[m
 #define NUM_INPUTS 3[m
[32m+[m[32m#endif // ENABLE_LOCAL_WEB_SITE[m
 [m
 [m
 /********************************* Constants **********************************/[m
[31m-// Adjust these[m
[31m-// TODO This should be done using WPS.[m
[31m-const char* def_ssid = "FRITZ!Box 7430 FC";[m
[31m-const char* def_password =  "94044782303556147675";[m
[31m-//const char* def_ssid = "YourSSID";[m
[31m-//const char* def_password =  "************";[m
[31m- [m
[31m-[m
 /***************************** Struct definitions *****************************/[m
 /**************************** Prototype functions *****************************/[m
 /**************************** Variable definitions ****************************/[m
[36m@@ -66,8 +62,8 @@[m [mvoid setup() {[m
 #ifndef UNITTESTS_INSTEAD_OF_MAIN[m
   storage_setup();[m
   // Uncomment these as needed[m
[31m-  store_ssid(def_ssid, strlen(def_ssid));[m
[31m-  store_pw(def_password, strlen(def_password));[m
[32m+[m[32m  store_ssid(YOUR_SSID, strlen(YOUR_SSID));[m
[32m+[m[32m  store_pw(YOUR_PASSWORD, strlen(YOUR_PASSWORD));[m
   //dummy_eeprom_print();[m
   // Try to load ssid and pw froom eeprom, if it does not work choose default.[m
   printf("EEPROM is data available: %i\n\r", storage_data_available());[m
[36m@@ -76,8 +72,8 @@[m [mvoid setup() {[m
   {[m
     printf("Failed to load valid ssid and password from the EEPROM. "[m
             "Using the default.\n\r");[m
[31m-    strcpy(ssid, def_ssid);[m
[31m-    strcpy(password, def_password);[m
[32m+[m[32m    strcpy(ssid, YOUR_SSID);[m
[32m+[m[32m    strcpy(password, YOUR_PASSWORD);[m
   }[m
   else[m
   {[m
[36m@@ -89,8 +85,6 @@[m [mvoid setup() {[m
 [m
   setup_motor_control();[m
 [m
[31m-  //setup_local_comm();[m
[31m-[m
   /*[m
    * This allows the correct timeoffset to be used for localtime().[m
    * Do not worry about the IDE warning, the function is defined in a macro.[m
[36m@@ -111,14 +105,11 @@[m [mvoid setup() {[m
   setup_time_keeper();[m
   printf("cur time %i %i %i \n", get_current_h(), get_current_m(), get_current_s());[m
 [m
[31m-  //setup_user_comm();[m
[31m-[m
[31m-  //setup_web_comm();[m
[31m-[m
[31m-  // Setup input_handler_t[m
   setup_user_input_handler_t(&inputs[0]);[m
   setup_local_input_handler_t(&inputs[1]);[m
[32m+[m[32m#ifdef ENABLE_LOCAL_WEB_SITE[m
   setup_web_input_handler_t(&inputs[2]);[m
[32m+[m[32m#endif // ENABLE_LOCAL_WEB_SITE[m
 #endif // UNITTESTS_INSTEAD_OF_MAIN[m
 }[m
 [m
