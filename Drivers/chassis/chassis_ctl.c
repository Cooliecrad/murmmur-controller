#include "chassis_ctl.h"
#include "emm42_v5.h"
#include "emm42_v5_protocol.h"
#include "common.h"
#include <stdlib.h>
#include <math.h>

static chassis_def_t CH_DEFINE;
static emm42_v5_t CHASSIS_UART;
static ps_uart_t CHASSIS_PS_UART;
static uint8_t LAST_REACHED = 0; // 最后到达的电机地址
uint8_t CHASSIS_RECV_BUFFER0[4];
uint8_t CHASSIS_RECV_BUFFER1[4];
uint8_t CHASSIS_RECV_BUFFER2[4];

emm42_handle_t chassis_uart_handle;

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
    chassis_uart_handle = &CHASSIS_UART;

    // 初始化串口
    CHASSIS_PS_UART.buffer[0] = CHASSIS_RECV_BUFFER0;
    CHASSIS_PS_UART.buffer[1] = CHASSIS_RECV_BUFFER1;
    CHASSIS_PS_UART.buffer[2] = CHASSIS_RECV_BUFFER2;
    CHASSIS_PS_UART.recv_len = sizeof(CHASSIS_RECV_BUFFER0);
    CHASSIS_PS_UART.pHUART = pHUART;
    CHASSIS_PS_UART.recv_callback = NULL;
    CHASSIS_UART.ps_uart_handle = &CHASSIS_PS_UART;
    // 初始化底盘控制
    CH_DEFINE = define;
    emm42_v5_init(chassis_uart_handle);
    // 初始化底盘电机
    emm42_set_state(chassis_uart_handle, 0, 1, 0);
    emm42_reset(chassis_uart_handle);
    emm42_halt(chassis_uart_handle, 0, 0);
    emm42_response(chassis_uart_handle, 0, false);
}

void chassic_pos_ctl(chassis_pos_ctl_t *ctl)
{
    // 将距离转化成脉冲和方向
    static uint32_t pulse[CHASSIS_MOTOR];
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
            emm42_pos_ctl(chassis_uart_handle, index+CHASSIS_ADDR_SHIFT,
                        dir[index], ctl->speed,
                        ctl->acc, pulse[index], ctl->abs, 1);
        }
    }
    // 设置同步
    emm42_sync(chassis_uart_handle);
}

void chassis_speed_ctl(chassis_spd_ctl_t *ctl)
{
    // 设置电机
    for (int index=0; index<CHASSIS_MOTOR; index++)
    {
        uint8_t dir = ctl->speed[index] >= 0 ? chassis_direction_FWD
                                             : chassis_direction_BAK;
        uint16_t spd = velocity_to_rpm(&CH_DEFINE, ctl->speed[index]);
        emm42_speed_ctl(chassis_uart_handle, index+CHASSIS_ADDR_SHIFT,
                        dir, spd, ctl->acc, 0);
    }
}

void chassis_arrived(void)
{
    if (LAST_REACHED != 0)
        emm42_arrived(chassis_uart_handle, LAST_REACHED);
}