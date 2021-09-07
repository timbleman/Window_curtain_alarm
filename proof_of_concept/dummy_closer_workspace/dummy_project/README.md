# Window curtain alarm proof of concept


## Intro
This project serves as quick prototype for a window curtain alarm.
Basic functions are implemented and tested without the actual hardware:
Parsing strings, setting and keeping track of time will be fully implemented.
The motor control and socket interaction will only be implemented as a dummy, as this is highly platform dependent.

## Files and contents
This section quickly discusses the files and their contents.
For simplicity reasons, only the h-file is described, not the implemention.
* command_parser.h: Here user input strings are parsed into an action type.
* main.c: This is the main file running the main loop.
* motor_controller.h: This file handles the calibration and operation of the motor.
* System.c: This file is for the uCUnit framework. The print function has to be changed when porting. 
* Testsuite.h: The actual test cases run by the uCUnit framework. 
* time_keeper.h: Saves wake and sleep times for each weekday. Keeps track of time.
* uCUnit-v1.0.h: A lightweight unit testing framework.