#ifndef __SR04_H_
#define __SR04_H_

#include "stm32f1xx_hal.h"
#include "tim.h"

void SR04_Start(void);
float SR04_GetDistance(uint16_t count);

#endif
