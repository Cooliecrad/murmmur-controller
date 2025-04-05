#include "maintask.h"

#include <string.h>
#include "vision.h"
#include "coordinate.h"
#include "HWT101.h"
#include "Chassis.h"
#include "Arm.h"
#include "driver_middle.h"
#include "vision_adjust.h"
#include "servor_ctl.h"
/************宏定义**********************/
const static uint16_t LINE_SPEED = 650;
const static uint8_t LINE_ACC = 180;
/************物流搬运初赛场地上的点位**********************/

namespace points
{
    const Point2f ZERO {0.075, 0};
    const Point2f START {0.175, 0.175};
    const Point2f START2 {0.25, 0.175};
    const Point2f SCANQR {0.65,0.175};
    const Point2f MATERIAL {1.5, 0.175};
    const Point2f RIGHT_MID {1.1, 0.175};
    const Pose2f TEMP_STORAGE {{1.1, 1.95}, 180};
    const Pose2f TEST_STORAGE {{1.1, 1.95}, 0};
    const Point2f LEFT_UP {1.95, 1.95};
    const Pose2f PROCESS {{1.95, 1.13}, 90};
    const Point2f RIGHT_UP {1.95, 0.175};
    const Point2f MATERIAL2 {1.56, 0.18};
}

/**
 * @brief 前往出库点位
 */
void To_start(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, points::START);
#	ifdef __SAFE_MOVE_MODE
        chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
#	endif
}

void To_start2(void)
{
    chassis_to(100, 300, points::START2);
#	ifdef __SAFE_MOVE_MODE
        chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
#	endif
}

void to_right_middle(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, points::RIGHT_MID);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
}

/**
 * @brief 前往左上
 */
void To_Leftup(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, points::LEFT_UP);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
}

/**
 * @brief 前往右上
 */
void to_right_up(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, points::RIGHT_UP);
    chassis_rotate_abs(LINE_ACC,LINE_SPEED,0);
}

void To_Scan(void)
{
    chassis_to(LINE_ACC,LINE_SPEED,points::SCANQR);
#	ifdef __SAFE_MOVE_MODE
        chassis_rotate_abs(LINE_ACC,LINE_SPEED,0);
#	endif
}

/**
 * @brief 前往原料区
 */
void to_materials(void)
{
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
    chassis_to(LINE_ACC, LINE_SPEED, points::MATERIAL);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
}

/**
 * @brief 前往物料处理区
 */
void to_process(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, points::TEMP_STORAGE.xy);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
    vision_adjust_chassis(0, &points::TEMP_STORAGE);
}

/**
 * @brief 前往储存区
 */
void to_storage(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, points::PROCESS.xy);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
    vision_adjust_chassis(1, &points::PROCESS);
}

void To_stop(void)
{
    chassis_to(50, 200, points::ZERO);
}

void to_materials_2(void)
{
    chassis_to(100, 300, points::MATERIAL2);
    Point2f new_dst = coordinate_transform_Z_rotate(points::MATERIAL, -90);
    CHASSIS.pos = new_dst;
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
}

void materials_task(uint8_t round)
{
    arm_item_detect();

    /**
     * @brief 如果只扫描一次，那么只有首个需要抬升到检测位置
     */
#   ifdef __MATERIALS_TASK_ONLY_SCAN_ONCE
        const int start = 1;
        color_t color = vision_order(round, 0);
        uint8_t pos = vision_get_item(color); 
        arm_Z_standby(); // 还要降到就绪位置
        arm_action_get_materials(pos, color);
#   else
        const int start = 0; // 多次扫描，不需要额外工作
#   endif

    for (int i = start; i < ITEM_COUNT; i++)
    {
        color_t color = vision_order(round, i);
        uint8_t pos = vision_get_item(color); 
        arm_action_get_materials(pos, color);
    }
    vision_item_detect_stop(); // 通知上位机检测结束

    arm_standby(); // 翅膀关闭
}

void process_place_task(uint8_t round)
{
    // 机械爪&翅膀打开, 调整位置，扫描
    // claw_ctl(false, true);
    // arm_ring_detect();
    arm_item_detect();
    vision_subscribe_rings();

    // 放物块，拿物块
    arm_Z_standby();
    for (int i = 0; i < 3; i++)
    {
        color_t color = vision_order(round, i);
        arm_action_store_to_ground(color);
    }
    for (int i = 0; i < 3; i++)
    {
        color_t color = vision_order(round, i);
        arm_action_ground_to_store(color);
    }

    // 关闭翅膀，恢复机械臂
    wing_ctl(true, false);
    arm_standby();
}

void storage_place_task(uint8_t round)
{
    // 机械爪&翅膀打开, 调整位置，扫描
    // claw_ctl(false, true);
    arm_item_detect();
    vision_subscribe_rings();

    // 放物块
    arm_Z_standby();
    auto place_func = round == 0 ? arm_action_store_to_ground
                                 : arm_action_palletising;
    for (int i = 0; i < 3; i++)
    {
        color_t color = vision_order(round, i);
        place_func(color);
    }

    // 关闭翅膀，恢复机械臂
    wing_ctl(true, false);
    arm_standby();
}

void task(void)
{
    ///************第一圈**********************/
    To_start();
    
    To_Scan();
    
    
    vision_subscribe_order();
    vision_sync();
    vision_idle();

    to_materials();
    
    //执行 圆盘——》储存
    materials_task(0);

    to_right_middle();

    to_process();
    
    //执行 储存——》圆环——》储存
    process_place_task(0);

    To_Leftup();

    to_storage();
    
    //执行 存储——》圆环
    storage_place_task(0);
    
    to_right_up();
    
    ///************第二圈**********************/

    //执行 圆盘——》存储
    to_materials_2();
    materials_task(1);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);

    to_right_middle();

    to_process();
    
    process_place_task(1);

    To_Leftup();

    to_storage();
    
    //执行 码垛
    storage_place_task(1);
    
    to_right_up();
    
    To_start2();

    To_stop();
}