#ifndef __MMCAR_CONFIGURE_H__
#define __MMCAR_CONFIGURE_H__

/**
 * @brief 实验性功能：物料区单圈仅扫描一次。如果视觉支持仅扫描一次完成三个物品预测的功能
 * 那么可以打开这个选项，这将跳过每次夹取完毕回到扫描位置的过程，提高获取物料速度。
 */
#define __MATERIALS_TASK_ONLY_SCAN_ONCE

/**
 * @brief 额外实验性功能：物料整局取仅扫描一次
 */
// #define __MATERIALS_TASK_ONLY_SCAN_ONCE_V2

#ifdef __MATERIALS_TASK_ONLY_SCAN_ONCE_V2
#   ifndef __MATERIALS_TASK_ONLY_SCAN_ONCE
        static_assert(false, "要启用V2选项，必须启用__MATERIALS_TASK_ONLY_SCAN_ONCE");
#   endif
#endif

/**
 * @brief 安全移动模式。启动之后每次点位间移动会调整车子的姿态
 */
// #define __SAFE_MOVE_MODE

#endif