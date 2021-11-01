/********************************** Includes **********************************/
#include "data_storage.h"
#include <stdio.h>
#include <string.h>
#include "EEPROM.h"


/********************************* Constants **********************************/
// 1 Kb for now
#define EEPROM_MAX_SIZE 0x400
#define SSID_MAX_SIZE 32
#define SSID_EEPROM_OFFSET 0
#define PW_EEPROM_OFFSET ((SSID_MAX_SIZE) + 4)
#define TIMES_EEPROM_OFFSET 96
#define TIMES_EL_SIZE (sizeof(uint8_t) * 4)

// Debugging data storage
#define EEPROM_DEBUGGING_PRINTS

/***************************** Struct definitions *****************************/


/**************************** Prototype functions *****************************/
void dummy_eeprom_put(int const address, uint8_t const val);
uint8_t dummy_eeprom_read(int const address);
int store_generic_string(const char *str1, int str_len, int max_size, 
                            size_t mem_offset);
int load_generic_string(char *buf1, int buf_max_len, int *str_len, 
                            size_t mem_offset);


/**************************** Variable definitions ****************************/
// Dummy eeprom for unit testing
static uint8_t dummy_eeprom[512] = {0};


/**************************** Function definitions ****************************/
/*
 * Sets up the eeprom.
 * 
 * @return: None.
 */
void storage_setup()
{
    EEPROM.begin(256);
}

/*
 * Checks whether stored data is available.
 * 
 * @return: 0 if no data, 1 if there is stored data. No bool due to portability.
 */
int storage_data_available()
{
    int status = 0;
    
    /*
     * When storing password or ssid, the lenght is saved.
     * If the length in the storage does not match the actual strings lenght,
     * an error code is returned. This is used to detect valid storage status.
     */
    char dummy_buf[33] = {0};
    int dummy_len = 0;
    status |= load_ssid(dummy_buf, 33, &dummy_len);
    //printf("status after ssid %i, dummy buf ssid: %s \n\r", status, dummy_buf);
    status |= load_pw(dummy_buf, 33, &dummy_len);
    //printf("status after pw %i, dummy buf pw: %s \n\r", status, dummy_buf);

    // If an error occured --> status == 1 --> no data available
    return !status;
}

/*
 * Store the ssid.
 * 
 * @param ssid: The ssid string to store.
 * @param ssid_len: The length of the ssid.
 * @return: Success
 */
int store_ssid(const char *ssid, int ssid_len)
{
    return store_generic_string(ssid, ssid_len, SSID_MAX_SIZE, 
                                SSID_EEPROM_OFFSET);
}

/*
 * Load the ssid. 
 * Fails if the saved length does not match the strings length.
 * 
 * @param ssid: The buffer to copy the ssid into. 
 * @param buf_max_len: The buffer length.
 * @param ssid_len: Out param for the ssid length.
 * @return: Success.
 */
int load_ssid(char *ssid, int buf_max_len, int *ssid_len)
{
    return load_generic_string(ssid, buf_max_len, ssid_len,
                                SSID_EEPROM_OFFSET);
}

/*
 * Store the password.
 * 
 * @param pw: The password string to store.
 * @param pw_len: The length of the password.
 * @return: Success
 */
int store_pw(const char *pw, int pw_len)
{
    return store_generic_string(pw, pw_len, SSID_MAX_SIZE, 
                                PW_EEPROM_OFFSET);
}

/*
 * Load the password. 
 * Fails if the saved length does not match the strings length.
 * 
 * @param password: The buffer to copy the password into. 
 * @param buf_max_len: The buffer length.
 * @param password_len: Out param for the password length.
 * @return: Success.
 */
int load_pw(char *pw, int buf_max_len, int *pw_len)
{
    return load_generic_string(pw, buf_max_len, pw_len,
                                PW_EEPROM_OFFSET);
}

/*
 * Stores a string in eeprom.
 * 
 * @param str1: The str to store.
 * @param str_len: The length of the str.
 * @param max_size: The maximum size in the eeprom.
 * @param mem_offset: Offset in the eeprom.
 * @return: Success
 */
