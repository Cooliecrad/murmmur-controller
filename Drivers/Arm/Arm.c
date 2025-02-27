#include "Arm.h"

/**
 * @brief 机械臂基础控制
 */
ps_uart_t ARM_PS_UART;
emm42_v5_t ARM_EMM42;

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
		.handle = &ARM_EMM42,
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
		.handle = &ARM_EMM42,
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
		.handle = &ARM_EMM42,
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

// 供通信使用
uint8_t ARM_RECV_BUFFER0[4];
uint8_t ARM_RECV_BUFFER1[4];
uint8_t ARM_RECV_BUFFER2[4];


void arm_ctl_init(UART_HandleTypeDef *pHUART)
{
    // 初始化串口
    ARM_PS_UART.buffer[0] = ARM_RECV_BUFFER0;
    ARM_PS_UART.buffer[1] = ARM_RECV_BUFFER1;
    ARM_PS_UART.buffer[2] = ARM_RECV_BUFFER2;
    ARM_PS_UART.recv_len = sizeof(ARM_RECV_BUFFER0);
    ARM_PS_UART.pHUART = pHUART;
    ARM_PS_UART.recv_callback = NULL;
    ARM_EMM42.ps_uart_handle = &ARM_PS_UART;

    // 初始化步进电机结构体
    arm_define_t *arm = pARM_DEFINE;
    arm->deadzone.inner = true;

    // 初始化电机
    emm42_v5_init(&ARM_EMM42);
    arm_set_state(true);
    emm42_set_reach_wnd(&ARM_EMM42, ARM_DEFINE.motor_r.ID, 1);
    emm42_set_reach_wnd(&ARM_EMM42, ARM_DEFINE.motor_z.ID, 1);
    emm42_set_reach_wnd(&ARM_EMM42, ARM_DEFINE.motor_x.ID, 1);
    arm_position_update();
}

void arm_set_state(bool state)
{
    emm42_set_state(&ARM_EMM42, ARM_DEFINE.motor_r.ID, state, 0);
    emm42_set_state(&ARM_EMM42, ARM_DEFINE.motor_z.ID, state, 0);
    emm42_set_state(&ARM_EMM42, ARM_DEFINE.motor_x.ID, state, 0);
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

