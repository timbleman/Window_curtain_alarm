import socket
import time

import InputParser
import MotorHandler
import TimeKeeper
from typedefs import coms, COMMAND_TYPE, DAT_PRSD

PORT = 4000

def main():
    # Setup hardware
    # Setup socket
    sckt = socket.socket()
    sckt.bind(('', PORT))
    print("Socket bound to", PORT)
    sckt.listen(5)
    sckt.setblocking(False)

    user_input = ""

    # Infinite loop that waits vor clients or an alarm time to happen
    # For simple testing without unconditional loop:
    deltime = 1000
    while True:
    #for i in range(1):
        try:
            conn, addr = sckt.accept()
            print(addr, "connected!")
            conn.setblocking(False)
            user_input = conn.recv(1024)
            print("user_input", user_input)
            print("Received data: ", user_input)
            deltime = 1000
        except:
            deltime = 1
            pass
        if not user_input:
            user_input = ""
        #if Socket available:
        #       user_input = Socket read; such stuff, should be always listening
        # Feed some dummy input
        #user_input = "SET_WAKE THU 22:00"
        parsed_in = InputParser.parse_str(user_input)
        print("Parsed to:", parsed_in)

        # Choose an appropriate action for the command type
        # If/elif/else as there is no switch in python
        # Make sure each command type is covered to create latch like behaviour
        status = 0
        if parsed_in[COMMAND_TYPE] == coms.OPEN:
            MotorHandler.open_blocking()
        elif parsed_in[COMMAND_TYPE] == coms.CLOSE:
            MotorHandler.close_blocking()
        elif parsed_in[COMMAND_TYPE] == coms.SET_WAKE:
            status = TimeKeeper.set_wake_time_from_str(parsed_in[DAT_PRSD])
            if status != 0:
                print("Setting the desired wake time failed!")
        elif parsed_in[COMMAND_TYPE] == coms.SET_SLEEP:
            status = TimeKeeper.set_sleep_time_from_str(parsed_in[DAT_PRSD])
            if status != 0:
                print("Setting the desired sleep time failed!")
        elif parsed_in[COMMAND_TYPE] == coms.DE_ACTIVATE:
            MotorHandler.de_activate_motor()
        elif parsed_in[COMMAND_TYPE] == coms.NOTHING:
            pass
        elif parsed_in[COMMAND_TYPE] == coms.ERROR:
            print("The command type could not be extracted from the command!")
            exit(0)

        print(TimeKeeper.minutes_until_wake(), "minutes until wake")
        if TimeKeeper.minutes_until_wake() <= 2:
            MotorHandler.open_blocking()

        print(TimeKeeper.minutes_until_sleep(), "minutes until sleep")
        if TimeKeeper.minutes_until_sleep() <= 2:
            MotorHandler.close_blocking()

        #if parsed_in[COMMAND_TYPE] != coms.NOTHING:
        time.sleep(deltime)

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main()