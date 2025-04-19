#include "maintask.h"

#include <string.h>
#include "vision.h"
#include "coordinate.h"
#include "HWT101.h"
#include "Chassis.h"
#include "Arm.h"
#include "arm_seqs.h"
#include "driver_middle.h"
#include "vision_adjust.h"
#include "servor_ctl.h"
#include "screen.h"
#include "vision.h"
/************宏定义**********************/
// static uint16_t LINE_SPEED = 500;
// static uint8_t LINE_ACC = 200;
static uint16_t LINE_SPEED = 1000;
static uint8_t LINE_ACC = 180;
static uint16_t ROTATE_SPEED = 1000;
static uint8_t ROTATE_ACC = 200;
/************物流搬运初赛场地上的点位**********************/
// namespace points
// {
//      const Point2f START {0.175, 0.175};
//      const Point2f SCANQR {0.65,0.175};
//      const Pose2f MATERIAL {{1.51, 0.175}, 0};
//      const Point2f RIGHT_MID {1.1, 0.175};
//      const Pose2f PROCESS {{1.1, 1.95}, 180};
//      const Pose2f TEST_STORAGE {{1.1, 1.95}, 0};
//      const Point2f LEFT_UP {1.92, 1.95};
//      const Pose2f STORAGE {{1.92, 1.11}, 90};
//      const Point2f RIGHT_UP {1.92, 0.205}; // 为什么不平行？见上面的note
//      const Point2f MATERIAL2 {1.50, 0.20};
//      const Pose2f MID_UP {STORAGE}; // 应该对应STORAGE
//      const Pose2f MID_BOTTOM {STORAGE}; // 发车区下面
//      const Point2f START2 {0.25, 0.20};
//      const Point2f ZERO {0, 0.04};
// }

namespace points
{
     const Point2f START {0.175, 0.175};
     const Point2f SCANQR {0.65,0.175};
     const Pose2f MATERIAL {{1.51, 0.175}, 0};
     const Point2f RIGHT_MID {1.1, 0.175};
     const Pose2f PROCESS {{1.1, 1.95}, 180};
     const Pose2f TEST_STORAGE {{1.1, 1.95}, 0};
    //  const Point2f LEFT_UP {1.92, 1.95};
     const Pose2f STORAGE {{0.28, 1.0625}, 90};
    //  const Point2f MATERIAL2 {1.50, 0.20};
     const Point2f START2 {0.25, 0.20};
     const Point2f ZERO {0, 0.04};

     const Point2f MIDDLE {1.1, 1.0625}; // 中部区
     const Point2f LEFT_BOTTOM {0.28, 1.95}; // 左底部
     const Pose2f MID_BOTTOM {STORAGE};
}


/**
 * @brief 前往出库点位
 */
void To_start(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, points::START);
#	ifdef __SAFE_MOVE_MODE
        chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 0);
#	endif
}

void To_start2(void)
{
    chassis_to(100, 300, points::START2);
    chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 0);
#	ifdef __SAFE_MOVE_MODE
        chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 0);
#	endif
}

void to_right_middle(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, points::RIGHT_MID);
    chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 90);
}

/**
 * @brief 前往左上
 */
void to_left_bottom(void)
{
// #	ifdef __SAFE_MOVE_MODE
//         chassis_rotate_abs(LINE_ACC,LINE_SPEED,180);
// #	endif
//     chassis_to(LINE_ACC, LINE_SPEED, points::LEFT_UP);
//     chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 90);
}

/**
 * @brief 前往右上
 */
void to_right_up(void)
{
    chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 180);
    chassis_to(LINE_ACC, LINE_SPEED, points::MIDDLE);
    chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 90);
    chassis_to(LINE_ACC, LINE_SPEED, points::RIGHT_MID);
    chassis_rotate_abs(LINE_ACC,LINE_SPEED,0);
    chassis_to(LINE_ACC, LINE_SPEED, points::MATERIAL.xy);
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
void to_materials()
{
#   ifdef __SAFE_MOVE_MODE
        chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 0);
#   endif
    chassis_to(LINE_ACC, LINE_SPEED, points::MATERIAL.xy);
}

/**
 * @brief 前往物料处理区
 */
void to_process(uint8_t barricade)
{
    if (barricade == 1 || barricade == 3)
    {
        chassis_to(LINE_ACC, LINE_SPEED, points::PROCESS.xy);
        chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 180);
    } else if (barricade == 2)
    {
        chassis_to(LINE_ACC, LINE_SPEED, points::MIDDLE); // 中部
        chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 180);
        chassis_to(LINE_ACC, LINE_SPEED, points::MID_BOTTOM.xy); // 中下区
        chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 90);
        chassis_to(LINE_ACC, LINE_SPEED, points::LEFT_BOTTOM); // 左下区
        chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 180);
        chassis_to(LINE_ACC, LINE_SPEED, points::PROCESS.xy); // 处理区
    }
