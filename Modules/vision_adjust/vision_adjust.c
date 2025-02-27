#include "vision_adjust.h"
#include "HWT101.h"
#include "Chassis.h"
#include "vision.h"
#include "coordinate.h"
#include "Arm.h"

const static float VISION_ADJUST_POS_THRESHOLD = 0.01;
/**
 * @brief 如果是位置模式旋转，必须大于ROTATE_THRESHOLD，否则会有问题
 */
const static float VISION_ADJUST_ANGLE_THRESHOLD = 2;
const static uint16_t VISION_ADJUST_SPEED = 500;
const static uint16_t VISION_ADJUST_ACC = 220;

void vision_calibrate_gyro(float target)
{
    // 上一帧实际的偏航角   
	while (!vision_info.instance.can_adjust_yaw) HAL_Delay(0);
    float tmp = target + vision_info.instance.last_frame_vision_angle;
    // 计算上一帧的误差
    tmp -= vision_info.instance.last_frame_gyro_yaw;
    // HWT101_set_calibrate(tmp);
}

Pose2f speed;
void vision_adjust_chassis(uint8_t pos, const Pose2f *target)
{
    stepmotor_rotate(&pARM_DEFINE->motor_r, -180);
    arm_move_sync();

    // 等待获取视觉数据
    vision_subscribe_pos(pos);
    vision_sync(ps_comm_type_POS_DETECT);
    vision_calibrate_gyro(target->angle); // 校准陀螺仪
    float distance = sqrtf(  powf(vision_info.point2f.x, 2)
                           + powf(vision_info.point2f.y, 2));
    // 检测是否可以接受
    while (   fabs(distance) > VISION_ADJUST_POS_THRESHOLD
           || fabs(vision_info.pos_detect) > VISION_ADJUST_ANGLE_THRESHOLD)
    {
#   ifdef __VISION_CALIBRATE_POS_MODE
        float speed = distance * VISION_ADJUST_SPEED;
        chassis_move(VISION_ADJUST_ACC, speed, vision_info.point2f);
        chassis_rotate_abs(VISION_ADJUST_ACC, 200, target->angle);
        // 确保没有延迟
        HAL_Delay(100);
        vision_sync(ps_comm_type_POS_DETECT);
		vision_calibrate_gyro(target->angle);
        distance = sqrtf(  powf(vision_info.point2f.x, 2)
                         + powf(vision_info.point2f.y, 2));
#   else
        // 速度模式校准
		PID_Caclulate(&PID_LOCATION_JUST_X, 0, -(float)vision_info.point2f.x);
		PID_Caclulate(&PID_LOCATION_JUST_Y, 0, -(float)vision_info.point2f.y);	
        // 校准角度直接用校准后的陀螺仪即可 
		// PID_Caclulate(&ANGLE_PID2, 0, HWT101_read_yaw());
		speed.xy.x = PID_LOCATION_JUST_X.OUT;
		speed.xy.y = PID_LOCATION_JUST_Y.OUT;
		// speed.angle = 0.3*ANGLE_PID2.OUT;
		chassis_move_speed(VISION_ADJUST_ACC, &speed);
        HAL_Delay(50);
        chassis_rotate(100, 100, -vision_info.pos_detect);
#   endif
    }
	emm42_halt(chassis_emm42_handle, 0, 0);
    // 直接校准车身
    Point2f new_dst = coordinate_transform_Z_rotate(target->xy, -90);
    CHASSIS.pos = new_dst;
    HWT101_calibrate(target->angle);

    // 视觉校准完毕让视觉不要再工作了
    vision_subscribe(ps_comm_type_IDLE_REQ);
}