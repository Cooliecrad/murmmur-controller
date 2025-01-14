/**
 * @brief ili9341驱动。基于lcd.h & lcd.c修改，对应源文件已经附在本目录下
 * @author Cooliecrad & Ptil0psis
 */
#ifndef __ILI9341_LCD_H__
#define __ILI9341_LCD_H__

#include <stdlib.h>
#include <stdint.h>
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化LCD屏幕。
 * @note 用户需要自行完成所使用GPIO、SPI、DMA的初始化
 */
void ili9341_init(SPI_HandleTypeDef *hspi);

void ili9341_reset(void);

/**
 * @brief 选择写入GRAM的区域
 * @param x 写入开始的x坐标
 * @param x 写入的宽度
 * @param x 写入开始的y坐标
 * @param x 写入的高度
 */
void ili9341_set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief 向指定的寄存器写入16位数据
 */
void ili9341_write_reg(uint8_t LCD_Reg, uint16_t LCD_RegValue);

/**
 * @brief 阻塞的往GRAM区域写入数据
 * @param pixels 要写入的数据
 * @param len 写入长度
 * @note 写入前必须选择要写入的区域
 */
void ili9341_write_GRAM(uint16_t *pixels, int len);

/**
 * @brief 旋转显示方向，0：不旋转；1：旋转90度；2：旋转180度；3：旋转270度
 */
void ili9341_set_direction(uint8_t direction);


#ifdef __cplusplus
}
#endif

#endif

	 



