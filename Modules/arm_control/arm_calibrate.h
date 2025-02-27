#ifndef __ARM_CALIBRATE_H__
#define __ARM_CALIBRATE_H__

#include "arm_seqs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 用户将机械臂停在蓝色物块储存区储存的状态后调用本校准函数
 */
void arm_calibrate(arm_handle_t handle);


#ifdef __cplusplus
}
#endif

#endif