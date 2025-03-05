#include "Arm.h"

/**
 * @brief 机械臂基础控制
 */
arm_define_t ARM_DEFINE = {
    .center = {
        .x = -197,
        .y = 48
    },
    .claw_distance = 135,
    .deadzone = {
        .lower = -270,
        .upper = -90,
        .inner_max = -40,
        .outer_max = -140
    },
    .motor_r = {
        .ID = 1,
		.PPR = 3200,
        .ratio = 1. / 360. * 90. / 32. * 3200, // 25
        .acc = 215,
        .speed = 2100,
#		ifdef __ENABLE_SOFTWARE_ZERO
			.zero_shift = 0,
#		endif
        .lower = -360,
        .upper = 360
    },

    .motor_x = {
        .ID = 3,
		.PPR = 3200,
        .ratio = 50,
        .acc = 255,
        .speed = 2300,
#		ifdef __ENABLE_SOFTWARE_ZERO
			.zero_shift = 0,
#		endif
        .lower = 0,
        .upper = 300,
    },

    .motor_z = {
        .ID = 2,
		.PPR = 3200,
        .ratio = 80,
        .acc = 240,
        .speed = 2300,
#		ifdef __ENABLE_SOFTWARE_ZERO
			.zero_shift = 0,
#		endif
        .lower = -140,
        .upper = 0,
    }
};

arm_define_t *pARM_DEFINE = &ARM_DEFINE;

/*---------------步进电机控制部分---------------------*/
// 绝对位置控制
//  0--地址
//  1--0xFD
//  2--方向
//  3--速度高八位
//  4--速度低八位
//  5--加速度
//  6--脉冲
//  7--脉冲
//  8--脉冲
//  9--脉冲
//  10--相对/绝对--1绝对，0相对
//  11--多机同步标志
//  12--校验字节



void arm_ctl_init(UART_HandleTypeDef *pHUART)
{
    // 初始化串口
    stepmotor_init(pHUART);

    // 初始化步进电机结构体
    arm_define_t *arm = pARM_DEFINE;
    arm->deadzone.inner = true;

    // 初始化电机
    arm_position_update();
}

void arm_set_state(bool state)
{
    stepmotor_set_state(ARM_DEFINE.motor_r.ID, state);
    stepmotor_set_state(ARM_DEFINE.motor_z.ID, state);
    stepmotor_set_state(ARM_DEFINE.motor_x.ID, state);
}

void arm_position_update(void)
{
    stepmotor_position_update(&ARM_DEFINE.motor_r);
    stepmotor_position_update(&ARM_DEFINE.motor_x);
    stepmotor_position_update(&ARM_DEFINE.motor_z);
}

void arm_move_sync(void)
{
    stepmotor_arrived(&ARM_DEFINE.motor_r);
    stepmotor_arrived(&ARM_DEFINE.motor_x);
    stepmotor_arrived(&ARM_DEFINE.motor_z);
}