#   ifdef __VISION_ADJUST_ENABLE
        vision_adjust_chassis(vision_adjust_PROCESS, &points::PROCESS);
#   endif
}

/**
 * @brief 前往储存区
 */
void to_storage(uint8_t barricade)
{
    if (barricade != 3)
    {
        chassis_rotate_abs(ROTATE_ACC,ROTATE_SPEED, 180);
        chassis_to(LINE_ACC, LINE_SPEED, points::LEFT_BOTTOM);
        chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 270);
        chassis_to(LINE_ACC, LINE_SPEED, points::STORAGE.xy);
    } else
    {
        chassis_rotate(ROTATE_ACC, ROTATE_SPEED, 270);
        chassis_to(LINE_ACC, LINE_SPEED, points::MIDDLE);
        chassis_rotate(ROTATE_ACC, ROTATE_SPEED, 180);
        chassis_to(LINE_ACC, LINE_SPEED, points::STORAGE.xy);
        chassis_rotate(ROTATE_ACC, ROTATE_SPEED, 270);
    }
#   ifdef __VISION_ADJUST_ENABLE
        vision_adjust_chassis(vision_adjust_STORAGE, &points::STORAGE);
#   endif
}

void To_stop(void)
{
    chassis_to(50, 200, points::ZERO);
}

void to_materials_2(void)
{
    // chassis_to(100, 300, points::MATERIAL2);
    Point2f new_dst = coordinate_transform_Z_rotate(points::MATERIAL.xy, -90);
    CHASSIS.pos = new_dst;
    chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 0);
}

void scan_task()
{
#   ifdef __MATERIALS_TASK_ENABLE
        vision_subscribe_order();
        vision_sync();
        vision_idle_pos(idle_inst_HUNG);
        screen_show(vision_order_all());
#   endif
}

void materials_task(uint8_t round)
{
    chassis_move(LINE_ACC*0.5, LINE_SPEED*0.5, Point2f {0.02, 0});
#   ifdef __MATERIALS_TASK_ENABLE
        arm_item_detect();
        clear_item();
#       ifdef __MATERIALS_TASK_ONLY_SCAN_ONCE_V2
            if (round == 0) vision_item_detect_stop(); // 通知上位机检测结束
#       else
            vision_item_detect_stop();
#       endif
        HAL_Delay(20);
        /**
         * @brief 如果只扫描一次，那么只有首个需要抬升到检测位置
         */
#       ifdef __MATERIALS_TASK_ONLY_SCAN_ONCE
            const int start = 1;
            color_t color = vision_order(round, 0);
            uint8_t pos = vision_get_item(color); 
            arm_Z_standby(); // 还要降到就绪位置
            arm_action_get_materials(pos, color);
#       else
            const int start = 0; // 多次扫描，不需要额外工作
#       endif

        for (int i = start; i < ITEM_COUNT; i++)
        {
            color_t color = vision_order(round, i);
            uint8_t pos = vision_get_item(color);
            arm_action_get_materials(pos, color);
        }

        arm_standby(); // 翅膀关闭
#   endif
    chassis_move(LINE_ACC*0.5, LINE_SPEED*0.5, Point2f {-0.02, 0});
}

void process_place_task(uint8_t round)
{
#   ifdef  __MATERIALS_TASK_ENABLE
        // 机械爪&翅膀打开, 调整位置，扫描
        // claw_ctl(false, true);
        // arm_ring_detect();
        arm_item_detect();

        if (round != 0xff) // 测试用：传入0xff圈，使用上一次的参数跑第0圈，不请求视觉
            vision_subscribe_rings(1);
        else
            round = 0;

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
#   endif
}

void storage_place_task(uint8_t round)
{
#   ifdef __MATERIALS_TASK_ENABLE
    // 机械爪&翅膀打开, 调整位置，扫描
    // claw_ctl(false, true);
    arm_item_detect();
    vision_subscribe_rings(0);

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
#   endif
}

void task(void)
{
    screen_clear();
    ///************第一圈**********************/
    To_start();
    
    To_Scan();
    
    // 扫码任务
    scan_task();

    to_materials();

    //执行 圆盘——》储存
    materials_task(0);

    to_right_middle();

    to_process(vision_order(0, 0));
    
    //执行 储存——》圆环——》储存
    process_place_task(0);

    to_storage(vision_order(0, 0));
    
    //执行 存储——》圆环
    storage_place_task(0);
    
    To_start2();
    // to_right_up();
    
    ///************第二圈**********************/

    //执行 圆盘——》存储
    // to_materials_2();
    to_materials();
    materials_task(1);
    chassis_rotate_abs(ROTATE_ACC, ROTATE_SPEED, 0);

    to_right_middle();

    to_process(vision_order(0, 0));
    
    process_place_task(1);

    to_storage(vision_order(0, 0));
    
    //执行 码垛
    storage_place_task(1);
    
    To_start2();

    To_stop();
}