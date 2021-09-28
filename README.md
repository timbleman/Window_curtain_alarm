# Window curtain alarm

## Basics
This project aims at creating an alarm that opens and closes a window curtain at certain times each weekday. The system is connected to the local network and is configured using socket communication.

## Features
### Must-have features
* Automatically keeping track of time
* One wake and sleep time for each weekday
* Belt driven opening and closing of curtains
* Socket based user communication
* Button for snoozing one day
* Button for manually opening and closing the curtain
### Possible future work
* Multiple wake and sleep times for each weekday
* WPS button for easy WIFI connection
* Locally hosting a website for more comfortable interaction
* Different end stop locations
### Intentional limitations
* Only accessible in the local network
* No secure communication


## Project structure
Here are some directories that may contain interesting files:
* /course_python_draft/: This quick python prototype aims at visualizing the core concepts and ideas of the project. Untested.
* /esp8266_code/test_stuff/: Some quick projects testing basic functionality using the ESP8266.
	* /esp8266_code/test_stuff/socket_test/: TCP socket server test project for the ESP8266.
* /proof_of_concept/: Unit tested project implementing generic components. This project does not strictly target embedded hardware for ease of building and debugging.


## Implementation
### Hardware
Opening and closing the curtain shall be done using a belt-driven by a stepper motor. A NEMA17 stepper in combination with a TMC2209 driver are used currently. The target platform is the ESP8266. This microcontroller has been chosen instead of a Raspberry PI due to lower costs and power consumption. 
The target 
### Mounting
The hardware, including especially the motor, belt, and end stop, will be mounted using 3D printed objects. Currently, the project is not intended as a 'one design fits all' solution, but will need to be adapted to the used curtain and window.
### Programming language and environment
Due to its dominance in embedded systems and easier debugging, C is used for most of the more generic project components. These components include user input parsing, keeping track of time, and controlling the IO.
As Arduino and most of its libraries are based on C++, the main function and socket server use this more high-level approach. The PlatformIO IDE and toolchain is used for ESP8266 code. The proof of concept has been developed using CodeLite.
### Testing
The uCunit framework has been used for unit testing. The framework is basic but very portable and requires only a few header files.

## Legacy readme stuff
_Can this be removed?_

'test'

16 03 01 02 00 01 00 01 fc 03 03 8a b2 f4 72 25 bf 2b 13 a5 0c 4f fc 73 4e d6 ab 96 80 11 ff 05 f0 57 b0 1e 4b 54 7c 3d 03 4d 9b 20 71 bd 30 14 45 ec 88 85 27 b5 3c b9 8d d8 3a 6e 77 4e 44 a8 d5 1c e7 22 61 30 25 5a e6 b5 e2 e5 00 22 7a 7a 13 01 13 02 13 03 c0 2b c0 2f c0 2c c0 30 cc a9 cc a8 c0 13 c0 14 00 9c 00 9d 00 2f 00 35 00 0a 01 00 01 91 ba ba 00 00 00 17 00 00 ff 01 00 01 00 00 0a 00 0a 00 08 fa fa 00 1d 00 17 00 18 00 0b 00 02 01 00 00 23 00 00 00 10 00 0e 00 0c 02 68 32 08 68 74 74 70 2f 31 2e 31 00 05 00 05 01 00 00 00 00 00 0d 00 14 00 12 04 03 08 04 04 01 05 03 08 05 05 01 08 06 06 01 02 01 00 12 00 00 00 33 00 2b 00 29 fa fa 00 01 00 00 1d 00 20 8f 28 6d ab 57 60 08 f9 70 73 f0 d9 22 cb 77 cd dd df 41 92 0c e4 23 2a 8e d3 68 f0 6e 4e 0c 2b 00 2d 00 02 01 01 00 2b 00 0b 0a aa aa 03 04 03 03 03 02 03 01 00 1b 00 03 02 00 02 5a 5a 00 01 00 00 15 00 e1


To run Python on Macos use https://www.youtube.com/watch?v=veJvQ88ULOM