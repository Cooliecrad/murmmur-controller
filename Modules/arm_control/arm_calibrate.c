#include "arm_calibrate.h"


void arm_calibrate(arm_handle_t handle)
{
    Point3f target = {.x = STEPMOTOR_X_POSITIONS[2],
                      .y = STEPMOTOR_R_POSITIONS[2],
                      .z = STEPMOTOR_Z_POSITIONS[5]};
    // 更新机械臂位置, 计算零偏量
    stepmotor_position_update(&handle->motor_x);
    stepmotor_position_update(&handle->motor_r);
    stepmotor_position_update(&handle->motor_z);
#   ifdef __ENABLE_SOFTWARE_ZERO
        handle->motor_x.zero_shift = target.x - handle->motor_x.position;
        handle->motor_r.zero_shift = target.y - handle->motor_r.position;
        handle->motor_z.zero_shift = target.z - handle->motor_z.position;
#   endif
}
