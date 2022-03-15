import time

# Set the number of repetitions
NUM_REP = 5
template_wake = "set_wake -d week,sat,sun -h %d -m %d \n"
template_sleep = "set_sleep -d week,sat,sun -h %d -m %d \n"


def main():
    repeated_wake_and_sleep_events()


def repeated_wake_and_sleep_events():
    """
    Writes set_wake and set_sleep commands to a file.

    :return: None.
    """
    f = open("command_list.txt", "w")

    f.write("Paste these lines into a telnet client of your choice\n")

    for i in range(0, NUM_REP):
        time_wake = get_new_time(i*2)
        f.write(template_wake % (time_wake[0], time_wake[1]))
        time_sleep = get_new_time(i * 2 + 1)
        f.write(template_sleep % (time_sleep[0], time_sleep[1]))

    f.close()


def get_new_time(extra_min: int) -> [int, int, int]:
    """

    :param extra_min: How many minutes later the new time should be.
    :return: [hour, min, sec]
    """
    time_arr = get_time_formatted()
    time_arr[1] += extra_min
    if time_arr[1] > 59:
        time_arr[1] %= 60
        time_arr[0] += 1
    return time_arr


def get_time_formatted() -> [int, int, int]:
    """

    :return: Current [hour, min, sec]
    """
    time_arr = [time.localtime().tm_hour, time.localtime().tm_min, time.localtime().tm_sec]
    return time_arr


if __name__ == '__main__':
    main()
