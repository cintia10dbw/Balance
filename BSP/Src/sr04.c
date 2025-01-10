#include "sr04.h"
#include "pid.h"

uint16_t SR04_Count;
float SR04_Distance;

void SR04_Start(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    Delay_us(30);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
}

float SR04_GetDistance(uint16_t count)
{
    float distance = 0.0;
    distance = (count * 0.034) / 2;
    return distance;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_2)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_SET)
        {
            __HAL_TIM_SetCounter(&htim3, 0);
            HAL_TIM_Base_Start(&htim3);
        }
        else
        {
            HAL_TIM_Base_Stop(&htim3);
            SR04_Count = __HAL_TIM_GetCounter(&htim3);
            SR04_Distance = SR04_GetDistance(SR04_Count);
        }
    }
    if (GPIO_Pin == GPIO_PIN_5)
    {
        Control_PID();
    }
}

