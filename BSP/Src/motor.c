#include "motor.h"

#define MOTOR1_PORT GPIOB
#define MOTOR2_PORT GPIOB
#define MOTOR1_EN_PIN GPIO_PIN_13
#define MOTOR1_DIR_PIN GPIO_PIN_12
#define MOTOR2_EN_PIN GPIO_PIN_14
#define MOTOR2_DIR_PIN GPIO_PIN_15

#define PWM_MAX 100
#define PWM_MIN -100
// 定义一个函数，用于计算一个整数的绝对值
int abs(int x)
{
    // 如果x小于0，则返回-x，否则返回x
    return x < 0 ? -x : x;
}

// 初始化电机
void motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}


void motor_set_speed(int speed1, int speed2) // speed1 and speed2 are in the range [-100, 100]
{
    if (speed1 > 0)
    {
        // Set motor direction to forward
        // Set motor speed to speed
        HAL_GPIO_WritePin(MOTOR1_PORT, MOTOR1_EN_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR1_PORT, MOTOR1_DIR_PIN, GPIO_PIN_RESET);
    }
    else
    {
        // Set motor direction to reverse
        // Set motor speed to -speed
        HAL_GPIO_WritePin(MOTOR1_PORT, MOTOR1_EN_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR1_PORT, MOTOR1_DIR_PIN, GPIO_PIN_SET);
    }
    // 设置定时器1的通道4的比较值，比较值为speed1的绝对值
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, abs(speed1));
    if (speed2 > 0)
    {
        // Set motor direction to forward
        // Set motor speed to speed
        HAL_GPIO_WritePin(MOTOR2_PORT, MOTOR2_EN_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR2_PORT, MOTOR2_DIR_PIN, GPIO_PIN_RESET);
    }
    else
    {
        // Set motor direction to reverse
        // Set motor speed to -speed
        HAL_GPIO_WritePin(MOTOR2_PORT, MOTOR2_EN_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR2_PORT, MOTOR2_DIR_PIN, GPIO_PIN_SET);
    }
    // 设置定时器1的通道1的比较值，比较值为speed2的绝对值
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, abs(speed2));
}

void motor_limit_speed(int *speed1, int *speed2)
{
    if (*speed1 > PWM_MAX)
        *speed1 = PWM_MAX;
    else if (*speed1 < PWM_MIN)
        *speed1 = PWM_MIN;
    if (*speed2 > PWM_MAX)
        *speed2 = PWM_MAX;
    else if (*speed2 < PWM_MIN)
        *speed2 = PWM_MIN;
}