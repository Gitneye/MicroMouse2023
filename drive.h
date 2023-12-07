#ifndef _DRIVE_H
#define _DRIVE_H

#include <stdint.h>

enum Motor {
	LEFT,
	RIGHT
};

void motor_initialise();

void motor_off();

void motor_speed(enum Motor, float speed);

void rotate(float angle);

void drive_to_next_cell();

#endif //_DRIVE_H