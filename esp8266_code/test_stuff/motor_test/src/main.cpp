#include <Arduino.h>
#include "motor_controller.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Hello!");

  setup_motor_control();

  enable_stepper();
  Serial.println("Moving in dir 0");
  for (int i = 0; i < 20; i++)
  {
    make_step(0);
  }
  delay(5000);

  Serial.println("Moving in dir 1");
  for (int i = 0; i < 10; i++)
  {
    make_step(1);
  }
  delay(5000);
  
  Serial.println("Moving with disabled stepper");
  enable_stepper();
  for (int i = 0; i < 10; i++)
  {
    make_step(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}