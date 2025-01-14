//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 测试硬件：单片机STM32F407ZGT6,正点原子Explorer STM32F4开发板,主频168MHZ，晶振12MHZ
// QDtech-TFT液晶驱动 for STM32 IO模拟
// xiao冯@ShenZhen QDtech co.,LTD
// 公司网站:www.qdtft.com
// 淘宝网站：http://qdtech.taobao.com
// wiki技术网站：http://www.lcdwiki.com
// 我司提供技术支持，任何技术问题欢迎随时交流学习
// 固话(传真) :+86 0755-23594567
// 手机:15989313508（冯工）
// 邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
// 技术支持QQ:3002773612  3002778157
// 技术交流QQ群:324828016
// 创建日期:2018/08/09
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 深圳市全动电子技术有限公司 2018-2028
// All rights reserved
/****************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/
#include "ili9341.h"
#include <stdlib.h>
#include "stm32h7xx_hal_gpio.h"

// 管理LCD重要参数
#define LCD_CS_GPIO_TYPE GPIOC
#define LCD_CS_GPIO_PIN GPIO_PIN_4
#define LCD_RESET_GPIO_TYPE GPIOA
#define LCD_RESET_GPIO_PIN GPIO_PIN_4
#define LCD_RS_GPIO_TYPE GPIOC
#define LCD_RS_GPIO_PIN GPIO_PIN_5

#define LCD_CS(GPIO_PIN_MODE) HAL_GPIO_WritePin(LCD_CS_GPIO_TYPE, \
                                                LCD_CS_GPIO_PIN,  \
                                                GPIO_PIN_MODE)
#define LCD_RESET(GPIO_PIN_MODE) HAL_GPIO_WritePin(LCD_RESET_GPIO_TYPE, \
                                                   LCD_RESET_GPIO_PIN,  \
                                                   GPIO_PIN_MODE)
#define LCD_RS(GPIO_PIN_MODE) HAL_GPIO_WritePin(LCD_RS_GPIO_TYPE, \
                                                LCD_RS_GPIO_PIN,  \
                                                GPIO_PIN_MODE)
static SPI_HandleTypeDef *HSPI;

const static uint16_t __CMD_SELECT_X    = 0x2A;
const static uint16_t __CMD_SELECT_Y    = 0x2B;
const static uint16_t __CMD_WRITE_MEM   = 0x2C;
/**
 * @brief T_2Bytes = 2 *       8 *    2 *         1024
 *                   2边沿采样 一字节  2字节一单位 换算毫秒
 * @brief 延迟系数 = log(baud / T_2Bytes) = log(baud) - 15
 * @brief 实际延时 = uint16 >> 延迟系数
 */
const static int __INTERVAL_SCALE = 10;
const static uint16_t LCD_LONG_SIDE     = 320; // LCD屏的长边
const static uint16_t LCD_SHORT_SIDE    = 240; // LCD屏的短边
const static uint8_t DEFUALT_DIRECTION = 1; // 默认显示方向
uint16_t LCD_WIDTH; // LCD屏的宽度
uint16_t LCD_HEIGHT; // LCD屏的高度

/*****************************************************************************
 * @name       :void __lcd_select_reg(uint8_t data)
 * @date       :2018-08-09
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
 ******************************************************************************/
void __lcd_select_reg(uint8_t data)
{
    LCD_CS(GPIO_PIN_RESET);
    LCD_RS(GPIO_PIN_RESET);
    HAL_SPI_Transmit(HSPI, &data, 1, HAL_MAX_DELAY);
    LCD_CS(GPIO_PIN_SET);
}

/*****************************************************************************
 * @name       :void __lcd_write_byte(uint8_t data)
 * @date       :2018-08-09
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
 ******************************************************************************/
void __lcd_write_byte(uint8_t data)
{
    LCD_CS(GPIO_PIN_RESET);
    LCD_RS(GPIO_PIN_SET);
    HAL_SPI_Transmit(HSPI, &data, 1, HAL_MAX_DELAY);
    LCD_CS(GPIO_PIN_SET);
}

void ili9341_write_reg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
    __lcd_select_reg(LCD_Reg);
    __lcd_write_byte(LCD_RegValue);
}

void ili9341_write_GRAM(uint16_t *pixels, int len)
{
    LCD_CS(GPIO_PIN_RESET);
    LCD_RS(GPIO_PIN_SET);
    while (HAL_SPI_Transmit_DMA(HSPI, (uint8_t*)pixels, sizeof(uint16_t)*len) == HAL_BUSY);
    HAL_Delay((len >> __INTERVAL_SCALE));
    LCD_CS(GPIO_PIN_SET);
}

void ili9341_reset(void)
{
    LCD_RESET(GPIO_PIN_RESET);
    HAL_Delay(100);
    LCD_RESET(GPIO_PIN_SET);
    HAL_Delay(50);
}

