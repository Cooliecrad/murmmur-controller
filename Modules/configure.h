#ifndef __MMCAR_CONFIGURE_H__
#define __MMCAR_CONFIGURE_H__

/**
 * @brief 物料区仅扫描一次。如果视觉支持仅扫描一次完成三个物品预测的功能，那么可以打开这个选项，
 * 这将跳过每次夹取完毕回到扫描位置的过程，提高获取物料速度。
 */
#define __MATERIALS_TASK_ONLY_SCAN_ONCE

/**
 * @brief 安全移动模式。启动之后每次点位间移动会调整车子的姿态
 */
// #define __SAFE_MOVE_MODE

#endif