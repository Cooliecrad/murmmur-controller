#ifndef __ARMTASK_H_
#define __ARMTASK_H_

#include "stepmotor.h"
#include "usart.h"
#include "common.h"


/**
 * @brief 采用全功能机械臂解算函数来控制机械臂位置
 */
// #define __USING_FULL_FUNCTION_ARM_MOVE

typedef struct
{
    bool inner; // 标志着当前在死区内
    float lower; // 死区下限
    float upper; // 死区上限
    float inner_max; // 死区内最大移动量
    float outer_max; // 死区外最大移动量
} arm_deadzone_t;

typedef struct
{
    Point2f center; // 旋转中心（Z轴）在视觉坐标系中的位置
    float claw_distance; // 爪子零点到旋转中心的距离
    arm_deadzone_t deadzone;
    stepmotor_t motor_r; // 旋转电机
    stepmotor_t motor_x; // 前伸电机
    stepmotor_t motor_z; // 上升电机
} arm_define_t;

typedef arm_define_t *arm_handle_t;

extern arm_handle_t pARM_DEFINE;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 初始化机械臂控制
 */
void arm_ctl_init(UART_HandleTypeDef *pHUART);

/**
 * @brief 设置机械臂使能状态
 */
void arm_set_state(bool state);

/**
 * @brief 更新机械臂位置
 */
void arm_position_update(void);

/**
 * @brief 等待机械臂的所有电机运动完毕
 */
void arm_move_sync(void);

#ifdef __cplusplus
}
#endif

#endif
