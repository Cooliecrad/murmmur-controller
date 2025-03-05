#include "stepmotor.h"
#include "emm42_v5.h"

// 供通信使用
static uint8_t STEPMOTOR_BF0[40];
static uint8_t STEPMOTOR_BF1[40];
static uint8_t STEPMOTOR_BF2[40];
static uint8_t *STEPMOTOR_BUFFER[3] = {STEPMOTOR_BF0, STEPMOTOR_BF1, STEPMOTOR_BF2};

ps::emm42::emm42_v5 *stepmotor_emm42_handle;

void stepmotor_init(UART_HandleTypeDef *pHUART)
{
    stepmotor_emm42_handle = new ps::emm42::emm42_v5 {pHUART, STEPMOTOR_BUFFER};
    stepmotor_emm42_handle->set_state(0, true);
    stepmotor_emm42_handle->set_reach_wnd(0, 1);
}

void stepmotor_set_state(uint8_t addr, bool enable)
{
    stepmotor_emm42_handle->set_state(addr, enable);
}

/**
 * @brief 驱动特定电机移动到指定位置
 */
static inline void __stepmotor_move(stepmotor_t *motor, float position)
{
    // 计算实际移动距离
    float distance = position - motor->position;
    motor->position = position;
    // 发送到驱动
    uint8_t dir = position >= 0 ? 1 : 0;
    uint32_t step = fabs(position) * motor->ratio;
    stepmotor_arrived(motor); // 确保到位
    stepmotor_emm42_handle->pos_ctl(motor->ID, dir, motor->speed, motor->acc, step, 1, 0);
}

void stepmotor_move(stepmotor_t *motor, float position)
{
    // 限制移动范围
    if (position < motor->lower)
    {
        position = motor->lower;
    }
    else if (position > motor->upper)
    {
        position = motor->upper;
    }

    // 移动补偿：由于步进电机零点会偏移，所以有一个抽象零点
    // 实现这个抽象零点依靠的是零点偏移补偿
#	ifdef __ENABLE_SOFTWARE_ZERO
		position -= motor->zero_shift;
#	endif

    __stepmotor_move(motor, position);
}

void stepmotor_rotate(stepmotor_t *motor, float position)
{
    // 取值 (0, upper-lower)
    position = fmodf(position, 360.);
    // 越界要退一圈
    if (position > motor->upper)
        position -= 360;
    else if (position < motor->lower)
        position += 360;

    // 移动补偿：由于步进电机零点会偏移，所以有一个抽象零点
    // 实现这个抽象零点依靠的是零点偏移补偿
#	ifdef __ENABLE_SOFTWARE_ZERO
		position -= motor->zero_shift;
#	endif

    __stepmotor_move(motor, position);
}

void stepmotor_position_update(stepmotor_t *stepmotor)
{
    float pulse = stepmotor_emm42_handle->read_position(stepmotor->ID);
    stepmotor->position = ( pulse / 360. * stepmotor->PPR ) / stepmotor->ratio;
}

void stepmotor_arrived(stepmotor_t *motor)
{   
    stepmotor_emm42_handle->arrived(motor->ID);
}