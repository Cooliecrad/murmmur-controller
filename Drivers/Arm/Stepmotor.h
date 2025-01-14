#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32h7xx_hal.h"

#define Emm_V5

extern uint8_t M_Tx_Data[20];//���͵���˶����������
extern uint8_t M_Rx_Data[20];//���ܵ������״̬������  ��ʱû�õ�
extern uint8_t send_flag;//���ͱ�־λ
extern uint8_t send_num;//�����ֽ���
extern uint8_t Screwrod_send_flag;//���췢�ͱ�־λ
extern uint8_t RobotArm_SendFlag;//��е�۷��ͱ�־λ
typedef enum{
	FORWARD=0X00,//���ǰ��
	BACKWARD=0X01,//��Ժ���
}Motor_Dir;

typedef enum{
	ABSOLUTE=0X01,//����λ��
	RELATIVE=0X00,//���λ��
}Motor_Angle_Mode;

typedef enum{
	MOTOR1=0X01,
	MOTOR2=0X02,
	MOTOR3=0X03,
	MOTOR4=0X04,
	MOTORALL=0X00,//0x00�㲥ģʽ
}Motor_Address;

typedef enum{
	SYNCHRONOUS=0X01,//ͬ��ģʽ
	ASYNCHRONOUS=0X00,//�첽ģʽ
}MotorMulti_Motion_Mode;

#ifdef Emm_V5

#ifdef __cplusplus
extern "C" {
#endif


void Motor_Enable(Motor_Address Motor_Address);
void Motor_Disable(Motor_Address Motor_Address);
void Set_Speed(Motor_Address Motor_Address,Motor_Dir Dir,uint16_t Acceleration,uint16_t Target_Speed,MotorMulti_Motion_Mode Synchronous_Motion);
void Set_Position(Motor_Address Motor_Address,
				  Motor_Dir Dir,
				  uint16_t Acceleration,
				  uint16_t MAX_Speed,
				  uint32_t Pulse,
				  Motor_Angle_Mode Angle_mode,
				  MotorMulti_Motion_Mode Synchronous_Motion);
void Motor_Emer_Stop(Motor_Address Motor_Address);
void Synchronous_Motion(void);
void Clear_AngleCounter(Motor_Address Motor_Address);
void Screwrod_SetPositon(Motor_Dir Dir,
													uint16_t Acceleration,
													uint16_t MAX_Speed,
													uint32_t Pulse);
void Screwrod_Enable(void);
void Screwrod_Emer_Stop(void);
void Screwrod_Clear_AngleCounter(void);
void Screwrod_Blockage_protection_Disable(void);

void RobotArm_Motor_Enable(Motor_Address Motor_Address);
void RobotArm_Motor_Disable(Motor_Address Motor_Address);
void RobotArm_Set_Speed(Motor_Address Motor_Address,
						Motor_Dir Dir,
						uint16_t Acceleration,
						uint16_t Target_Speed,
						MotorMulti_Motion_Mode Synchronous_Motion);
void RobotArm_Set_Position(Motor_Address Motor_Address,
				  Motor_Dir Dir,
				  uint16_t Acceleration,
				  uint16_t MAX_Speed,
				  uint32_t Pulse,
				  Motor_Angle_Mode Angle_mode,
				  MotorMulti_Motion_Mode Synchronous_Motion);
void RobotArm_Emer_Stop(Motor_Address Motor_Address);
void RobotArm_Synchronous_Motion(void);
void RobotArm_Clear_AngleCounter(Motor_Address Motor_Address);
void RobotArm_Blockage_protection_Disable(Motor_Address Motor_Address);
#endif

#ifdef ZDTX42

void Motor_Enable(Motor_Address Motor_Address);

void Motor_Disable(Motor_Address Motor_Address);

void Motor_Emer_Stop(Motor_Address Motor_Address);

void Synchronous_Motion(void);

void Trapezoidal_Curve_Position_Mode(Motor_Address Motor_Address,
									 Motor_Dir Dir,
									 uint16_t Acceleration,
									 uint16_t Deceleration,
									 uint16_t MAX_Speed,
									 uint32_t Angle,
									 Motor_Angle_Mode Angle_mode,
									 MotorMulti_Motion_Mode Synchronous_Motion);

void Direct_speed_limit_position(Motor_Address Motor_Address,
								 Motor_Dir Dir,
								 uint16_t Speed,
								 uint32_t Angle,
								 Motor_Angle_Mode Angle_mode,
								 MotorMulti_Motion_Mode Synchronous_Motion);

void Set_Speed(Motor_Address Motor_Address,
			   Motor_Dir Dir,
			   uint16_t Acceleration,
			   uint16_t Target_Speed,
			   MotorMulti_Motion_Mode Synchronous_Motion);

void Clear_AngleCounter(Motor_Address Motor_Address);

void Torque_Mode_Ctrl(Motor_Address Motor_Address,
					  Motor_Dir Dir,
					  uint16_t Torque_Acc,
					  uint16_t Torque_target,
					  MotorMulti_Motion_Mode Synchronous_Motion);

#endif

#ifdef __cplusplus
}
#endif

#endif
