#include "Timer.h"
#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "tim.h"

#include "Arm.h"
#include "Communication.h"
#include "Chassis.h"
#include "HWT101.h"
#include "vision.h"

typedef struct
{
	TIM_HandleTypeDef htim;
	uint16_t TimerCounter;
	uint16_t Second;

} TimerDef;

TimerDef Timer3;
TimerDef Timer4;


//uint8_t Servor_Packet_test[10]={0x55,0x55,0x08,0x03,0x01,0,0,0,0,0};
//uint8_t servor_Tx_flag=0;

//struct Servor Servor_test={
//    .Num=0,
//    .Time=1000,
//    .Angle={0,0,0,0,0
//    }
//};

//volatile uint8_t arm_test_flag=0;
//volatile ArmAction action_test;
//volatile ArmTarget color_test; 



//void Servor_Tx(struct Servor *servor)
//{
//	Servor_Packet_test[7]=servor->Num;
//	Servor_Packet_test[5]=(uint8_t)(servor->Time);
//	Servor_Packet_test[6]=(uint8_t)(servor->Time>>8);
//	Servor_Packet_test[8]=(uint8_t)(angle_to_position(servor->Angle[0]));
//	Servor_Packet_test[9]=(uint8_t)(angle_to_position(servor->Angle[0])>>8);
//	HAL_UART_Transmit(&huart5,Servor_Packet_test,10,1000);
//}

/***********????????????**************/

/****************************************/

/**
 * @description: ???????????
 * @return {*}
 */
void debug(void)
{
//	Chassis_To(&Chassis,0,500,2,0);
}


/**
 * @description:
 * @return {*}
 */

void Timer_Start(void)
{
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
}

void Timer3_Callback(TIM_HandleTypeDef *htim) // 0.1ms?????
{
    if (htim == &htim3)
    {
        
        if (Timer3.TimerCounter >= 60000)
		{
			Timer3.TimerCounter = 0;
		}
        
        
    }
        
}

void Timer4_Callback(TIM_HandleTypeDef *htim)		//1ms
{
	if (htim == &htim4)
	{
        vision_update_nowait();
        HWT101_update_nowait();

		Timer4.TimerCounter++;
		if (Timer4.TimerCounter >= 60000)
		{
			Timer4.TimerCounter = 0;
		}
		
//		if(servor_Tx_flag==1)
//		{
//			servor_Tx_flag=0;
//			Servor_Tx(&Servor_test);
//		}
		
//		if(arm_test_flag==1)
//		{
//			arm_test_flag=0;
//			Arm_Action(action_test,color_test);
//		}
	}
}



/*???????????????????????????????*/
void Timer2_Callback(TIM_HandleTypeDef *htim)
{
	// if(htim == &htim2)
	// {
	// 	StepMotor_Control(&Motor_R);
	// }
}
	
void Timer5_Callback(TIM_HandleTypeDef *htim)
{
	// if(htim == &htim5)
	// {
	// 	StepMotor_Control(&Motor_Z);
	// }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(htim);

	/* NOTE : This function should not be modified, when the callback is needed,
			  the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
	 */

	
	Timer2_Callback(htim);
	Timer5_Callback(htim);
	Timer3_Callback(htim);
	Timer4_Callback(htim);
}



