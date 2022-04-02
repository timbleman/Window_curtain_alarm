/********************************** Includes **********************************/
#include "user_communication.h"
#include <string.h>
#include <stdbool.h>
#include "ESP8266WiFi.h"
#include "../portable_code/command_parser.h"


/********************************** Defines **********************************/
#define USER_SCKT_BUF_LEN 256


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/**************************** Variable definitions ****************************/
WiFiServer wifiServer(23);
WiFiClient client;
bool connected = false;

char user_socket_buf[USER_SCKT_BUF_LEN] = {};
bool user_data_available = false;


/**************************** Function definitions ****************************/
int setup_user_input_handler_t(input_handler_t *inh)
{
    inh->setup = setup_user_comm;
    inh->input_available = get_user_in1;
    inh->fetch_action = fetch_user_action;
    inh->respond_to_user = respond_to_user;
    strncpy(inh->tag, "telnet", TAG_LEN);

    return inh->setup();
}

/*
 * Setup the user communication.
 * 
 * @return: Success status.
 */
int setup_user_comm()
{
    wifiServer.begin();
    return 0;
}

/* 
 * Update and check for user inputs.
 * Has to be called repeatedly.
 * Returns success if an input is available to be fetched.
 * 
 * @return: 0 if input is available, 1 if not.
 */
int get_user_in1()
{
    return get_user_in(user_socket_buf, USER_SCKT_BUF_LEN);
}

/* 
 * Fetch an action if available. 
 * Pass by value with copying is used, should not be a problem for this small
 * size.
 * 
 * @return: A user_action_t.
 */
user_action_t fetch_user_action()
{
    user_data_available = false;

    user_action_t new_act = {};
    new_act = get_action(user_socket_buf);

    if (strlen(user_socket_buf) > 0)
        memset(user_socket_buf, 0, strlen(user_socket_buf) * sizeof(char));

    return new_act;
}

/*
 * Check for user input.
 * Stores the input in the buffer and returns 0 if successful.
 * 
 * @param buf: Buffer to store the input into.
 * @param buf_max_len: The size of the buffer.
 * @return: 0 if success and new input, else 1.
 */
int get_user_in(char *buf, int buf_max_len)
{
    int receive_success = 1;
    if (!connected)
    {
        client = wifiServer.available();
    }

    if (client) {
        if(client.connected() && !connected)
        {
        //client.setDefaultNoDelay(true);
#ifdef RECV_DEBUG
        Serial.println("Client Connected");
#endif // RECV_DEBUG
        connected = true;
        }
        
        if(client.connected()){      
            while(client.available()>0){                
                // This seems to block until the number or break character is hit.
                // Additionally, exceeding the max_size number seems to break printf() 
                if (client.readBytesUntil('\n', &buf[0], buf_max_len) > 0)
                {
#ifdef RECV_DEBUG
                    Serial.print("Received buf: ");
                    //delay(50);
                    Serial.println(buf);
#endif // RECV_DEBUG
                    receive_success = 0;
                }
            }
        }
    }

    if (connected)
    {
        if (!client.connected())
        {
#ifdef RECV_DEBUG
            Serial.println("Client disconnected");
#endif // RECV_DEBUG
            client.stop();
            connected = false;
        }  
    }

    //printf("Success stat %i \n", receive_success);
    return receive_success;
}

/* 
 * Send text back to the user. 
 * Does not send if the user is no longer connected.
 *
 * @param buf: The string to send back.
 * @param buf_max_len: The maximum number of chars to send.
 * @return: Success status.
 */
int respond_to_user(char *buf, int buf_max_len)
{
    if (!connected || !client || !client.connected())
    {
        return 1;
    }
    
    int status = client.printf("%s\n", buf);

    if (status != 0)
        return 0;
    else
        return 1;
}