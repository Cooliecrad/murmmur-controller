#include "vision_adjust.h"
#include "HWT101.h"
#include "chassis.h"
#include "vision.h"
#include "coordinate.h"
#include "arm_seqs.h"

namespace
{
    const float POS_THRESHOLD = 0.003;
    const float POS_RATE = 0.8;
    const uint16_t SPEED = 500;
    const uint16_t ACC = 220;
}

Pose2f speed;
void vision_adjust_chassis(uint8_t pos, const Pose2f *target)
{
    arm_item_detect();

    // 等待获取视觉数据
    Point2f pos_error = vision_get_pos(pos);
    float distance = sqrtf(powf(pos_error.x, 2) + powf(pos_error.y, 2)) * POS_RATE;
    // 检测位置是否可以接受
    while (fabs(distance) > POS_THRESHOLD)
    {
#   ifdef __VISION_CALIBRATE_POS_MODE
        float speed = distance * SPEED;
        chassis_move(ACC, speed, pos_error);
        chassis_rotate_abs(ACC, 200, target->angle);
        // 下一轮校准
        pos_error = vision_get_pos(pos);
        distance = sqrtf(powf(pos_error.x, 2) + powf(pos_error.y, 2)) * POS_RATE;
#   else
        // 速度模式校准
		PID_Caclulate(&PID_LOCATION_JUST_X, 0, -(float)vision_info.point2f.x);
		PID_Caclulate(&PID_LOCATION_JUST_Y, 0, -(float)vision_info.point2f.y);	
        // 校准角度直接用校准后的陀螺仪即可 
		// PID_Caclulate(&ANGLE_PID2, 0, HWT101_read_yaw());
		speed.xy.x = PID_LOCATION_JUST_X.OUT;
		speed.xy.y = PID_LOCATION_JUST_Y.OUT;
		// speed.angle = 0.3*ANGLE_PID2.OUT;
		chassis_move_speed(ACC, &speed);
        HAL_Delay(50);
        chassis_rotate(100, 100, -vision_info.pos_detect);
#   endif
    }
    chassis_halt();
    // 直接更新坐标位置
    Point2f new_dst = coordinate_transform_Z_rotate(target->xy, -90);
    CHASSIS.pos = new_dst;

    // 视觉校准完毕让视觉不要再工作了
    vision_idle();
}