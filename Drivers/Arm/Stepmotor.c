#include "stepmotor.h"

void stepmotor_position_update(stepmotor_t *stepmotor)
{
    float pulse = (float)emm42_read_position(stepmotor->handle, stepmotor->ID);
    stepmotor->position = ( pulse / 360. * stepmotor->PPR ) / stepmotor->ratio;
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
    emm42_pos_ctl(motor->handle, motor->ID, dir, motor->speed,
                  motor->acc, step, 1, 0);
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
void stepmotor_arrived(stepmotor_t *motor)
{   
    emm42_arrived(motor->handle, motor->ID);
}