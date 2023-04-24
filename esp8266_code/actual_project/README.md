# Curtain closer alarm ESP8266 code
This directory contains the proof_of_concept curtain closer code adapted to the ESP8266, specifically the NodeMCU V3.

## Files of interest:
To get a quick overview, I will list which files do what. To keep it compact, only the headers are listed:
* /src/hardware_specific_code/: Code specifically written for the ESP.
    * /src/hardware_specific_code/networking/:
        * /src/hardware_specific_code/web_server/: Local website, webserver and websocket for user communication. Needs arduinoWebSockets library. OTA updating code.
        * user_communication.h: Handles TCP socket setup and communication.
        * wifi_conector.h: onnect to WiFi using SSID stored in EEPROM, specified at compiletime, Serial or WPS. 
    * /src/hardware_specific_code/local_hardware/:
        * data_storage.h: Stores the configuration. Uses the EEPROM library.
        * local_communication.h: Local (hardware, button) input and display.
        * motor_controller.h: This file handles the calibration and operation of the motor.
* /src/portable_code/: Portable, unit tested code.
    * action_executer.h: Executes an action created by the command_parser.
    * alarm_checker.h: Checks whether the curtain should be opened or closed now. Has to be called in a loop. 
    * command_parser.h: Here user input strings are parsed into an action type.
    * time_keeper.h: Saves wake and sleep times for each weekday. Keeps track of time.
    * types_and_enums.h: Type and enum definitions that are shared between multiple components.
* /src/configuration.h: Configures project build and testsuite execution.
* /src/main.cpp: This is the main file running the main loop.

## Configuration
There are some configuration parameters.
SSID and your WIFI password can be set in *configuration.h*. You can also skip this step and directly throw the binary on the ESP and connect to WiFi via Serial or WPS. Look at the main README.md for further instructions.  
The UTC time offset will need to be set in main.c using the `setenv()` function.  
The speed of the stepper motor can be adjusted in motor_controller.c using `FAST_STEP_TIME` and `SLOW_STEP_TIME`.  
Most prints can be disabled using define statements.  
