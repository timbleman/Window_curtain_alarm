/********************************** Includes **********************************/
#include "wifi_connector.h"
#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "data_storage.h"
#include "../configuration.h"


/********************************* Constants **********************************/
// States for the Serial connection state machine.
enum SERIAL_CON_STATES {
    SERIAL_ENTER_NONE = 1,
    SERIAL_ENTER_SSID = 2,
    SERIAL_ENTER_PW = 4
};


/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
/***************************** Struct definitions *****************************/
/**************************** Prototype functions *****************************/
int wifi_connect_address(const char* ssid, const char* pw, size_t num_tries);
int ssid_pw_from_serial();
int read_string_until_noblocking(char *buf, size_t buflen, char terminator1, 
                                    char terminator2);
void clear_serial_rx_buf();


/**************************** Variable definitions ****************************/
// Buffers to store the ssid or password
static char ssid[SSID_MAX_LEN];
static char password[SSID_MAX_LEN];
static int ssid_len = 0;
static int pw_len = 0;


/**************************** Function definitions ****************************/
/*
 * Try to connect to WiFi using several different means.
 * This function blocks until success.
 *
 * @return  Succes status, 0 for success.
 */
int wifi_connect()
{
    int status = 1;
        
    /*
     * Try to load ssid and pw from eeprom, if it does not work choose default.
     */
    printf("EEPROM is data available: %i\n\r", storage_data_available());
    if ((load_ssid(ssid, SSID_MAX_LEN, &ssid_len) != EXIT_SUCCESS)
        || (load_pw(password, SSID_MAX_LEN, &pw_len) != EXIT_SUCCESS))
    {
        printf("Failed to load valid ssid and password from the EEPROM. "
                "Using the default %s.\n\r", YOUR_SSID);
        strcpy(ssid, YOUR_SSID);
        strcpy(password, YOUR_PW);
        store_ssid(YOUR_SSID, strlen(YOUR_SSID));
        store_pw(YOUR_PW, strlen(YOUR_PW));
    }
    else
    {
        printf("Loaded SSID %s and password of len %u from the EEPROM.\r\n", 
                ssid, strlen(password));
    }
    
    /*
     * Get SSID and password using the Serial console and try to connect.
     */ 
    while (wifi_connect_address((const char*) ssid, (const char*) password, 
            NUM_CONNECTION_TRIES) != 0)
    {
        while (ssid_pw_from_serial() != 0)
        {
            yield();
        }
        Serial.print("Trying ssid ");
        Serial.print(ssid);
        Serial.print(" and pw of length ");
        Serial.println(strlen(password));
    }

    return status;
}

/*
 * Read SSID and password from the Serial Terminal.
 * Nonblocking, needs to be called until success.
 * Uses a state machine.
 * 
 * @return  Success status, 0 for success.
 */
int ssid_pw_from_serial()
{
    static enum SERIAL_CON_STATES serial_con_state = SERIAL_ENTER_NONE;
    static char local_buf[SSID_MAX_LEN] = {0};

    int status = 1;

    switch (serial_con_state)
    {
        case SERIAL_ENTER_NONE: serial_con_state = SERIAL_ENTER_SSID;
                                printf("Enter your SSID:\r\n");
                                memset((void *)local_buf, 0, 
                                        SSID_MAX_LEN * sizeof(char));
                                clear_serial_rx_buf();
                                break;
        case SERIAL_ENTER_SSID: {
                                if (read_string_until_noblocking(local_buf, 
                                    SSID_MAX_LEN, '\n', '\r') == 0)
                                {
                                    memset((void *)ssid, 0, 
                                            SSID_MAX_LEN * sizeof(char));
                                    strncpy(ssid, (const char*)local_buf, 
                                            SSID_MAX_LEN);
                                    ssid[strlen(local_buf)] = '\0';

                                    serial_con_state = SERIAL_ENTER_PW;
                                    memset((void *)local_buf, 0, 
                                            SSID_MAX_LEN * sizeof(char));
                                    printf("Enter your password:\r\n");
                                    clear_serial_rx_buf();
                                }
                                break;
                                }
        case SERIAL_ENTER_PW:   {
                                if (read_string_until_noblocking(local_buf, 
                                    SSID_MAX_LEN, '\n', '\r') == 0)
                                {
                                    memset((void *)password, 0, 
                                            SSID_MAX_LEN * sizeof(char));
                                    strncpy(password, (const char*)local_buf, 
                                            SSID_MAX_LEN);
                                    serial_con_state = SERIAL_ENTER_NONE;
                                    memset((void *)local_buf, 0, 
                                            SSID_MAX_LEN * sizeof(char));
                                    printf("Received ssid %s and password of"
                                            " length %u.\r\n", 
                                            ssid, strlen(password));
                                    clear_serial_rx_buf();
                                    status = 0;
                                }
                                break;
                                }
        default: printf("Undefined state in ssid_pw_from_serial!\n\r");
    }

    return status;
}

