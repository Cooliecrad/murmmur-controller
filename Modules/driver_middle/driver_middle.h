#ifndef __DRIVER_MIDDLE_H__
#define __DRIVER_MIDDLE_H__

#include "common.h"
#include "Arm.h"

/**
 * @brief 将color映射到ArmTarget
 */
const static ArmTarget COLOR_ARM_TARGET_MAP[] = {
    Ordinary2,  // color_none -> Ordinary2
    Red2,       // color_red -> Red2
    Green2,     // color_green -> Green2
    Blue2,      // color_blue -> Blue2
};

#endif