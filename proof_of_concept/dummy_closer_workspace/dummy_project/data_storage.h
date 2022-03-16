/*
 * Data storage saves data that needs to be persistent.
 * The implementation is highly hardware specific and needs to be adjusted.
 */

#ifndef _DATA_STORAGE_H_
#define _DATA_STORAGE_H_

/********************************** Includes **********************************/
#include <stdint.h>


/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
/*
 * Sets up the eeprom.
 * 
 * @return: None.
 */
int setup_storage();

/*
 * Checks whether stored data is available.
 * 
 * @return: 0 if no data, 1 if there is stored data. No bool due to portability.
 */
int storage_data_available();

/*
 * Store the ssid.
 * 
 * @param ssid: The ssid string to store.
 * @param ssid_len: The length of the ssid.
 * @return: Success
 */
int store_ssid(char *ssid, int ssid_len);

/*
 * Load the ssid. 
 * Fails if the saved length does not match the strings length.
 * 
 * @param ssid: The buffer to copy the ssid into. 
 * @param buf_max_len: The buffer length.
 * @param ssid_len: Out param for the ssid length.
 * @return: Success.
 */
int load_ssid(char *ssid, int buf_max_len, int *ssid_len);

/*
 * Store the password.
 * 
 * @param pw: The password string to store.
 * @param pw_len: The length of the password.
 * @return: Success
 */
int store_pw(char *pw, int pw_len);

/*
 * Load the password. 
 * Fails if the saved length does not match the strings length.
 * 
 * @param password: The buffer to copy the password into. 
 * @param buf_max_len: The buffer length.
 * @param password_len: Out param for the password length.
 * @return: Success.
 */
int load_pw(char *pw, int buf_max_len, int *pw_len);

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
void store_time(int time_num, uint8_t day, uint8_t h, uint8_t m, uint8_t s);

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
void load_time(int time_num, uint8_t *day, uint8_t *h, uint8_t *m, uint8_t *s);

#endif // _DATA_STORAGE_H_