/*
 * Read a string from the Serial rx buffer until one of two terminators is
 * reached. You need up to two teminators, as hitting  the "Enter"-key
 * writes both '\r' and '\n' into the buffer. Which of these comes first
 * may depend on your Serial terminal! Remember to use a clean rx buffer
 * to prevent unexpected behaviour. The string terminator is not copied into
 * the buf. Nonblocking, needs to be called until success.
 * 
 * @param   buf         The buffer to copy the received string into.
 * @param   buflen      The max string length than can fit in the buffer.
 * @param   terminator1 First string terminator, stop when encountered.
 * @param   terminator2 Second string terminator, stop when encountered.
 * @return  Success status, 0 for success.
 */
int read_string_until_noblocking(char *buf, size_t buflen, char terminator1, 
                                    char terminator2)
{
    int status = 1;

    size_t available_chars = Serial.available();
    if (available_chars > 0)
    {
        size_t index = strlen(buf);
        size_t space_in_buf = buflen - index - 1;

        size_t els_to_cpy = available_chars > space_in_buf ? 
                            space_in_buf : available_chars;

        size_t num_copied = 0;
        char last_char = ' ';
        while(num_copied < els_to_cpy && 
                (last_char != terminator1 && last_char != terminator2))
        {
            last_char = Serial.read();
            buf[index] = last_char;
            Serial.print('*');
            num_copied++;
            index++;
            // yield?
        }

        if (last_char == terminator1 || last_char == terminator2)
        {
            buf[index - 1] = '\0';
            status = 0;
            Serial.print("\r\n");
            clear_serial_rx_buf();
        }
    }

    return status;
}

/*
 * Remove all availble chars from the Serial rx buffer.
 * Call this before prompting the user to input something.
 * 
 * @return  None.
 */
void clear_serial_rx_buf()
{
    while (Serial.available())
        Serial.read();
}

/*
 * Try to connect to a WiFi a few times.
 *
 * @param   ssid        The SSID of the network.
 * @param   pw          The password of the network.
 * @param   num_tries   How often to try until it gives up.
 * @return  Success status, 0 if connection was successful.
 */
int wifi_connect_address(const char* ssid, const char* pw, size_t num_tries)
{
    int status = 1;

    WiFi.begin(ssid, pw);

    size_t tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < num_tries) {
        delay(1000);
        Serial.println("Connecting..");
        tries++;
    }
    
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.printf("Connected to WiFi %s IP:", ssid);
        Serial.println(WiFi.localIP());
        // Store ssid and pw if successful
        store_ssid(ssid, strlen(ssid));
        store_pw(pw, strlen(pw));

        status = 0;
    }
    else
    {
        delay(100);
        printf("Could not connect to %s, pw of len %u within %u tries\r\n", 
                ssid, strlen(pw), num_tries);
        delay(100);
        WiFi.disconnect(true);
    }

    return status;
}
