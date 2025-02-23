#include "arm_calibrate.h"

void arm_calibrate(arm_handle_t handle)
{
    Point3f target = {.x = StepMotor_Forward_Store[2],
                      .y = StepMotor_R_StorePosition[2],
                      .z = StepMotor_Z_Position[5]};
    // 更新机械臂位置, 计算零偏量
    stepmotor_position_update(handle->emm42_handle, handle->PULSE_PER_ROUND,
                              &handle->motor_x);
    handle->motor_x.zero_shift = target.x - handle->motor_x.position;
    stepmotor_position_update(handle->emm42_handle, handle->PULSE_PER_ROUND,
                              &handle->motor_r);
    handle->motor_r.zero_shift = target.y - handle->motor_r.position;
    stepmotor_position_update(handle->emm42_handle, handle->PULSE_PER_ROUND,
                              &handle->motor_z);
    handle->motor_z.zero_shift = target.z - handle->motor_z.position;
}