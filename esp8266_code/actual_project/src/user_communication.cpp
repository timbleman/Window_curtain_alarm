/********************************** Includes **********************************/
#include "user_communication.h"

#include "ESP8266WiFi.h"


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/**************************** Variable definitions ****************************/
WiFiServer wifiServer(23);
WiFiClient client;
bool connected = false;



/**************************** Function definitions ****************************/
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