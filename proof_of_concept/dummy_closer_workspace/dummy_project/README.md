# Window curtain alarm proof of concept


## Intro
This project serves as a quick prototype for a window curtain alarm.
Basic functions are implemented and tested without the actual hardware:
Parsing strings, setting, and keeping track of time will be fully implemented.
New code should be tested at first in this project before being ported to a MCU.
The motor control and socket interaction will only be implemented as a dummy, as these are highly platform-dependent.
Extensive unit testing using the uCUnit framework should reduce the amount of debugging on the embedded platform.

## Files and contents
This section quickly discusses the files and their contents.
For simplicity reasons, only the h-file is described, not the implementation.
* action_executer.h: Executes an action created by the command_parser.
* alarm_checker.h: Checks whether the curtain should be opened or closed now. Has to be called in a loop. 
* command_parser.h: Here user input strings are parsed into an action type.
* configuration.h: Configures project build and test suite execution.
* data_storage.h: A dummy file to test storing data. Resembles the EEPROM library. 
* main.c: This is the main file running the main loop.
* motor_controller.h: This file handles the calibration and operation of the motor.
* System.c: This file is for the uCUnit framework. The print function has to be changed when porting. 
* Testsuite.h: The actual test cases run by the uCUnit framework. 
* time_keeper.h: Saves wake and sleep times for each weekday. Keeps track of time.
* types_and_enums: Type and enum definitions that are shared between multiple components.
* uCUnit-v1.0.h: A lightweight unit testing framework.