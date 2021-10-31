/********************************** Includes **********************************/
#include <Arduino.h>
#include <stdint.h>
#include "ESP8266WiFi.h"
#include "user_communication.h"
#include "command_parser.h"
#include "time_keeper.h"
#include "action_executer.h"
#include "motor_controller.h"
#include "types_and_enums.h"
#include "local_communication.h"
#include "alarm_checker.h"
#include "data_storage.h" // TODO remove
#include "time.h" // setenv(), do not worry about IDE warning, compiles fine


/********************************* Constants **********************************/
// Nam
//const char* ssid = "FRITZ!Box 7430 FC";
//const char* password =  "94044782303556147675";
// Deg
const char* ssid = "It hurts when IP";
const char* password =  "SagIchDirNicht!";
 
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/**************************** Variable definitions ****************************/
WiFiServer wifiServer1(23);
WiFiClient client1;

bool connected1 = false;


/**************************** Function definitions ****************************/
void setup() {
  Serial.begin(9600);

  delay(1000);

  storage_setup();
  printf("EEPROM is data available: %i\n\r", storage_data_available());
  dummy_eeprom_print();
  char dummy_ssid[512] = "hello";
  store_ssid(dummy_ssid, 6);

  setup_motor_control();

  //setup_local_comm();

  //configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  // implement NTP update of timekeeping (with automatic hourly updates)
  //configTime("CET-1CEST,M3.5.0,M10.5.0/3", "fritz.box", "de.pool.ntp.org");
  //configTime(0, 0, "pool.ntp.org");
  /*
   * This allows the correct timeoffset to be used for localtime().
   * Do not worry about the IDE warning, the function is defined in a macro.
   */
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 3);
  //configTime("CET-1CEST,M3.5.0/02,M10.5.0/03", "pool.ntp.org");

  // info to convert UNIX time to local time (including automatic DST update)
  //setenv("TZ", "EST+5EDT,M3.2.0/2:00:00,M11.1.0/2:00:00", 1);
  /*
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  */
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  //wifiServer1.begin();

  /*
  time_t rawtime;
  struct tm currenttime;
  time(&rawtime);
  // Copy by value to not be affected by delays
  currenttime = *localtime(&rawtime);
  printf("cur time %i %i %i \n", currenttime.tm_hour, currenttime.tm_min, currenttime.tm_sec);
  */
  setup_time_keeper();
  printf("cur time %i %i %i \n", get_current_h(), get_current_m(), get_current_s());

  setup_user_comm();

  //set_wake(WED_T | TUE_T | THU_T, 15, 15, 15);
  //printf("Time until wake %lu \n", time_until_wake());
  //printf("Time until sleep %lu \n", time_until_sleep());
}

void loop() {
  static char buf[256] = {0};
  static size_t max_size = 256;
  static bool busy = false;
  static user_action_t usr_act;

  if (!busy)
  {
    if (get_user_in(buf, max_size) == 0)
    {
      usr_act = get_action(buf);
      busy = true;
    } 
    else if (get_local_input())
    {
      usr_act = fetch_local_action();
      busy = true;
    }
  }
  if (busy)
  {
    busy = execute_action_non_blocking(&usr_act, buf, max_size);
    if (!busy)
    {
      respond_to_user(buf, max_size);
      memset(buf, 0, max_size);
    }
  }

  // TODO Check if this actually is able to open/close.
  //check_and_alarm_non_blocking();
  /*
  int new_user_in_success = get_user_in(buf, max_size);
  if (new_user_in_success == 0)
  {
    printf("User in in main: %s\n", buf);
    printf("6 chars in user in in main %i %i %i %i %i %i \n", buf[0], buf[1], 
                buf[2], buf[3], buf[4], buf[5]);
    printf("6 chars in user in in main %c %c %c %c %c %c \n", buf[0], buf[1], 
                buf[2], buf[3], buf[4], buf[5]);
    //printf("%s\n", buf);
    respond_to_user(buf, max_size);

    user_action_t usr_act = get_action(buf);
    printf("Usr_act type %X \n", usr_act.act_type);

    memset(buf, 0, max_size);
  }
  */
}