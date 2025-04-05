#include "chassis_ctl.h"
#include "ps_stmt.h"
#include "common.h"
#include <stdlib.h>
#include <math.h>

using namespace ps;

static chassis_def_t CH_DEFINE;
static uint8_t LAST_REACHED = 0; // 最后到达的电机地址
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
        if (distance != 0)
        {
            if (max_abs_distance < fabs(distance))
            {
                max_index = index;
                max_abs_distance = fabs(distance);
            }
            MOTORS[index]->move(distance, true);
        }
    }
    // 记录最远移动
    if (max_index != -1) LAST_REACHED = max_index + CHASSIS_ADDR_SHIFT;
    // 设置同步
    chassis_emm42_handle->sync();
}

void chassis_speed_ctl(chassis_spd_ctl_t *ctl)
{
    // 设置电机
    for (int index=0; index<CHASSIS_MOTOR; index++)
    {
        MOTORS[index]->move_speed(ctl->speed[index], ctl->acc, true);
    }
    chassis_emm42_handle->sync();
}

void chassis_arrived(void)
{
    if (LAST_REACHED != 0)
        chassis_emm42_handle->arrived(LAST_REACHED);
    LAST_REACHED = 0;
}