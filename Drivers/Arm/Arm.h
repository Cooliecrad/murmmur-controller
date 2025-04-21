#ifndef __ARMTASK_H_
#define __ARMTASK_H_

#include "common.h"
#include "stm32h7xx_hal.h"
#include <stdbool.h>

/**
 * @brief 采用全功能机械臂解算函数来控制机械臂位置
 */

// #define __USING_FULL_FUNCTION_ARM_MOVE

#ifdef __cplusplus

#include "ps_stmt.h"
extern ps::emm42::emm42_v5 *arm_emm42_handle;
extern ps::motor::Stepmotor *motor_r;
extern ps::motor::Stepmotor *motor_x;
extern ps::motor::Stepmotor *motor_z;
extern ps::motor::Stepmotor *motor_xzr[3];
extern float CLAW_DISTANCE;

extern "C"
{
#endif

/**
 * @brief 初始化机械臂控制
 */
void arm_ctl_init(UART_HandleTypeDef *pHUART);

/**
 * @brief 重置机械臂零点
 */
void arm_zero_reset();

/**
 * @brief 设置机械臂使能状态
 */
void arm_set_state(int state);

/**
 * @brief 更新机械臂位置
 */
void arm_position_update(void);

/**
 * @brief 等待机械臂的所有电机运动完毕
 */
void arm_move_sync(void);


// /**
//  * @brief 用户将机械臂停在蓝色物块储存区储存的状态后调用本校准函数
//  */
// void arm_calibrate();

#ifdef __cplusplus
}
#endif

#endif
