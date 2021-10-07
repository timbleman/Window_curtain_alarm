/********************************** Includes **********************************/
#include <stdio.h>
#include <string.h>
#include "action_executer.h"
#include "command_parser.h"
#include "motor_controller.h"
#include "time_keeper.h"


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
int curtain_control_from_act(user_action_t *act,
                                char *message,
                                int message_max_len);
/**************************** Variable definitions ****************************/
/**************************** Function definitions ****************************/
/*
 * Execute an user_action_t entity.
 * Nonblocking: Return 0 if finished, 1 otherwise.
 * If finished, a message is copied into the supplied location.
 * 
 * @param act: user_action_t to execute.
 * @param message: String location to copy into.
 * @param message_max_len: Max string length.
 * @return: 0 if finished, 1 if still working.
 */
int execute_action_non_blocking(user_action_t *act,
                                char *message,
                                int message_max_len)
{
    // Perform message size check. Make sure message fits.
    const int REQUIRED_LEN = 128;
    if (message_max_len < REQUIRED_LEN)
        return 0;
        
    int status = 0;
    int internal_status = 0;
    
    // Decide what to do.
    switch(act->act_type)
    {
        case NONE_T:    strcpy(message, "NONE_T action\n");
                        break;
        case WAKE_SET_T:    // Check if the parser appended errors to the first data field.
                            if ((act->data[0] & ALL_ERRS) == 0)
                            {
                                internal_status = set_wake(act->data[0], 
                                                act->data[1], act->data[2], 
                                                act->data[3]);
                            }
                            else
                            {
                                internal_status |= (act->data[0] & ALL_ERRS);
                            }
                            // Message differs if error occured or not.
                            if ((internal_status & ALL_ERRS) != 0)
                            {
                                const char wake_err_str[] = 
                                        "Setting wake caused errors: ";
                                strcpy(message, wake_err_str);
                                /*
                                 * Write errors to end of message.
                                 * Overwrite the current '\0'.
                                 * Pass the space left in the buffer.
                                 */
                                get_message_from_errors(internal_status, 
                                        &message[strlen(wake_err_str)], 
                                        (message_max_len - strlen(message)));
                            }
                            else
                            {
                                sprintf(message, 
                                        "Successfully set wake days 0x%X, h %i, m %i, s %i\n",
                                        act->data[0], act->data[1], act->data[2], 
                                            act->data[3]);
                            }
                            break;
        case SLEEP_SET_T:   // Check if the parser appended errors to the first data field.
                            if ((act->data[0] & ALL_ERRS) == 0)
                            {
                                internal_status = set_sleep(act->data[0], 
                                                act->data[1], act->data[2], 
                                                act->data[3]);
                            }
                            else
                            {
                                internal_status |= (act->data[0] & ALL_ERRS);
                            }
                            // Message differs if error occured or not.
                            if ((internal_status & ALL_ERRS) != 0)
                            {
                                const char wake_err_str[] = 
                                        "Setting sleep caused errors: ";
                                strcpy(message, wake_err_str);
                                // See WAKE_SET_T case
                                get_message_from_errors(internal_status, 
                                        &message[strlen(wake_err_str)], 
                                        (message_max_len - strlen(message)));
                            }
                            else
                            {
                                sprintf(message, 
                                        "Successfully set sleep days 0x%X, h %i, m %i, s %i\n",
                                        act->data[0], act->data[1], act->data[2], 
                                            act->data[3]);
                            }
                            break;
        case CURTAIN_CONTROL_T:     status = curtain_control_from_act(act, message, 
                                                        message_max_len);
                                    break;
        case CURTIME_T: sprintf(message, "Current internal time: %i:%i:%i\n",
                            get_current_h(), get_current_m(), get_current_s());
                        break;
        case HELP_T:    sprintf(message, "Example commands:\n"
                                            "set_wake -d mon,tue -h 7 -m 22 -s 0\n"
                                            "set_sleep -d week,weekend -h 7 -m 22 -s 0\n"
                                            "curtime\n"
                                            "open\n"
                                            "close\n"
                                            "calibrate\n"
                                            "curtainxor\n"
                                            "waketimes\n"
                                            "sleeptimes\n");
                        break;
        case WAKE_TIMES_T:  status = write_wake_times_message(message, message_max_len);
                            break;
        case SLEEP_TIMES_T: status = write_sleep_times_message(message, message_max_len);
                            break;
        case ERROR_T:   strcpy(message, "ERROR_T action\n");
                        break;
        default:    strcpy(message, "Invalid action_type\n");
                    break;
    }

    return status;
}

int curtain_control_from_act(user_action_t *act,
                                char *message,
                                int message_max_len)
{
    if (act->act_type != CURTAIN_CONTROL_T)
        return 1;
    // TODO String bounds checking.
        
    int status = 0;
    // Check which curtain control should be performed.
    switch(act->data[0])
    {
        case OPEN_T:    //status = open_nonblocking();
                        // Write message if successful
                        if (!status)
                        {
                            strcpy(message, "Opened curtain\n");
                        }
                        break;
        case CLOSE_T:   //status = close_nonblocking();
                        // Write message if successful
                        if (!status)
                        {
                            strcpy(message, "Closed curtain\n");
                        }
                        break;
        case CALIBRATE_T:   //status = calibrate_nonblocking();
                            // Write message if successful
                            if (!status)
                            {
                                strcpy(message, "Calibrated curtain end stop\n");
                            }
                            break;
        case CURTAIN_ERROR_T:   strcpy(message, "CURTAIN_ERROR_T action!\n");
                                break;
        default:    strcpy(message, "Invalid curtain action data!\n");
                    break;
    }
    
    return status;
}