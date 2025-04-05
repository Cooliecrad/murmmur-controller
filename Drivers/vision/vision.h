#ifndef __PS_VISION_H__
#define __PS_VISION_H__

#include "common.h"
#include "stm32h7xx_hal.h"
#include <stddef.h>


const static uint8_t ITEM_COUNT = 3; // 单圈搬运需要搬运的个数

#ifdef __cplusplus

#include "ps_uart_v3.h"
extern ps::UART *vision_uart_handle;

extern "C" {
#endif

/**
 * @brief 远程控制模式下收到的数据
 */
typedef struct
{
    uint8_t *data;
    int length;
} RC_data_t;

/**
 * @brief 初始化视觉通信
 */
void vision_init(UART_HandleTypeDef *pHUART);

/**
 * @brief 请求视觉进入空闲状态，不要扫描以免在运动中传输错误信息
 */
void vision_idle();
 
/**
 * @brief 到达检测位置，开始检测物品顺序
 */
void vision_subscribe_order(void);

/**
 * @brief 检测放置物品的顺序
 * @param round 圈数
 * @param index 当前圈的第几个物品
 */
color_t vision_order(uint8_t round, uint8_t index);

/**
 * @brief 向视觉订阅指定物品的信息
 * @param color 需要的颜色
 */
void vision_subscribe_item(color_t color);

/**
 * @brief 阻塞的向视觉订阅指定物品的信息，当获取到物品位置时才返回
 * @param color 需要的颜色
 */
uint8_t vision_get_item(color_t color);

/**
 * @brief 停止检测物品。由于视觉是通过时间来估计物品位置的。当物品检测结束，必须清空视觉
 * 已经保存的时间记录
 * @note 没有必要使用同步等待这个请求，因为根本不会返回值
 */
void vision_item_detect_stop();

/**
 * @brief 向视觉订阅指定位置的误差
 * @param pos 位置编号
 */
void vision_subscribe_pos(uint8_t pos);

/**
 * @brief 返回当前的位置误差
 * @note 如果未提前调用vision_subscribe_pos()，可能会导致程序卡死、数据读取错误等问题
 */
Point2f vision_get_pos(uint8_t pos);

/**
 * @brief 向视觉请求当前所有色环的信息
 */
void vision_subscribe_rings();

/**
 * @brief 获取指定颜色的色环位置，按照上次调用vision_get_rings的结果返回
 */
Point2f vision_get_ring(color_t color);

/**
 * @brief 进入远程控制模式
 */
void vision_remote_control_mode();

/**
 * @brief 接收远程控制数据
 * @return 远程控制数据，用户自行回收。如果没有收到，返回的数据结构中，data == NULL
 */
RC_data_t vision_get_RC();

/**
 * @brief 等待下位机收到指定类型的数据
 * @note 这个函数可以用来保证至少收到一帧所需数据包
 */
void vision_sync();

#ifdef __cplusplus
}
#endif

#endif