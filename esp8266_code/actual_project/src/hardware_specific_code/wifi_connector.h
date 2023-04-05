#ifndef _WIFI_CONNECTOR_H_
#define _WIFI_CONNECTOR_H_

/********************************** Includes **********************************/
/********************************** Defines ***********************************/
#define NUM_CONNECTION_TRIES 15


/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions *****************************/
/*
 * Try to connect to WiFi using several different means.
 * This function blocks until success.
 *
 * @return  Success status, 0 for success.
 */
int wifi_connect();

#endif // _WIFI_CONNECTOR_H_
