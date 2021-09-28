#include <Arduino.h>
#include <stdint.h>
#include "ESP8266WiFi.h"

const char* ssid = "FRITZ!Box 7430 FC";
const char* password =  "94044782303556147675";
 
WiFiServer wifiServer(23);
WiFiClient client;

char buf[128] = {0};
size_t max_size = 16;

bool connected = false;

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

  wifiServer.begin();
}

void loop() {
  client = wifiServer.available();

  if (client) {
    if(client.connected() && !connected)
    {
      //client.setDefaultNoDelay(true);
      Serial.println("Client Connected");
      connected = true;
    }
    
    while(client.connected()){      
      while(client.available()>0){
        // read data from the connected client
        //char c = client.read();
        //Serial.write(c); 
        //client.read(buf, max_size);
        //client.write(client.read());
        
        // This seems to block until the number or break character is hit.
        // Additionally, exceeding the max_size number seems to break printf() 
        if (client.readBytesUntil('\n', &buf[0], max_size) > 0)
        {
          Serial.print("Received buf: ");
          //delay(50);
          Serial.println(buf);
        }
        
        //client.write(c);
      }
      //Serial.print("Received buf: ");
      //Send Data to connected client
      /*
      while(Serial.available()>0)
      {
        client.write(Serial.read());
      }
      */
    }

    if (!client.connected())
    {
      Serial.println("Client disconnected");
      client.stop();
      connected = false;
    }    
  }
}