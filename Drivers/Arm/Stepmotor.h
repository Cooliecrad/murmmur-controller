#ifndef __MOTOR_H_
#define __MOTOR_H_

#include <stdbool.h>
#include <stdint.h>
#include "emm42_v5.h"
#include "stm32h7xx_hal.h"

/**
 * @brief 机械臂步进电机软件零点
 */
// #define __ENABLE_SOFTWARE_ZERO

typedef struct
{
	// 硬件配置
	emm42_handle_t handle; // 通信句柄
    uint8_t ID; // 地址
	const uint32_t PPR; // Pulse per round, 每圈脉冲数
    double ratio; // 减速比 移动 1°/1mm 需要的脉冲数量
	
	// 运动相关
    uint8_t acc; // 电机运动的加速度配置
    uint16_t speed; // 电机运动的速度配置
    float position; // 当前的位置

#	ifdef __ENABLE_SOFTWARE_ZERO
		// 补偿
		float zero_shift; // 零点偏移量
#	endif

    // 保护用
    float lower; // 保护用：电机可用使用位置的最小值
    float upper; // 保护用：电机可用使用位置的最大至
} stepmotor_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 电机绝对移动
 */
void stepmotor_move(stepmotor_t *motor, float position);

/**
 * @brief 电机绝对旋转
 */
void stepmotor_rotate(stepmotor_t *motor, float position);

/**
 * @brief 读取步进数据更新内存中的步进电机位置
 */
void stepmotor_position_update(stepmotor_t *stepmotor);

/**
 * @brief 等待步进电机到达位置
 */
void stepmotor_arrived(stepmotor_t *stepmotor);

#ifdef __cplusplus
}
#endif

#endif
