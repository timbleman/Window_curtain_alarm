/********************************** Includes **********************************/
#include <Arduino.h>
#include <stdint.h>
#include "ESP8266WiFi.h"
#include "user_communication.h"


/********************************* Constants **********************************/
const char* ssid = "FRITZ!Box 7430 FC";
const char* password =  "94044782303556147675";

 
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/**************************** Variable definitions ****************************/
WiFiServer wifiServer1(23);
WiFiClient client1;

bool connected1 = false;

char buf[128] = {0};
size_t max_size = 16;


/**************************** Function definitions ****************************/
void setup() {
  Serial.begin(9600);
 
  delay(1000);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  //wifiServer1.begin();

  setup_user_comm();
}

void loop() {
  int new_user_in_success = get_user_in(buf, max_size);
  if (new_user_in_success == 0)
  {
    printf("User in in main: %s\n", buf);
    //printf("%s\n", buf);
    respond_to_user(buf, max_size);
    memset(buf, 0, max_size);
  }
}