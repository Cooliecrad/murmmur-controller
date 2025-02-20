#ifndef __PS_EMM42_V5_H__
#define __PS_EMM42_V5_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "ps_uart_v2.h"

#define __EMM42_V5_INDICATOR

/**
 * @brief 控制是否需要检查电机的收到响应（Receive）
 * @note 根据测试，收到响应实际上还是相当稳定的，所以也可以不检查，只检查到位响应
 */
#define __EMM42_V5_CHECK_RESPONSE

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    /**
     * @brief 最后一次运动到位的标志量
     * @note 只有调用emm42_pos_ctl才会修改这个标志量
     */
    volatile bool reached;

#   ifdef __EMM42_V5_CHECK_RESPONSE
        bool received; // 收到数据
#   endif

#   ifdef __EMM42_V5_INDICATOR
        uint32_t accept_sum;
        uint32_t error_sum;
#   endif
} emm42_v5_context_instance_t;

/**
 * @brief 基于上下文的舵机控制
 */
typedef struct
{
    ps_uart_handle_t ps_uart_handle;
    emm42_v5_context_instance_t instance;
} emm42_v5_t;

typedef emm42_v5_t *emm42_handle_t;

void emm42_v5_init(emm42_handle_t handle);

/**
 * @brief 设置电机状态
 * @param addr 广播地址0
 * @param enable 使能1，失能0
 * @param sync 同步1，不同步0
 */
HAL_StatusTypeDef emm42_set_state(emm42_handle_t handle, uint8_t addr,
                                  uint8_t enable, uint8_t sync);

/**
 * @brief 电机状态标志位
 */
typedef enum
{
    emm42_v5_state_DISABLE = 0x00, // 失能
    emm42_v5_state_ENABLE = 0x01, // 使能
    emm42_v5_state_REACHED = 0x02, // 到达
    emm42_v5_state_CLOG = 0x04, // 堵转
    emm42_v5_state_CLOG_PROTECT = 0x08, // 堵转保护
} emm42_v5_state_t;

/**
 * @brief 读取电机状态
 * @param addr 要读取的地址
 * @return 电机状态，标志位
 */
emm42_v5_state_t emm42_read_state(emm42_handle_t handle, uint8_t addr);

/**
 * @brief 设置速度控制模式
 */
HAL_StatusTypeDef emm42_speed_ctl(emm42_handle_t handle, uint8_t addr,
                                  uint8_t dir, uint16_t RPM,
                                  uint8_t ACC,  uint8_t sync);

/**
 * @brief 设置位置控制模式
 */
HAL_StatusTypeDef emm42_pos_ctl(emm42_handle_t handle, uint8_t addr, uint8_t dir,
                                uint16_t RPM, uint8_t ACC,  uint32_t pulse,
                                uint8_t relate_abs, uint8_t sync);

/**
 * @brief 急停
 */
HAL_StatusTypeDef emm42_halt(emm42_handle_t handle, uint8_t addr, uint8_t sync);

/**
 * @brief 多机同步
 */
HAL_StatusTypeDef emm42_sync(emm42_handle_t handle);

/**
 * @brief 归零
 */
HAL_StatusTypeDef emm42_reset(emm42_handle_t handle);

/**
 * @brief 等待最后一次运动到达（标志量：handle->reached）
 * @note 这个函数只有用户需要到位等待的时候才需要调用，是可选功能。有时候，
 * 不是必须等待底盘控制指令结束，如果平移的话，完全可以尝试叠加的效果
 */
void emm42_arrived(emm42_handle_t handle, uint8_t addr);

/**
 * @paragraph 广播设置参数
 * @note 关于广播：
 * @note 1. 如果没有1号地址的电机，将无法采用广播设置
 * @note 2. 如果采用广播地址，所有的电机将会和1号地址的电机参数相同
 */

/**
 * @brief 设置响应模式
 * @param addr 要使用广播地址，请阅读"广播设置参数""
 * @note 这个指令相当耗时间，应该和后面一个指令间隔几十毫秒的延时
 */
void emm42_set_response(emm42_handle_t handle, uint8_t addr, bool reached);

/**
 * @brief 设置到位窗口大小
 * @param addr 要使用广播地址，请阅读"广播设置参数""
 * @param deg 单位 0.1°
 */
void emm42_set_reach_wnd(emm42_handle_t handle, uint8_t addr, uint16_t deg);

#ifdef __cplusplus
}
#endif

#endif