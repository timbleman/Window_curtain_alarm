/********************************** Includes **********************************/
#include <Arduino.h>
#include <stdint.h>
#include "ESP8266WiFi.h"
#include "configuration.h"
#include "hardware_specific_code/networking/web_server/ota/ota.h"
#include "hardware_specific_code/networking/user_communication.h"
#include "configuration.h"
#ifndef UNITTESTS_INSTEAD_OF_MAIN
#include "portable_code/action_executer.h"
#include "portable_code/alarm_checker.h"
#include "portable_code/command_parser.h"
#include "hardware_specific_code/local_hardware/data_storage.h"
#include "hardware_specific_code/local_hardware/local_communication.h"
#include "hardware_specific_code/networking/web_server/web_communication.h"
#include "hardware_specific_code/local_hardware/motor_controller.h"
#include "hardware_specific_code/networking/wifi_connector.h"
#include "portable_code/time_keeper.h"
#include "portable_code/types_and_enums.h"
#else
#include "Testsuite.h"
#endif // UNITTESTS_INSTEAD_OF_MAIN
#include "time.h" // setenv(), do not worry about IDE warning, compiles fine


/********************************** Defines **********************************/
#define RESPOND_BUF_SIZE 256
#ifndef ENABLE_LOCAL_WEB_SITE
#define NUM_INPUTS 2
#else
#define NUM_INPUTS 3
#endif // ENABLE_LOCAL_WEB_SITE


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/**************************** Variable definitions ****************************/
#ifdef PRINT_HEAP_STATS_EVERY_MILLIS
unsigned long last_heap_print = 0;
#endif // PRINT_HEAP_STATS_EVERY_MILLIS

// Manage multiple inputs: input_handler_t 
input_handler_t inputs[NUM_INPUTS] = {0};


/**************************** Function definitions ****************************/
void setup() {
  Serial.begin(9600);
  delay(1000);

#ifndef UNITTESTS_INSTEAD_OF_MAIN
  storage_setup();

  setup_command_parser();

  setup_motor_control();

  /*
   * This allows the correct timeoffset to be used for localtime().
   * Do not worry about the IDE warning, the function is defined in a macro.
   */
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 3);
 
  wifi_connect();

  setup_time_keeper();
  printf("cur time %i %i %i \n", get_current_h(), get_current_m(), get_current_s());

  // Setup input_handler_t
  setup_user_input_handler_t(&inputs[0]);
  setup_local_input_handler_t(&inputs[1]);
#ifdef ENABLE_LOCAL_WEB_SITE
  setup_web_input_handler_t(&inputs[2]);

  setup_ota();
#endif // ENABLE_LOCAL_WEB_SITE
#endif // UNITTESTS_INSTEAD_OF_MAIN
}

void loop() {
#ifndef UNITTESTS_INSTEAD_OF_MAIN
  static char buf[RESPOND_BUF_SIZE] = {0};
  static bool busy = false;
  static user_action_t usr_act;

  // Check for new input if no action is currently executed.
  for (int i = 0; i < NUM_INPUTS; i++)
  {
    input_handler_t *current_input = &inputs[i];
    if (!busy)
    {
      if (current_input->input_available() == 0)
      {
        usr_act = current_input->fetch_action();
        busy = true;
      }
    }
  }
  
  // Execute an action and respond back to all users when done
  if (busy)
  {
    // Execute an action.
    busy = execute_action_non_blocking(&usr_act, buf, RESPOND_BUF_SIZE);
    if ((!busy) && (strlen(buf) > 0))
    {
      for (int i = 0; i < NUM_INPUTS; i++)
      {
        input_handler_t *current_input = &inputs[i];
        current_input->respond_to_user(buf, strlen(buf));
      }
      memset(buf, 0, RESPOND_BUF_SIZE);
    }
  }
  else
  {
    // Check for alarms, open or close the curtain.
    check_and_alarm_non_blocking();
  }

#ifdef PRINT_HEAP_STATS_EVERY_MILLIS
  /* 
   * There seem to be no memory leaks. 
   * I will, however, still leave this code as it does not hurt to check.
   */
  if (millis() - last_heap_print > PRINT_HEAP_STATS_EVERY_MILLIS)
  {
    printf("Free heap: %u, heap fragmentation: %u\n\r", ESP.getFreeHeap(),
            ESP.getHeapFragmentation());
    last_heap_print = millis();
  }
#endif // PRINT_HEAP_STATS_EVERY_MILLIS
#endif // UNITTESTS_INSTEAD_OF_MAIN
}
