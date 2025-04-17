#ifndef __PS_VISION_ADJUST_H__
#define __PS_VISION_ADJUST_H__

#include "common.h"

/**
 * @brief 使用位置/速度模式来控制视觉校准
 */
#define __VISION_CALIBRATE_POS_MODE

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 根据当前视觉信息校准位姿
 * @param target 视觉校正的目标位置
 */
void vision_adjust_chassis(vision_adjust_t index, const Pose2f *target);

#ifdef __cplusplus
}
#endif

#endif