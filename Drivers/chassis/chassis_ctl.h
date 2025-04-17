#ifndef __PS_CHASSIS_CTL_H__
#define __PS_CHASSIS_CTL_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"

/**
 * @note 电机地址：电机的地址可以是1~255，底盘电机必须是一个连续地址段
 */

const static int CHASSIS_MOTOR = 4; // 电机的数量
const static int CHASSIS_ADDR_SHIFT = 1; // 电机地址开始地址

/**
 * @brief 底盘的定义
 */
typedef struct
{
	float wheel_diameter;
    float pitch_a_b_sum; // a + b
} chassis_def_t;

/**
 * @brief 底盘运动方向到数据的映射
 */
typedef enum
{
    chassis_direction_FWD = 0x0,  
    chassis_direction_BAK = 0x1,
} chassis_direction_t;

/**
 * @brief 底盘平移
 */
typedef struct
{
    uint8_t acc;
    uint8_t abs;
    uint16_t speed;
    float distance[CHASSIS_MOTOR];
} chassis_pos_ctl_t;

typedef struct
{
    uint8_t acc;
    float speed[CHASSIS_MOTOR];
} chassis_spd_ctl_t;

#ifdef __cplusplus

#include "emm42_v5.h"
extern ps::emm42::emm42_v5 *chassis_emm42_handle;

extern "C" {
#endif

/**
 * @brief 底盘上下文初始化
 */
void chassic_ctl_init(UART_HandleTypeDef *pHUART, double wheel_diameter);

/**
 * @brief 停止底盘
 */
void chassis_halt();

/**
 * @brief 核心出装：向四个电机发送位置控制信号，四个电机需要同步
 * @note 不会等待到位输出，需要的话请自行调用chassis_arrived()
 */
void chassic_pos_ctl(chassis_pos_ctl_t *ctl);

/**
 * @brief 向四个电机发送速度控制信号
 */
void chassis_speed_ctl(chassis_spd_ctl_t *ctl);

/**
 * @brief 等待电机运动结束
 */
void chassis_arrived(void);

#ifdef __cplusplus
}
#endif

#endif