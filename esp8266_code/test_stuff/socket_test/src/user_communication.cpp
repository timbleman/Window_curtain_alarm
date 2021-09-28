/********************************** Includes **********************************/
#include "user_communication.h"

#include "ESP8266WiFi.h"


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/**************************** Variable definitions ****************************/
WiFiServer wifiServer(23);
WiFiClient client;

//char buf[128] = {0};
//size_t max_size = 16;

bool connected = false;



/**************************** Function definitions ****************************/
int setup_user_comm()
{
    wifiServer.begin();
    return 0;
}

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
                // read data from the connected client
                //char c = client.read();
                //Serial.write(c); 
                //client.read(buf, max_size);
                //client.write(client.read());
                
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
                
                //client.write(c);
            }
            //Serial.print("Received buf: ");
            //Send Data to connected client
            /*
            while(Serial.available()>0)
            {
                client.write(Serial.read());
            }
            */
        }

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