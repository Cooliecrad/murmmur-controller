#include "StepmotorGPIO.h"

void stepmotor_position_update(emm42_handle_t handle, int pulse_per_round,
                               stepmotor_t *stepmotor)
{
    float pulse = (float)emm42_read_position(handle, stepmotor->ID) / 360. * pulse_per_round;
    stepmotor->position = pulse / stepmotor->ratio;
}

////���ò�����������
//void StepMotor_SetData(stepmotor_t *motor,int32_t step,float speed,uint8_t flag)
//{
//	if(motor->Step_Running_Flag ==1 )
//		return;
//	//���±�־λ
//	motor->Step_Flag=1;
//	motor->ToStep_Flag=0;
//	motor->ToAngle_Flag=0;
//	//���²���
//	motor->Steps_Tar=step;
//	motor->Slow_Flag=flag;
//	motor->Speed_Tar=speed;
//	if(motor->Slow_Flag==0)
//		motor->Speed_Cur=motor->Speed_Tar;
//	else
//		motor->Speed_Cur=motor->Speed_Min;
//}

////���ò��������׼���������
//void StepMotor_ToStepData(stepmotor_t *motor,int32_t ToStep,float speed,uint8_t flag)
//{
//	if(motor->Step_Running_Flag ==1 )
//		return;
//	//���±�־λ
//	motor->Step_Flag=0;
//	motor->ToStep_Flag=1;
//	motor->ToAngle_Flag=0;
//	//���²���
//	motor->Steps_Tar = ToStep - motor->Steps_Sum ;
//	motor->Steps_ToDir=ToStep;
//	motor->Speed_Tar=speed;
//	if(motor->Slow_Flag==0)
//		motor->Speed_Cur=motor->Speed_Tar;
//	else
//		motor->Speed_Cur=motor->Speed_Min;
//	motor->Slow_Flag=flag;
//	motor->Step_Finish_Flag=0;
//}

////���ò����������ٶȡ�����
//void StepMotor_ToAngleData(stepmotor_t *motor,float ToAngle,float speed,uint8_t flag)
//{
//	if(motor->Step_Running_Flag ==1 )
//		return;
//	//���±�־λ
//	motor->Step_Flag=0;
//	motor->ToStep_Flag=1;
//	motor->ToAngle_Flag=0;
//	//���²���
//	int16_t toangle = (ToAngle/360.0)*3200;
//	StepMotor_ToStepData(motor,toangle,speed,flag);
//}

////���¶�ʱ����ʵ�ֵ���
//void StepMotor_Updata(stepmotor_t *motor)
//{
//	HAL_TIM_Base_Stop_IT(motor->htim);
//	uint16_t new_prescaler = (240000000 / (motor->Speed_Cur*1000)) - 1;
//	__HAL_TIM_SET_PRESCALER(motor->htim,new_prescaler);//prescaler
//	__HAL_TIM_SET_AUTORELOAD(motor->htim,499);//arr
//	motor->Step_Finish_Flag = 0;
//	motor->Step_Running_Flag = 1;
//	HAL_TIM_Base_Start_IT(motor->htim);
//}

////����������ǰ500��
//void StepMotor_SlowStart(stepmotor_t* motor)
//{
//	float delta_v = motor->acceleration * (1.0f/motor->Speed_Cur);
//	motor->Speed_Cur +=delta_v;
//	if(motor->Speed_Cur < motor->Speed_Tar)
//	{
//		motor->Slow_Finish_Flag=0;
//		motor->Speed_Cur +=delta_v;
//	}
//	else
//	{
//		motor->Speed_Cur=motor->Speed_Tar;
//		motor->Slow_Finish_Flag=1;
//	}
//	StepMotor_Updata(motor);
//}

////��ͣ
//void StepMotor_SlowStop(stepmotor_t* motor)
//{
//	float delta_v = motor->acceleration * (1.0f/motor->Speed_Cur);
//	
//	if(motor->Speed_Cur > motor->Speed_Min)
//	{
//		motor->Speed_Cur +=delta_v;
//	}
//	else
//		motor->Speed_Cur=motor->Speed_Min;
//	StepMotor_Updata(motor);
//}

////ֱ�ӷŵ��жϻص�������stepmotor_t
//void StepMotor_Control(StepMotorStruct *motor)
//{
//	if(motor->ToStep_Flag==1 && motor->Step_Running_Flag ==0 &&motor->Step_Finish_Flag==0 && motor->Step_Running_Flag ==0)
//	{
//		motor->Step_Flag=0;
//		motor->ToAngle_Flag=0;
//		motor->Steps_Tar=motor->Steps_ToDir - motor->Steps_Sum;
//	}
//	
//	int16_t steps_remain = abs(motor->Steps_Tar)-abs(motor->Steps_Cur);
//	if(steps_remain>(motor->Speed_Cur * motor->Speed_Cur)/(2*motor->deceleration))
//	{
//		if(motor->Speed_Cur < motor->Speed_Tar)
//		{
//			StepMotor_SlowStart(motor);
//		}
//	}
//	else
//	{
//		if(motor->Speed_Cur > motor->Speed_Min)
//		{
//			StepMotor_SlowStop(motor);
//		}
//	}
//	
//	if(motor->Step_Running_Flag == 0 && motor->Step_Finish_Flag == 0)
//	{
//		StepMotor_Updata(motor);
//	}
//	
//	if(motor->Step_Running_Flag ==1 )
//	{
//		if(motor->Steps_Tar > 0)
//		{
//			motor->Motor_Direction = MOTOR_FORWARD;
//			HAL_GPIO_WritePin(motor->GPIO_Port_DIR,motor->GPIO_PIN_DIR,1);
//		}
//		else
//		{
//			motor->Motor_Direction = MOTOR_BACKWARD;
//			HAL_GPIO_WritePin(motor->GPIO_Port_DIR,motor->GPIO_PIN_DIR,0);
//		}

//		if(motor->Pulse_CNT % 2 == 0)
//		{
//			HAL_GPIO_WritePin(motor->GPIO_Port_OUT,motor->GPIO_PIN_OUT,GPIO_PIN_SET);
//		}
//		else
//		{
//			HAL_GPIO_WritePin(motor->GPIO_Port_OUT,motor->GPIO_PIN_OUT,GPIO_PIN_RESET);
//		}
//		motor->Pulse_CNT ++ ;
//		motor->Step_Finish_Flag = 0;
//		if(motor->Pulse_CNT>=2)
//		{
//			motor->Pulse_CNT = 0;
//			if(motor->Steps_Tar>0) {
//				motor->Steps_Cur ++;
//				motor->Steps_Sum ++;
//			}
//			else if(motor->Steps_Tar < 0) {
//				motor->Steps_Cur --;
//				motor->Steps_Sum --;
//			}
//			if(abs(motor->Steps_Cur) >= abs(motor->Steps_Tar))
//			{
//				motor->Pulse_CNT=0;
//				motor->Slow_CNT=0;
//				motor->Steps_Cur=0;
//				motor->Steps_Tar=0;
//				motor->Steps_ToDir=0;
//				motor->Step_Finish_Flag = 1;
//				motor->Step_Running_Flag = 0;
//				if(motor->Slow_Flag==1)
//				{
//					motor->Speed_Cur=motor->Speed_Min;
//				}
//				HAL_GPIO_WritePin(motor->GPIO_Port_OUT, motor->GPIO_PIN_OUT, GPIO_PIN_RESET);
//			}
//		}
//	}
//	 __HAL_TIM_CLEAR_FLAG(motor->htim,TIM_FLAG_UPDATE);
//}

