#include <Arduino.h>
#include "motor_controller.h"

bool opened = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");

  setup_motor_control();

  enable_stepper();
  for(int j = 0; j < 1; j++)
  {
    Serial.println("Moving in dir 0");
    for (int i = 0; i < 200000; i++)
    {
      make_step_no_del(0);
      // yield() to reset the esp8266 watchdog timer
      yield();
    }
    delay(1000);

    Serial.println("Moving in dir 1");
    for (int i = 0; i < 100000; i++)
    {
      make_step_no_del(1);
      // yield() to reset the esp8266 watchdog timer
      yield();
    }
    delay(1000);
  }

  Serial.println("Moving with disabled stepper");
  disable_stepper();
  for (int i = 0; i < 100000; i++)
  {
    make_step_no_del(1);
    // yield() to reset the esp8266 watchdog timer
    yield();
  }

  delay(1000);

  enable_stepper();
  while(calibrate_nonblocking_rollback())
  {
    yield();
  }

}

void loop() {
  int status = 1;
  if (opened)
  {
    status = close_nonblocking();
  }
  else
  {
    status = open_nonblocking();
  }

  if (status == 0)
  {
    opened = !opened;
    delay(1000);
  }
}