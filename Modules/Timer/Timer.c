#include "Timer.h"
#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "tim.h"

#include "Arm.h"
#include "Chassis.h"
#include "vision.h"

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
    }
}

void Timer4_Callback(TIM_HandleTypeDef *htim)		//1ms
{
	if (htim == &htim4)
	{
	}
}



void Timer2_Callback(TIM_HandleTypeDef *htim)
{
}
	
void Timer5_Callback(TIM_HandleTypeDef *htim)
{
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



