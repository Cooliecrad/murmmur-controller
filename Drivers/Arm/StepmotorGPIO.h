#ifndef __STEPMOTORGPIO_H_
#define __STEPMOTORGPIO_H_

#include <stdbool.h>
#include <stdint.h>
#include "emm42_v5.h"

typedef enum
{
	MOTOR_STOP = 0,
	MOTOR_RUN,
}MotorState;

typedef enum
{
    MOTOR_BACKWARD = 0,
	MOTOR_FORWARD = 1
} motor_direction_t;

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
    float range;
} stepmotor_instance_t;

typedef struct
{
    uint8_t ID; // 地址
    double ratio; // 减速比 移动 1°/1mm 需要的脉冲数量
    uint8_t acc; // 电机运动的加速度配置
    uint16_t speed; // 电机运动的速度配置
    float position; // 当前的位置
    // 补偿
    float zero_shift; // 零点偏移量

    // 保护用
    float lower; // 保护用：电机可用使用位置的最小值
    float upper; // 保护用：电机可用使用位置的最大至

    stepmotor_instance_t instance;
} stepmotor_t;

/**
 * @brief 读取步进数据更新内存中的步进电机位置
 */
void stepmotor_position_update(emm42_handle_t handle, int pulse_per_round,
                               stepmotor_t *stepmotor);

extern stepmotor_t Motor_R;
extern stepmotor_t Motor_Z;
extern stepmotor_t Motor_X;
extern stepmotor_t Motor_Test;

#ifdef __cplusplus
extern "C" {
#endif


//void StepMotor_SetData(stepmotor_t *motor,int32_t step,float speed,uint8_t flag);
//void StepMotor_ToStepData(stepmotor_t *motor,int32_t ToStep,float speed,uint8_t flag);
//void StepMotor_ToAngleData(stepmotor_t *motor,float ToAngle,float speed,uint8_t flag);
//void StepMotor_Updata(stepmotor_t *motor);
//void StepMotor_SlowStart(stepmotor_t* motor);
//void StepMotor_Control(stepmotor_t *motor);

/*
����ʾ��

StepMotor_ToStepData(&Motor_R,3200,500,0);
StepMotor_Updata(Motor_R);

*/

#ifdef __cplusplus
}
#endif


#endif

