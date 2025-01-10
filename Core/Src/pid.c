#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"

int Encoder_Left , Encoder_Right;
extern mpu_type_r mpu_r; // 欧拉角数据
mpu_type_a mpu_a; // 加速度数据
mpu_type_w mpu_w; // 角速度数据

// 定义变量，用于存储速度输出、转向输出、垂直输出、目标速度和目标转向
int Velocity_Out, Turn_Out, Vertical_Out, Target_Speed, Target_Turn;
int Motor1_PWM, Motor2_PWM;


// 声明一个浮点型变量，用于存储中值角度
float Med_Angle;
// 声明垂直方向的比例系数和微分系数
float Vertical_Kp, Vertical_Kd;
// 声明速度的比例系数和积分系数
float Velocity_Kp, Velocity_Ki;
// 声明转向的比例系数和微分系数
float Turn_Kp, Turn_Kd;

// 垂直方向PID控制函数
int Vertical_PID(float Med, float Angle, float gyroY)
{
    int temp;
    temp = Vertical_Kp * (Angle - Med) + Vertical_Kd * (gyroY);
    return temp;
}

// 速度PID控制函数
int Velocity_PID(int Traget, int Encoder_L, int Encoder_R)
{
    static int Error_LowOut_Last,Encoder_S;
    static float flag = 0.7;
    int Error,Error_LowOut,temp;
    // 计算误差
    Error = (Encoder_L + Encoder_R) - Traget;
    // 计算低通滤波后的误差
    Error_LowOut = (1 - flag) * Error + Error_LowOut_Last;
    // 更新低通滤波后的误差
    Error_LowOut_Last = Error_LowOut;
    // 计算积分项
    Encoder_S += Error_LowOut;
    // 限制积分项的范围
    if(Encoder_S > 5000) Encoder_S = 5000;
    if(Encoder_S < -5000) Encoder_S = -5000;
    temp = Velocity_Kp * Error + Velocity_Ki * Encoder_S;
    return temp;
}

int Turn_PID(int Traget, float gyroZ)
{
    int temp;
    temp = Turn_Kp * Traget + Turn_Kd * gyroZ;
    return temp;
}

void Control_PID(void)
{
    int PWM_Out;
    Encoder_Left = Read_Speed(&htim2);
    Encoder_Right = -Read_Speed(&htim4);
    mpu_dmp_get_data(&mpu_r.pitch,&mpu_r.roll,&mpu_r.yaw);
    MPU_Get_Gyroscope(&mpu_w.gyro_x,&mpu_w.gyro_y,&mpu_w.gyro_z);
    MPU_Get_Accelerometer(&mpu_a.ax,&mpu_a.ay,&mpu_a.az);

    Velocity_Out = Velocity_PID(Target_Speed, Encoder_Left, Encoder_Right);
    Vertical_Out = Vertical_PID(Velocity_Out + Med_Angle, mpu_r.roll, mpu_w.gyro_y);
    Turn_Out = Turn_PID(Target_Turn, mpu_w.gyro_z);
    PWM_Out = Vertical_Out;
    Motor1_PWM = PWM_Out - Turn_Out;
    Motor2_PWM = PWM_Out + Turn_Out;
    motor_limit_speed(&Motor1_PWM, &Motor2_PWM);
    motor_set_speed(Motor1_PWM, Motor2_PWM);
}
