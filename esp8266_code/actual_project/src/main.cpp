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
const char* def_ssid = "FRITZ!Box 7430 FC\0";
const char* def_password =  "94044782303556147675";
// Deg
//const char* def_ssid = "It hurts when IP";
//const char* def_password =  "SagIchDirNicht!";
#define SSID_MAX_LEN 33
 
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/**************************** Variable definitions ****************************/
WiFiServer wifiServer1(23);
WiFiClient client1;

bool connected1 = false;

// Buffers to store the ssid or password
char ssid[33];
char password[33];
int ssid_len = 0;
int pw_len = 0;


/**************************** Function definitions ****************************/
void setup() {
  Serial.begin(9600);

  delay(1000);

  storage_setup();
  // Uncomment these as needed
  store_ssid(def_ssid, strlen(def_ssid));
  store_pw(def_password, strlen(def_password));
  //dummy_eeprom_print();
  // Try to load ssid and pw froom eeprom, if it does not work choose default.
  printf("EEPROM is data available: %i\n\r", storage_data_available());
  if ((load_ssid(ssid, SSID_MAX_LEN, &ssid_len) != EXIT_SUCCESS)
      || (load_pw(password, SSID_MAX_LEN, &pw_len) != EXIT_SUCCESS))
  {
    printf("Failed to load valid ssid and password from the EEPROM. "
            "Using the default.\n\r");
    strcpy(ssid, def_ssid);
    strcpy(password, def_password);
  }
  else
  {
    printf("Loaded SSID and password from the EEPROM.\n\r");
  }
  printf("Using ssid '%s'\n\r", ssid);

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
 
  // TODO WPS button
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

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
    else if (get_local_input() == 0)
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
  else
  {
    check_and_alarm_non_blocking();
  }
}