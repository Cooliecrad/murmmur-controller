#ifndef __PS_HWT101_H__
#define __PS_HWT101_H__

#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "coordinate.h"

#ifdef __cplusplus

#include "ps_uart_v3.h"
extern ps::UART *HWT101_uart_handle;

extern "C" {
#endif

typedef struct
{
	float YawExp; // 预期的当前偏航角
	float YawZ; // 原始的偏航角（未校正） -180~180
	float Yaw_calib; // 偏航角的用户校准值
	float WY;
	float WZ;
#	ifdef __PS_UART_INDICATOR
		uint32_t accept_sum;
#	endif

} HWT101DataStruct;

extern volatile HWT101DataStruct HWT101Data;

/**
 * @brief 初始化陀螺仪配置
 */
void HWT101_init(UART_HandleTypeDef *pHUART);

/**
 * @brief 将陀螺仪校准到指定的值
 */
void HWT101_calibrate(float dst);

/**
 * @brief 设置陀螺仪的校准值
 */
inline void HWT101_set_calibrate(float dst)
{
    HWT101Data.Yaw_calib = dst;
}

/**
 * @brief 读取校准过的偏航角
 */
float HWT101_read_yaw(void);
	
/**
 * @brief 发送指令: 归零偏航角
 */
void HWT101_yaw_reset(void);

#ifdef __cplusplus
}
#endif

#endif