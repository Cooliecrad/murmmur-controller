#ifndef __PS_VISION_ADJUST_H__
#define __PS_VISION_ADJUST_H__

#include "common.h"
#include <stdbool.h>

/**
 * @brief 使用位置/速度模式来控制视觉校准
 */
#define __VISION_CALIBRATE_POS_MODE

/**
 * @brief 根据目标角度和视觉的误差角度，校准陀螺仪的角度
 * @param target 目标角度
 */
void vision_calibrate_gyro(float target);

/**
 * @brief 根据当前视觉信息校准位姿
 * @param target 视觉校正的目标位置
 */
void vision_adjust_chassis(Pose2f *target);

#endif