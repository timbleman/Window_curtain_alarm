#ifndef _USER_COMMUNICATION_H_
#define _USER_COMMUNICATION_H_

/********************************** Includes **********************************/
/********************************* Constants **********************************/
#define RECV_DEBUG


/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions ****************************/
int setup_user_comm();

int get_user_in(char *buf, int buf_max_len);

int respond_to_user(char *buf, int buf_max_len);

#endif // _USER_COMMUNICATION_H_