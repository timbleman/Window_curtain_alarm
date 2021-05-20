import time
from enum import IntEnum


class wd(IntEnum):
    MON = 0
    TUE = 1
    WED = 2
    THU = 3
    FRI = 4
    SAT = 5
    SUN = 6


# These will be realized as C structs
# Instead of none, a default struct time (available in Py and C should be used)
# In Python using the time lib, this could be done much more dynamically
# However, easy portability to C should be ensured
ZERO_TIME = time.strptime("0:0", "%H:%M")
wake_times = {wd.MON.value: ZERO_TIME,
              wd.TUE.value: ZERO_TIME,
              wd.WED.value: ZERO_TIME,
              wd.THU.value: ZERO_TIME,
              wd.FRI.value: ZERO_TIME,
              wd.SAT.value: ZERO_TIME,
              wd.SUN.value: ZERO_TIME}

sleep_times = {wd.MON: ZERO_TIME,
               wd.TUE: ZERO_TIME,
               wd.WED: ZERO_TIME,
               wd.THU: ZERO_TIME,
               wd.FRI: ZERO_TIME,
               wd.SAT: ZERO_TIME,
               wd.SUN: ZERO_TIME}


def set_wake_time_from_str(in_str: str) -> int:
    """ Saves the the wakeup time of one day in the struct equivalent.
    Gets passes a string in the following format: "mon 18:00"

    :param in_str: string of the form "mon 18:00"
    :return: 0 if successful, -1 if failed
    """
    tm = time_from_str(in_str)
    weekd = week_day_from_str(in_str)

    if tm is not None and weekd is not None:
        set_wake_time(weekd, tm)
        return 0
    else:
        return -1


def set_sleep_time_from_str(in_str: str) -> int:
    """ Saves the the wakeup time of one day in the struct equivalent.
    Gets passes a string in the following format: "mon 18:00"

    :param in_str: string of the form "mon 18:00"
    :return: 0 if successful, -1 if failed
    """
    tm = time_from_str(in_str)
    weekd = week_day_from_str(in_str)

    if tm is not None and weekd is not None:
        set_sleep_time(weekd, tm)
        return 0
    else:
        return -1


def time_from_str(in_str: str):
    """ Extracts correct weekday representation from string.
    None if no valid input.
    Extracts to (Python) timestruct format. Should be replaceable by C++ chrono lib?

    :param in_str: string of the form "mon 18:00"
    :return: time in timestruct format
    """
    split_str = in_str.split(" ")
    try:
        return time.strptime(split_str[1], "%H:%M")
    except:
        print("Could not parse time from:", in_str)
        return None


# Stupid but works as an example
def week_day_from_str(in_str: str) -> wd:
    """ Extracts correct weekday representation from string.
    None if no valid input.

    :param in_str: string of the form "mon 18:00"
    :return: week day from enum wd or None
    """
    split_str = in_str.split(" ")[0]
    if split_str.upper() == "MON":
        return wd.MON
    elif split_str.upper() == "TUE":
        return wd.THU
    elif split_str.upper() == "WED":
        return wd.WED
    elif split_str.upper() == "THU":
        return wd.THU
    elif split_str.upper() == "FRI":
        return wd.FRI
    elif split_str.upper() == "SAT":
        return wd.SAT
    elif split_str.upper() == "SUN":
        return wd.SUN
    else:
        print("Could not parse week day from:", in_str)
        return None

def set_wake_time(weekday: wd, tm: time.struct_time):
    # parse string to tm using:
    #new_tm = time.strptime(time_string, "%H:%M")
    wake_times[weekday] = tm


def set_sleep_time(weekday: wd, tm: time.struct_time):
    sleep_times[weekday] = tm


def minutes_until_wake() -> int:
    cur_weekday = time.localtime().tm_wday
    wake_today = wake_times[cur_weekday]
    return minutes_until_t(wake_today)


def minutes_until_sleep() -> int:
    cur_weekday = time.localtime().tm_wday
    sleep_today = sleep_times[cur_weekday]
    return minutes_until_t(sleep_today)


def minutes_until_t(t: time.struct_time) -> int:
    cur_time = time.localtime()
    if cur_time.tm_hour > t.tm_hour:
        return float('inf')
    else:
        return (t.tm_hour - cur_time.tm_hour) * 60 \
            + t.tm_min - cur_time.tm_min
