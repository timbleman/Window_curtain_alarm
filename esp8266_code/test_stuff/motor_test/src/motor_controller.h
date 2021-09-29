#ifndef _MOTOR_CONTROLLER_H_
#define _MOTOR_CONTROLLER_H_

/********************************** Includes **********************************/
/********************************* Constants **********************************/
/***************************** Struct definitions *****************************/
/**************************** Variable definitions ****************************/
/**************************** Prototype functions ****************************/

int setup_motor_control();

void make_step(int dir);

void enable_stepper();

void disable_stepper();

#endif // _MOTOR_CONTROLLER_H_