
# Realized as button
end_stop = 0

# Global, may be set by an interrupt
current_steps = 0

# Constant that fits most windows
DEFAULT_OPEN_STEPS = 300
open_steps = DEFAULT_OPEN_STEPS

# To deactivate the motor if desired
active = 1


def de_activate_motor():
    global active
    active = active ^ 1
    if active:
        "Activated Motor!"
    else:
        "Deactivated Motor!"


def close_blocking():
    global current_steps

    print("Closing the curtain...")
    # Interrupt instead of polling?
    while(end_stop != 0):
        current_steps += 1
        turn_step('forward')


def open_blocking():
    print("Opening the curtain...")
    for i in range(open_steps):
        turn_step('backward')


def turn_step(dir: str):
    # Some hw stuff
    return


def set_open_steps(stps: int):
    global open_steps
    open_steps = stps


def return_open_steps():
    return open_steps