int store_generic_string(const char *str1, int str_len, int max_size, 
                            size_t mem_offset)
{
    if (str_len > max_size || (size_t)str_len != strlen(str1))
    {
#ifdef EEPROM_DEBUGGING_PRINTS
        printf("String len %i missmatches with passed str_len %i or "
                "max_size %i.\n\r", strlen(str1), str_len, max_size);
#endif // EEPROM_DEBUGGING_PRINTS
        return 1;
    }
    
    EEPROM.write(mem_offset, (uint8_t)str_len);
    for (int i = 0; i < str_len; i++)
    {
        EEPROM.write(mem_offset + i + 1, str1[i]);
    }
    for (int i = str_len; i < max_size; i++)
    {
        EEPROM.write(mem_offset + i + 1, 0);
    }
    
    //EEPROM.write(str_len, 0);
    EEPROM.commit();

    return 0;
}

/*
 * Load a string from the eeprom. 
 * Fails if the saved length does not match the strings length.
 * 
 * @param buf1: The buffer to copy the str into. 
 * @param buf_max_len: The buffer length.
 * @param str_len: Out param for the str length.
 * @param mem_offset: Offset in the eeprom.
 * @return: Success.
 */
int load_generic_string(char *buf1, int buf_max_len, int *str_len, 
                            size_t mem_offset)
{
    size_t saved_str_len = EEPROM.read(mem_offset);
    if ((size_t)buf_max_len < saved_str_len)
        return 1;
        
    *str_len = saved_str_len;
    for (size_t i = 0; i < saved_str_len; i++)
    {
        buf1[i] = EEPROM.read(mem_offset + i + 1);
    }
    
    // Check if the right length has been saved, else unsuccessful.
    //printf("buf1 when loading from eeprom: %s \n\r", buf1);
    //printf("strlen(buf1) %i, saved_str_len %i \n\r", strlen(buf1), saved_str_len);
    if (strlen(buf1) != saved_str_len)
    {
        memset(buf1, 0, buf_max_len);
        *str_len = 0;
        return 1;
    }
    
    return 0;
}

/*
 * Stores a time consisting of day, hour, minute and second.
 * Multiple times can be saved.
 * 
 * @param time_num: The number of the current time.
 * @param day: The day to save.
 * @param h: The hour to save.
 * @param m: The minute to save.
 * @param s: The second to save.
 * @return: None.
 */
void store_time(int time_num, uint8_t day, uint8_t h, uint8_t m, uint8_t s)
{
    EEPROM.write((TIMES_EEPROM_OFFSET + time_num * TIMES_EL_SIZE + 0 * sizeof(uint8_t)), day);
    EEPROM.write((TIMES_EEPROM_OFFSET + time_num * TIMES_EL_SIZE + 1 * sizeof(uint8_t)), h);
    EEPROM.write((TIMES_EEPROM_OFFSET + time_num * TIMES_EL_SIZE + 2 * sizeof(uint8_t)), m);
    EEPROM.write((TIMES_EEPROM_OFFSET + time_num * TIMES_EL_SIZE + 3 * sizeof(uint8_t)), s);
    
    EEPROM.commit();
}

/*
 * Loads a time consisting of day, hour, minute and second into pointers.
 * 
 * @param time_num: The number of the desired time.
 * @param day: The loaded day.
 * @param h: The loaded hour.
 * @param m: The loaded minute.
 * @param s: The loaded second.
 * @return: None.
 */
void load_time(int time_num, uint8_t *day, uint8_t *h, uint8_t *m, uint8_t *s)
{
    *day = EEPROM.read(TIMES_EEPROM_OFFSET + time_num * TIMES_EL_SIZE);
    *h = EEPROM.read(TIMES_EEPROM_OFFSET + time_num * TIMES_EL_SIZE + 1 * sizeof(uint8_t));
    *m = EEPROM.read(TIMES_EEPROM_OFFSET + time_num * TIMES_EL_SIZE + 2 * sizeof(uint8_t));
    *s = EEPROM.read(TIMES_EEPROM_OFFSET + time_num * TIMES_EL_SIZE + 3 * sizeof(uint8_t));
}

void dummy_eeprom_print()
{
    printf("Eeprom stuff: \n");
    for (int i = 0; i < 64; i++)
    {
        printf("%c ", EEPROM.read(i));
    }
    printf("\n\r");
}

void dummy_eeprom_put(int const address, uint8_t const val)
{
    dummy_eeprom[address] = val;
}

uint8_t dummy_eeprom_read(int const address)
{
    return dummy_eeprom[address];
}

