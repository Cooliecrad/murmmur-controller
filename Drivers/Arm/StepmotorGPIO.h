
#ifndef __STEPMOTORGPIO_H_
#define __STEPMOTORGPIO_H_


#include "main.h"

extern TIM_HandleTypeDef htim2;

typedef enum
{
	MOTOR_STOP = 0,
	MOTOR_RUN,
}MotorState;

typedef enum
{
    MOTOR_BACKWARD = 0,
	MOTOR_FORWARD = 1
}MotorDirection;

typedef enum
{
    SUBDIVISION_1 = 1,
    SUBDIVISION_2 = 2,
    SUBDIVISION_4 = 4,
    SUBDIVISION_8 = 8,
    SUBDIVISION_16 = 16,
    SUBDIVISION_32 = 32
} MotorSubdivision;

typedef struct
{
    uint8_t ID;//地址
    double Ratio;//减速比
    
    //电机控制方式标志位
    uint8_t Step_Flag;
    uint8_t ToStep_Flag;
    uint8_t ToAngle_Flag;
    uint8_t ToDistance_Flag;
    
    // 步数控制
    volatile int32_t Steps_Cur;    // 当前步数
    volatile int32_t Steps_Tar;    //目标步数
    int32_t Steps_ToDir; //目标位置
    int32_t Steps_Sum;    //总步数
    double Angle;    //总角度
    double Distance;    //前进距离（一周为32mm）
    
    // 电机状态、标志位
    volatile uint8_t Step_Finish_Flag;      // 步进完成标志位
    volatile uint8_t Step_Running_Flag;     //正在步进标志位

    //串口控制
    UART_HandleTypeDef *huart;
    // 定时器控制
    TIM_HandleTypeDef *htim;        // 定时器句柄
    uint16_t Tim_Channel;           // 定时器通道
    GPIO_TypeDef *GPIO_Port_EN;
    uint16_t GPIO_PIN_EN;
    GPIO_TypeDef *GPIO_Port_OUT;
    uint16_t GPIO_PIN_OUT;
    GPIO_TypeDef *GPIO_Port_DIR;
    uint16_t GPIO_PIN_DIR;

    // 速度控制
    uint8_t Pulse_CNT;              //步进正反计数
    MotorSubdivision Motor_Subdivision;  // 细分
    uint16_t Freq;                  // 当前频率
    uint16_t Freq_Tar;              // 目标频率
    float Speed_Cur;
    float Speed_Tar;
    float Speed_Max;             // 最大速度
    float Speed_Min;             // 最小速度
    float acceleration;
    float deceleration;
    
    
    volatile uint8_t OneStep_Finish_Flag;   // 单步步进完成标志位
    volatile uint8_t Slow_Flag;      //启用缓启动标志位
    volatile uint8_t Slow_Finish_Flag;      //缓启动完成标志位
    volatile uint16_t Slow_CNT;            //缓启动计数

    MotorState Motor_State;        // 电机状态
    MotorDirection Motor_Direction;  // 电机方向
    
} StepMotorStruct;

extern StepMotorStruct Motor_R;
extern StepMotorStruct Motor_Z;
extern StepMotorStruct Motor_X;
extern StepMotorStruct Motor_Test;

#ifdef __cplusplus
extern "C" {
#endif


//void StepMotor_SetData(StepMotorStruct *motor,int32_t step,float speed,uint8_t flag);
//void StepMotor_ToStepData(StepMotorStruct *motor,int32_t ToStep,float speed,uint8_t flag);
//void StepMotor_ToAngleData(StepMotorStruct *motor,float ToAngle,float speed,uint8_t flag);
//void StepMotor_Updata(StepMotorStruct *motor);
//void StepMotor_SlowStart(StepMotorStruct* motor);
//void StepMotor_Control(StepMotorStruct *motor);

/*
����ʾ��

StepMotor_ToStepData(&Motor_R,3200,500,0);
StepMotor_Updata(Motor_R);

*/

#ifdef __cplusplus
}
#endif


#endif

