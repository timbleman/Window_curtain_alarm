# Curtain closer alarm ESP8266 code
This directory contains the proof_of_concept curtain closer code adapted to the ESP8266, specifically the NodeMCU V3.

## Files of interest:
To get a quick overview, I will quickly list which files do what. To keep it compact, only the headers are listed:
* /src/action_executer.h: Executes an action created by the command_parser.
* /src/alarm_checker.h: Checks whether the curtain should be opened or closed now. Has to be called in a loop. 
* /src/command_parser.h: Here user input strings are parsed into an action type.
* /src/configuration.h: Configures project build and testsuite execution.
* /src/data_storage.h: Stores the configuration. Uses the EEPROM library.
* /src/local_communication.h: Local (hardware, button) input and display.
* /src/main.cpp: This is the main file running the main loop.
* /src/motor_controller.h: This file handles the calibration and operation of the motor.
* /src/time_keeper.h: Saves wake and sleep times for each weekday. Keeps track of time.
* /src/types_and_enums.h: Type and enum definitions that are shared between multiple components.
* /src/user_communication.h: Handles TCP socket setup and communication.
