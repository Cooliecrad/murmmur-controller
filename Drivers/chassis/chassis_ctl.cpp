#include "chassis_ctl.h"
#include "emm42_v5_protocol.h"
#include "emm42_v5.h"
#include "common.h"
#include <stdlib.h>
#include <math.h>

static chassis_def_t CH_DEFINE;
static uint8_t LAST_REACHED = 0; // 最后到达的电机地址
static uint8_t CHASSIS_BF0[4];
static uint8_t CHASSIS_BF1[4];
static uint8_t CHASSIS_BF2[4];
static uint8_t *CHASSIS_BUFFER[3] = {CHASSIS_BF0, CHASSIS_BF1, CHASSIS_BF2};

ps::emm42::emm42_v5 *chassis_emm42_handle;

const static int TX_DELAY_INTERVAL = 1; // 发送之间等待的间隔

/**
 * @brief 将轮子的运动距离转化为脉冲
 */
static inline uint32_t distance_to_pulse(chassis_def_t *cd, float dst)
{
    return (uint32_t)fabs(3200*dst/PI/cd->wheel_diameter);
}

/**
 * @brief 将轮子运动的线速度(m/s)转化为电机的转速(rpm)
 * @note 取的是绝对值
 */
static inline uint16_t velocity_to_rpm(chassis_def_t *cd, float v)
{
    return (uint16_t)fabs((60 * v) / (PI*cd->wheel_diameter));
}


void chassic_ctl_init(UART_HandleTypeDef *pHUART, chassis_def_t define)
{
    // 初始化串口
    chassis_emm42_handle = new ps::emm42::emm42_v5 {pHUART, CHASSIS_BUFFER};
    // 初始化底盘控制
    CH_DEFINE = define;
    // 初始化底盘电机
    chassis_emm42_handle->set_state(0, true);
    chassis_emm42_handle->reset();
    chassis_emm42_handle->halt();
    chassis_emm42_handle->set_response(0, false);
    chassis_emm42_handle->set_reach_wnd(0, 1);
}

void chassis_halt()
{
    chassis_emm42_handle->halt();
}

void chassic_pos_ctl(chassis_pos_ctl_t *ctl)
{
    // 将距离转化成脉冲和方向
    static uint32_t pulse[CHASSIS_MOTOR];
    static uint16_t speed[CHASSIS_MOTOR];
    static uint8_t dir[CHASSIS_MOTOR];
    
    int moved = -1; // 地址最小的且移动的电机
    for (int x=0; x<CHASSIS_MOTOR; x++)
    {
        pulse[x] = distance_to_pulse(&CH_DEFINE, ctl->distance[x]);
        dir[x] = ctl->distance[x] >= 0 ? chassis_direction_FWD
                                       : chassis_direction_BAK;
        if (moved == -1 && pulse[x] != 0) moved = x+CHASSIS_ADDR_SHIFT;
    }
    if (moved == -1) return; // 如果电机都不动，直接返回即可

    LAST_REACHED = moved;

    // 设置电机
    for (int index=0; index<CHASSIS_MOTOR; index++)
    {
        if (pulse[index])
        {
            chassis_emm42_handle->pos_ctl(index + CHASSIS_ADDR_SHIFT, dir[index],
                                     ctl->speed, ctl->acc, pulse[index],
                                     ctl->abs, true);
        }
    }
    // 设置同步
    chassis_emm42_handle->sync();
}

void chassis_speed_ctl(chassis_spd_ctl_t *ctl)
{
    // 设置电机
    for (int index=0; index<CHASSIS_MOTOR; index++)
    {
        uint8_t dir = ctl->speed[index] >= 0 ? chassis_direction_FWD
                                             : chassis_direction_BAK;
        uint16_t spd = velocity_to_rpm(&CH_DEFINE, ctl->speed[index]);
        chassis_emm42_handle->speed_ctl(index + CHASSIS_ADDR_SHIFT, dir, spd, ctl->acc);
    }
}

void chassis_arrived(void)
{
    if (LAST_REACHED != 0)
        chassis_emm42_handle->arrived(LAST_REACHED);
}