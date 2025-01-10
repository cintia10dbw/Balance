#include "encoder.h"

int Read_Speed(TIM_HandleTypeDef *htim)
{
    int temp;
    temp = (short)__HAL_TIM_GET_COUNTER(htim);
    __HAL_TIM_SET_COUNTER(htim, 0);
    return temp;
}