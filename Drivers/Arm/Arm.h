#ifndef __ARMTASK_H_
#define __ARMTASK_H_

#include "main.h" // Device header
#include "StepmotorGPIO.h"
#include "usart.h"
#include "common.h"
#include "emm42_v5.h"

extern uint8_t StepMotor_ToStepPacket[13];
extern uint8_t StepMotor_StopPacket[5];
extern uint8_t StepMotor_StatePacket[3];
extern int32_t Motor_TimeCNT[2];

typedef enum
{
    Red2 = 1,
    Ordinary2 = 0,
    Green2 = 2,
    Blue2 = 3,
    Dtct2 = 4,
    Limit = 5
} ArmTarget;

typedef enum
{
    Ordinary = 0,
    Open = 1,
    Close = 2
} ArmState;

typedef enum
{
    Ordinary3 = 0,
    Scan = 1,			 // 扫码
    Dtct = 2,			 // 在圆盘上识别
    Plate_To_Store = 3,	 // 从圆盘拾取放到存储机构
    Store_To_Ground = 4, // 从存储机构摆放到地面
    Ground_To_Store = 5, // 从地面拾取放到存储机构
    Maduo = 6,			 // 码垛
    Store_Open = 7,		 // 存储机构开
    Store_Close = 8		 // 存储机构关
} ArmAction;


#ifdef __cplusplus
extern "C"
{
#endif
	
void StepMotor_ToStepData(StepMotorStruct *motor,int32_t step,uint16_t speed,uint8_t flag);
void StepMotor_ToAngleData(StepMotorStruct *motor,int16_t angle,uint16_t speed,uint8_t flag);
void StepMotor_ToStepControl(StepMotorStruct *motor);
int16_t StepMotor_Time(StepMotorStruct *motor);
	
void StepMotor_Stop(StepMotorStruct *motor);	
void StepMotor_GetState(StepMotorStruct *motor);

	
uint16_t angle_to_position(float angle);
void Arm_Action(ArmAction action, ArmTarget color);

	
	
#ifdef __cplusplus
}
#endif

#endif
