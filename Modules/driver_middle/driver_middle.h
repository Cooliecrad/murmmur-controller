#ifndef __DRIVER_MIDDLE_H__
#define __DRIVER_MIDDLE_H__

#include "common.h"

/**
 * @brief 将color映射到ArmTarget
 */
const static ArmTarget COLOR_ARM_TARGET_MAP[] = {
    Ordinary2,  // color_none -> Ordinary2
    Red2,       // color_red -> Red2
    Green2,     // color_green -> Green2
    Blue2,      // color_blue -> Blue2
};

/**
 * @brief 将ArmTarget映射到color
 */
const static color_t ARM_TARGET_COLOR_MAP[] = {
    color_none,
    color_red,
    color_green,
    color_blue
};

#endif