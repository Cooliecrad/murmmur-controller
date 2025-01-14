#ifndef __SERVOR_H__
#define __SERVOR_H__
/**
 * @brief 基本舵机控制
 */

#include <stdint.h>
#include "stm32h7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化舵机控制
 */
void servor_init(UART_HandleTypeDef *pHUART);

/**
 * @brief 控制舵机旋转到一定角度
 */
HAL_StatusTypeDef servor_rotate(uint16_t duration, uint8_t addr, float angle);

/**
 * @brief 控制两个舵机旋转一定角度
 */
HAL_StatusTypeDef servor_rotate_2(uint16_t duration, uint8_t addr1, float angle1,
                                  uint8_t addr2, float angle2);

#ifdef __cplusplus
}
#endif

#endif