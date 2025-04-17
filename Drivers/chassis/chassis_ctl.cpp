#include "chassis_ctl.h"
#include "ps_stmt.h"
#include "common.h"
#include <stdlib.h>
#include <math.h>

using namespace ps;

static chassis_def_t CH_DEFINE;
uint8_t CHASSIS_ARRIVED = 0b0; // 底盘移动到位标志，每次发送指令清零
uint8_t CHASSIS_BF0[emm42::MAX_FRAME_LENGTH];
uint8_t CHASSIS_BF1[emm42::MAX_FRAME_LENGTH];
uint8_t CHASSIS_BF2[emm42::MAX_FRAME_LENGTH];
static uint8_t *CHASSIS_BUFFER[3] = {CHASSIS_BF0, CHASSIS_BF1, CHASSIS_BF2};
emm42::emm42_v5 *chassis_emm42_handle;
motor::Stepmotor *MOTORS[CHASSIS_MOTOR];


const static int TX_DELAY_INTERVAL = 1; // 发送之间等待的间隔

void chassic_ctl_init(UART_HandleTypeDef *pHUART, double wheel_diameter)
{
    // 初始化串口
    chassis_emm42_handle = new ps::emm42::emm42_v5 {pHUART, CHASSIS_BUFFER};
    chassis_emm42_handle->start();
    // 初始化底盘控制
    const uint32_t PPR = 3200;
    // ( Pulse / Round ) / C = Pulse / x
    double ratio = (double)PPR / (wheel_diameter * PI);
    // 初始化底盘电机
    for (int x=0; x<CHASSIS_MOTOR; x++)
    {
        MOTORS[x] = new motor::Stepmotor {*chassis_emm42_handle,
                                          static_cast<uint8_t>(x+CHASSIS_ADDR_SHIFT),
                                          ratio, PPR, 1};
        MOTORS[x]->reverse = true;
        MOTORS[x]->default_acc = 180;
        MOTORS[x]->default_speed = 650;
    }
}

void chassis_halt()
{
    chassis_emm42_handle->halt();
}

void chassic_pos_ctl(chassis_pos_ctl_t *ctl)
{
    // 记录最远移动距离
    int max_index = -1;
    float max_abs_distance = 0;
    // 设置电机
    for (int index=0; index<CHASSIS_MOTOR; index++)
    {
        float distance = ctl->distance[index];
        CHASSIS_ARRIVED = 0b0;
        if (distance != 0)
        {
            MOTORS[index]->move(distance, ctl->speed, ctl->acc, true);
        } else
            CHASSIS_ARRIVED |= (0b1<<index);
    }
    // 设置同步
    chassis_emm42_handle->sync();
}

void chassis_speed_ctl(chassis_spd_ctl_t *ctl)
{
    // 设置电机
    CHASSIS_ARRIVED = 0b1111;
    for (int index=0; index<CHASSIS_MOTOR; index++)
    {
        MOTORS[index]->move_speed(ctl->speed[index], ctl->acc, true);
    }
    chassis_emm42_handle->sync();
}

void chassis_arrived(void)
{
    for (int x=0; x<CHASSIS_MOTOR; x++)
    {
        // 检测所有需要检测到位的电机
        if ((CHASSIS_ARRIVED & (0b1<<x)) == 0)
            MOTORS[x]->arrived();
    }
}