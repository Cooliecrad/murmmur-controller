#include "Arm.h"
#include "ps_stmt.h"

#include "stm32h750xx.h"

using namespace ps;

emm42::emm42_v5 *arm_emm42_handle = nullptr;
motor::Stepmotor *motor_r = nullptr;
motor::Stepmotor *motor_x = nullptr;
motor::Stepmotor *motor_z = nullptr;
motor::Stepmotor *motor_xzr[3];
float CLAW_DISTANCE = 135;

namespace
{
    // 供通信使用
    uint8_t ARM_BF0[emm42::MAX_FRAME_LENGTH];
    uint8_t ARM_BF1[emm42::MAX_FRAME_LENGTH];
    uint8_t ARM_BF2[emm42::MAX_FRAME_LENGTH];
    uint8_t *ARM_BUFFER[3] = {ARM_BF0, ARM_BF1, ARM_BF2};
    // 机械臂结构定义
    Point2f center {-197, 48};
    // 机械臂微动开关对应的GPIO
    GPIO_TypeDef *Z_CLICK_GPIOx = GPIOD;
    const uint16_t Z_CLICK_GPIO_PIN = GPIO_PIN_6;
    GPIO_TypeDef *X_CLICK_GPIOx = GPIOD;
    const uint16_t X_CLICK_GPIO_PIN = GPIO_PIN_7;
}

// /**
//  * @brief 机械臂基础控制
//  */
// ps::ArmDefine ARM_DEFINE = {
//     .center = {
//         .x = -197,
//         .y = 48
//     },
//     .claw_distance = 135,
//     .motor_r = {
//     }
// //     .motor_r = {
// //         .ID = 1,
// // 		.PPR = 3200,
// //         .ratio = 1. / 360. * 100. / 16. * 3200,
// //         .acc = 215,
// //         .speed = 2100,
// // #		ifdef __ENABLE_SOFTWARE_ZERO
// // 			.zero_shift = 0,
// // #		endif
// //         .lower = -360,
// //         .upper = 360
// //     },

//     .motor_x = {
//         .ID = 3,
// 		.PPR = 3200,
//         .ratio = 50,
//         .acc = 255,
//         .speed = 2300,
// #		ifdef __ENABLE_SOFTWARE_ZERO
// 			.zero_shift = 0,
// #		endif
//         .lower = 0,
//         .upper = 300,
//     },

//     .motor_z = {
//         .ID = 2,
// 		.PPR = 3200,
//         .ratio = 80,
//         .acc = 240,
//         .speed = 2300,
// #		ifdef __ENABLE_SOFTWARE_ZERO
// 			.zero_shift = 0,
// #		endif
//         .lower = -140,
//         .upper = 0,
//     }
// };

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
    // 初始化通信
    arm_emm42_handle = new emm42::emm42_v5 {pHUART, ARM_BUFFER};
    arm_emm42_handle->start();

    // 设定R电机
    motor_r = new motor::Stepmotor {*arm_emm42_handle,
                                    1, 1. / 360. * 100. / 16. * 3200};
    motor_r->default_acc = 240;
    // motor_z->default_acc = 248;
    motor_r->default_speed = 2100;
    motor_r->set_protect(-360, 360);
    // 设定X电机
    motor_x = new motor::Stepmotor {*arm_emm42_handle, 3, 50};
    motor_x->default_acc = 252;
    // motor_z->default_acc = 254;
    motor_x->default_speed = 2300;
    motor_x->set_protect(0, 300);
    // 设定Z电机
    motor_z = new motor::Stepmotor {*arm_emm42_handle, 2, 80};
    motor_z->default_acc = 252;
    // motor_z->default_acc = 254;
    motor_z->default_speed = 2300;
    motor_z->set_protect(-220, 0);
    // 设定电机数组
    motor_xzr[0] = motor_x;
    motor_xzr[1] = motor_z;
    motor_xzr[2] = motor_r;

    // 初始化电机
    arm_position_update();
}

void arm_zero_reset()
{
    arm_set_state(SET); // 使能机械臂后开始

    // 设置电机运动来触发零点
    auto x_is_clicked = []() -> bool {
        return HAL_GPIO_ReadPin(X_CLICK_GPIOx, X_CLICK_GPIO_PIN) == GPIO_PIN_RESET;
    };
    motor_x->calibrate(x_is_clicked, -30, 255, 100);
    motor_x->arrived(); // 必须先归零x再归零z，不然会打到摄像头！

    auto z_is_clicked = []() -> bool {
        return HAL_GPIO_ReadPin(Z_CLICK_GPIOx, Z_CLICK_GPIO_PIN) == GPIO_PIN_RESET;
    };
    motor_z->calibrate(z_is_clicked, 30, 255, 100);
    motor_z->arrived();
    motor_r->move_to(0);
}

void arm_set_state(int state)
{
    bool _state = state ? true : false;
    motor_r->set_state(_state);
    motor_x->set_state(_state);
    motor_z->set_state(_state);
}

void arm_position_update(void)
{
    motor_r->update();
    motor_x->update();
    motor_z->update();
}

void arm_move_sync(void)
{
    motor_r->arrived();
    motor_x->arrived();
    motor_z->arrived();
}

// void arm_calibrate()
// {
//     Point3f target = {.x = STEPMOTOR_X_POSITIONS[2],
//                       .y = STEPMOTOR_R_POSITIONS[2],
//                       .z = STEPMOTOR_Z_POSITIONS[5]};
//     // 更新机械臂位置, 计算零偏量
//     stepmotor_position_update(&handle->motor_x);
//     stepmotor_position_update(&handle->motor_r);
//     stepmotor_position_update(&handle->motor_z);
// #   ifdef __ENABLE_SOFTWARE_ZERO
//         handle->motor_x.zero_shift = target.x - handle->motor_x.position;
//         handle->motor_r.zero_shift = target.y - handle->motor_r.position;
//         handle->motor_z.zero_shift = target.z - handle->motor_z.position;
// #   endif
// }