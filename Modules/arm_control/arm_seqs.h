#ifndef __ARM_SEQUENCES_H__
#define __ARM_SEQUENCES_H__

#include "Arm.h"

/**
 * @brief 机械臂动作序列定义
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	Ordinary2 = 0,
    Red2 = 1,
    Green2 = 2,
    Blue2 = 3,
    Dtct2 = 4,
    Limit = 5
} ArmTarget;

typedef enum
{
    Ordinary3 = 0,
    Scan = 1,			 // 扫码
    // Dtct = 2,			 // 在圆盘上识别
    Plate_To_Store = 3,	 // 从圆盘拾取放到存储机构
    Store_To_Ground = 4, // 从存储机构摆放到地面
    Ground_To_Store = 5, // 从地面拾取放到存储机构
    Maduo = 6,			 // 码垛
    Store_Open = 7,		 // 存储机构开
    Store_Close = 8,     // 存储机构关
    Store_Scan,
} ArmAction;

extern const float STEPMOTOR_R_POSITIONS[];
extern const float STEPMOTOR_X_POSITIONS[];
extern const float STEPMOTOR_Z_POSITIONS[];


/**
 * @brief 控制机械臂到达指定位置
 */
void arm_move(Point2f XY, float Z);

/**
 * @brief 机械臂抓取指定点位
 * @param point 要放置的点
 * @param z_smooth 平滑下降
 */
void arm_ground_place(const Point3f *point, bool smooth);

/**
 * @brief 安全和机械臂配合的机械爪控制
 * @param open_close false: 开, true: 闭
 * @note 只有机械臂稳定下来，机械爪才会接受控制。如果要在机械臂不稳定时控制机械爪，请
 * 使用servor_ctl.h中的函数
 */
void arm_claw_ctl(bool open_close);

/**
 * @brief 机械臂就绪位置
 * 机械臂：R = 0, Z = 原点， X = 原点
 */
void arm_standby(void);

/**
 * @brief 就绪位置 -> 识别物料位置
 * 翅膀：张开
 * 机械爪：张开
 * 机械臂：R = -180, Z = 原点， X = 原点
 */
void arm_item_detect(void);

/**
 * @brief 就绪位置 -> 夹取物料位置
 */
void arm_cat_materials(uint8_t pos);

/**
 * @brief 就绪位置 -> 翅膀上对应物料位置
 * @note 机械爪不会打开
 */
void arm_store_item(color_t color);

/**
 * @brief 机械臂扫描圆环位置
 * R = -180, Z = 扫描高度， X = 扫描前伸
 */
void arm_ring_detect(void);

/**
 * @brief 检测位置 -> 圆盘抓取物料 -> 存放 -> 检测位置
 */
void arm_action_get_materials(uint8_t pos, color_t color);

/**
 * @brief 就绪位置 -> 存储机构取出物料 -> 放置在圆环 -> 就绪位置
 */
void arm_action_store_to_ground(color_t color);

/**
 * @brief 就绪位置 -> 取出物品 -> 码垛 -> 就绪位置
 */
void arm_action_palletising(color_t color);

/**
 * @brief 就绪位置 -> 地面拾取物料 -> 存放到存储机构 -> 就绪位置
 */
void arm_action_ground_to_store(color_t color);

/**
 * @brief 翅膀抓取物块测试
 */
void arm_action_wing_test();

uint16_t angle_to_position(float angle);

void Arm_Action(ArmAction action, ArmTarget color);

#ifdef __cplusplus
}
#endif

#endif