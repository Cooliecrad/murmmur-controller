#include "vision_adjust.h"
#include "HWT101.h"
#include "Chassis.h"
#include "vision.h"
#include "using_PID.h"

const static float VISION_ADJUST_POS_THRESHOLD = 0.001;
const static float VISION_ADJUST_ANGLE_THRESHOLD = 0.1;
const static uint16_t VISION_ADJUST_SPEED = 1000;
const static uint16_t VISION_ADJUST_ACC = 150;

void vision_calibrate_gyro(float target)
{
    // 上一帧实际的偏航角
    float tmp = target - vision_info.instance.last_frame_vision_angle;
    // 计算上一帧的误差
    tmp -= vision_info.instance.last_frame_gyro_yaw;
    HWT101_set_calibrate(tmp);
}

void vision_adjust_chassis(Pose2f *target)
{
    // 等待获取视觉数据
    vision_open(ps_comm_type_ring_angle_detect);
    vision_calibrate_gyro(target->angle); // 校准陀螺仪
    // 检测是否可以接受
    while (   fabs(vision_info.ring_pos.x) > VISION_ADJUST_POS_THRESHOLD
           || fabs(vision_info.ring_pos.x) > VISION_ADJUST_POS_THRESHOLD
           || fabs(vision_info.ring_angle) > VISION_ADJUST_ANGLE_THRESHOLD)
    {
#   ifdef __VISION_CALIBRATE_POS_MODE
        chassis_move(VISION_ADJUST_ACC, (fabs(vision_info.ring_pos.x) + fabs(vision_info.ring_pos.y)) *VISION_ADJUST_SPEED, vision_info.ring_pos);
        chassis_arrived();
        chassis_rotate_abs(VISION_ADJUST_ACC, 150, target->angle);
        // 确保没有延迟
        vision_open(ps_comm_type_ring_angle_detect);

#   else
        // 速度模式校准
		PID_Caclulate(&PID_LOCATION_JUST_X, 0, -(float)vision_info.ring_pos.x);
		PID_Caclulate(&PID_LOCATION_JUST_Y, 0, -(float)vision_info.ring_pos.y);	
        // 校准角度直接用校准后的陀螺仪即可
		PID_Caclulate(&ANGLE_PID2, target->angle, HWT101_read_yaw());
		Pose2f speed;
		speed.xy.x = 5*PID_LOCATION_JUST_X.OUT;
		speed.xy.y = 5*PID_LOCATION_JUST_Y.OUT;
		speed.angle = -ANGLE_PID2.OUT;
		chassis_move_speed(VISION_ADJUST_ACC, &speed);
        HAL_Delay(2);
#   endif
    }
	emm42_halt(chassis_uart_handle, 0, 0);
    // 直接校准车身
    CHASSIS.pos = target->xy;
    HWT101_calibrate(target->angle);
}