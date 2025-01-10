#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f1xx_hal.h"
#include "tim.h"

void motor_Init(void);
void motor_set_speed(int speed1, int speed2);
void motor_limit_speed(int *speed1, int *speed2);

#endif /* MOTOR_H_ */