void ili9341_set_direction(uint8_t direction)
{
    switch (direction)
    {
    case 0:
        LCD_WIDTH = LCD_SHORT_SIDE;
        LCD_HEIGHT = LCD_LONG_SIDE;
        ili9341_write_reg(0x36, (1 << 3) | (0 << 6) | (0 << 7)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 1:
        LCD_WIDTH = LCD_LONG_SIDE;
        LCD_HEIGHT = LCD_SHORT_SIDE;
        ili9341_write_reg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    case 2:
        LCD_WIDTH = LCD_SHORT_SIDE;
        LCD_HEIGHT = LCD_LONG_SIDE;
        ili9341_write_reg(0x36, (1 << 3) | (1 << 6) | (1 << 7)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 3:
        LCD_WIDTH = LCD_LONG_SIDE;
        LCD_HEIGHT = LCD_SHORT_SIDE;
        ili9341_write_reg(0x36, (1 << 3) | (1 << 7) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    default:
        break;
    }
}

void ili9341_init(SPI_HandleTypeDef *hspi)
{
    HSPI = hspi;
    ili9341_reset();    // LCD 复位
    //*************3.2inch ILI9341初始化**********//
    __lcd_select_reg(0xCF);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0xD9); // C1
    __lcd_write_byte(0X30);
    __lcd_select_reg(0xED);
    __lcd_write_byte(0x64);
    __lcd_write_byte(0x03);
    __lcd_write_byte(0X12);
    __lcd_write_byte(0X81);
    __lcd_select_reg(0xE8);
    __lcd_write_byte(0x85);
    __lcd_write_byte(0x10);
    __lcd_write_byte(0x7A);
    __lcd_select_reg(0xCB);
    __lcd_write_byte(0x39);
    __lcd_write_byte(0x2C);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0x34);
    __lcd_write_byte(0x02);
    __lcd_select_reg(0xF7);
    __lcd_write_byte(0x20);
    __lcd_select_reg(0xEA);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0x00);
    __lcd_select_reg(0xC0);  // Power control
    __lcd_write_byte(0x1B); // VRH[5:0]
    __lcd_select_reg(0xC1);  // Power control
    __lcd_write_byte(0x12); // SAP[2:0];BT[3:0] //0x01
    __lcd_select_reg(0xC5);  // VCM control
    __lcd_write_byte(0x26); // 3F
    __lcd_write_byte(0x26); // 3C
    __lcd_select_reg(0xC7);  // VCM control2
    __lcd_write_byte(0XB0);
    __lcd_select_reg(0x36); // Memory Access Control
    __lcd_write_byte(0x08);
    __lcd_select_reg(0x3A);
    __lcd_write_byte(0x55);
    __lcd_select_reg(0xB1);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0x1A);
    __lcd_select_reg(0xB6); // Display Function Control
    __lcd_write_byte(0x0A);
    __lcd_write_byte(0xA2);
    __lcd_select_reg(0xF2); // 3Gamma Function Disable
    __lcd_write_byte(0x00);
    __lcd_select_reg(0x26); // Gamma curve selected
    __lcd_write_byte(0x01);
    __lcd_select_reg(0xE0); // Set Gamma
    __lcd_write_byte(0x1F);
    __lcd_write_byte(0x24);
    __lcd_write_byte(0x24);
    __lcd_write_byte(0x0D);
    __lcd_write_byte(0x12);
    __lcd_write_byte(0x09);
    __lcd_write_byte(0x52);
    __lcd_write_byte(0xB7);
    __lcd_write_byte(0x3F);
    __lcd_write_byte(0x0C);
    __lcd_write_byte(0x15);
    __lcd_write_byte(0x06);
    __lcd_write_byte(0x0E);
    __lcd_write_byte(0x08);
    __lcd_write_byte(0x00);
    __lcd_select_reg(0XE1); // Set Gamma
    __lcd_write_byte(0x00);
    __lcd_write_byte(0x1B);
    __lcd_write_byte(0x1B);
    __lcd_write_byte(0x02);
    __lcd_write_byte(0x0E);
    __lcd_write_byte(0x06);
    __lcd_write_byte(0x2E);
    __lcd_write_byte(0x48);
    __lcd_write_byte(0x3F);
    __lcd_write_byte(0x03);
    __lcd_write_byte(0x0A);
    __lcd_write_byte(0x09);
    __lcd_write_byte(0x31);
    __lcd_write_byte(0x37);
    __lcd_write_byte(0x1F);

    __lcd_select_reg(0x2B);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0x01);
    __lcd_write_byte(0x3f);
    __lcd_select_reg(0x2A);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0x00);
    __lcd_write_byte(0xef);
    __lcd_select_reg(0x11); // Exit Sleep
    HAL_Delay(120);
    __lcd_select_reg(0x29); // display on

    ili9341_set_direction(DEFUALT_DIRECTION); // 设置LCD显示方向
}

void ili9341_set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t x_end = x+width-1, y_end = y+height-1;

    __lcd_select_reg(__CMD_SELECT_X);
    __lcd_write_byte(x >> 8);
    __lcd_write_byte(0x00FF & x);
    __lcd_write_byte(x_end >> 8);
    __lcd_write_byte(0x00FF & x_end);

    __lcd_select_reg(__CMD_SELECT_Y);
    __lcd_write_byte(y >> 8);
    __lcd_write_byte(0x00FF & y);
    __lcd_write_byte(y_end >> 8);
    __lcd_write_byte(0x00FF & y_end);

    __lcd_select_reg(__CMD_WRITE_MEM); // 开始写入GRAM
}


