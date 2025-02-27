#include "maintask.h"

#include "vision.h"
#include "HWT101.h"
#include "string.h"
#include "Chassis.h"
#include "Arm.h"
#include "driver_middle.h"
#include "common.h"
#include "vision_adjust.h"
#include "servor_ctl.h"
///************宏定义**********************/

const static uint16_t LINE_SPEED = 650;
const static uint8_t LINE_ACC = 180;

#define targetpositionx 0
#define targetpositiony 0
#define __min(x, y) x < y ? x : y
#define __max(x, y) x > y ? x : y
///************物流搬运初赛场地上的点位**********************/
const static Point2f POINT_ZERO = {.x = 0.075, .y = 0};
const static Point2f POINT_START = {.x = 0.175, .y = 0.175};
const static Point2f POINT_START2 = {.x = 0.25, .y = 0.175};
const static Point2f POINT_SCANQR = {.x = 0.65,.y = 0.175};
const static Point2f POINT_MATERIAL = {.x = 1.5, .y = 0.175}; //0.175};
const static Point2f POINT_RIGHT_MID = {.x = 1.1, .y = 0.175};
const static Pose2f POSE_TEMP_STORAGE = {.xy = {.x = 1.1, .y = 1.95}, .angle = 180};
const static Point2f POINT_LEFT_UP = {.x = 1.98, .y = 1.95};
const static Pose2f POSE_PROCESS = {.xy = {.x = 1.98, .y = 1.13}, .angle = 90};
const static Point2f POINT_RIGHT_UP = {.x = 1.98, .y = 0.175};
const static Point2f POINT_MATERIAL2 = {.x = 1.59, .y = 0.18};
///***************************************************************/
Pose2f pose_error; // meter
Point2f point_move; // meter
///*********************************决赛点位**********************/

// float ScanPoint[2] = {0.62, 0.17}; // 0.18
// float StorePoint[2] = {1.35, 0.17};

// float KneePoint0[2] = {1.755, 0.17};
// float GreenPoint1[2] = {1.755, 1};
// float KneePoint1[2] = {1.755, 1.63}; // 回来：
// float GreenPoint2[2] = {0.92, 1.63};
// float KneePoint2[2] = {0.02, 1.63};				 // 0.04
// float KneePoint2_returnGreen1[2] = {0.22, 1.63}; // 0.04		先去0.2的位置再回来0.5的位置定坐标，然后去到0的位置再转270度
// float KneePoint2ToGreen2[2] = {0, 1.63};
// float KneePoint3[2] = {0, 0.17};
// float KneePoint3_returntoGreen2[2] = {0, 0.53};

// float KneePoint4[2] = {0, -0.1};

/////////////////////////////////
// float StorePoint_2[2] = {1.31, 0.13};

// float GreenPoint1_2[2] = {1.68, 1.1};
// float GreenPoint2_2[2] = {0.95, 1.68};

// float KneePoint0_2[2] = {1.725, 0.17};
// float KneePoint1_2[2] = {1.59, 1.67}; // 回来：
// float KneePoint2_2[2] = {0.17, 1.65};
// float KneePoint3_2[2] = {0.2, 1.8};
///***************************************************************/

// uint8_t ColorSequence1[3] = {1};
// uint8_t ColorSequence2[3] = {1};

// uint8_t RingColoSequencer[3] = {1, 2, 3};

///********************运行第二圈时，坐标会产生物产，要进行纠正、更新*****************************/
// void CoordUpdate(void)
//{
//	Chassis.coord.Mileage.XDistance = ScanPoint[0];
//	Chassis.coord.Mileage.YDistance = ScanPoint[1];
// }

// void KneePoint1Update_1(void)
//{
//	KneePoint1[0] = GreenPoint1[0];
//	KneePoint1[1] = GreenPoint1[1] + 0.83;
// }
// void KneePoint1Update_2(void)
//{
//	KneePoint1[0] = GreenPoint2[0] + 0.2;
//	KneePoint1[1] = GreenPoint2[1];
// }
///***************************************************************/

/**
 * @brief 前往出库点位
 */
void To_start(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, POINT_START);
#	ifdef __SAFE_MOVE_MODE
        chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
#	endif
}

void To_start2(void)
{
    chassis_to(100, 300, POINT_START2);
#	ifdef __SAFE_MOVE_MODE
        chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
#	endif
}

void to_right_middle(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, POINT_RIGHT_MID);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
}

/**
 * @brief 前往左上
 */
void To_Leftup(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, POINT_LEFT_UP);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
}

/**
 * @brief 前往右上
 */
void to_right_up(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, POINT_RIGHT_UP);
    chassis_rotate_abs(LINE_ACC,LINE_SPEED,0);
}

void To_Scan(void)
{
    chassis_to(LINE_ACC,LINE_SPEED,POINT_SCANQR);
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
    chassis_to(LINE_ACC, LINE_SPEED, POINT_MATERIAL);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
}

/**
 * @brief 前往物料处理区
 */
void to_process(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, POSE_TEMP_STORAGE.xy);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
    vision_adjust_chassis(0, &POSE_TEMP_STORAGE);
}

/**
 * @brief 前往储存区
 */
void to_storage(void)
{
    chassis_to(LINE_ACC, LINE_SPEED, POSE_PROCESS.xy);
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
    vision_adjust_chassis(1, &POSE_PROCESS);
}

void To_stop(void)
{
    chassis_to(50, 200, POINT_ZERO);
}

void to_materials_2(void)
{
    chassis_to(100, 300, POINT_MATERIAL2);
    Point2f new_dst = coordinate_transform_Z_rotate(POINT_MATERIAL, -90);
    CHASSIS.pos = new_dst;
    chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
}

void material_task(uint8_t round)
{
    arm_item_detect();

    // 看颜色是否与期望顺序一致
    for (int i = 0; i < 3; i++)
    {
        color_t color = vision_get_order(round, i);
        vision_subscribe_item(color);
        vision_sync(ps_comm_type_ITEM_DETECT);
        vision_subscribe(ps_comm_type_IDLE_REQ);
        uint8_t pos = (uint8_t)vision_info.item_detect;
        arm_action_get_materials(pos, color);
    }
    vision_item_detect_stop(); // 通知上位机检测结束

    arm_standby(); // 翅膀关闭
}

void process_place_task(uint8_t round)
{
    // 机械爪&翅膀打开, 调整位置，扫描
    claw_ctl(false, true);
    arm_ring_detect();
    wing_ctl(false, false);
    vision_subscribe_rings();

    // 放物块，拿物块
    for (int i = 0; i < 3; i++)
    {
        color_t color = vision_get_order(round, i);
        arm_action_store_to_ground(color);
    }
    for (int i = 0; i < 3; i++)
    {
        color_t color = vision_get_order(round, i);
        arm_action_ground_to_store(color);
    }

    // 关闭翅膀，恢复机械臂
    wing_ctl(true, false);
    arm_standby();
}

void storage_place_task(uint8_t round)
{
    // 机械爪&翅膀打开, 调整位置，扫描
    claw_ctl(false, true);
    arm_ring_detect();
    wing_ctl(false, false);
    vision_subscribe_rings();

    // 放物块
    void (*place_func)(color_t) = round == 0 ? arm_action_store_to_ground
                                             : arm_action_palletising;
    for (int i = 0; i < 3; i++)
    {
        color_t color = vision_get_order(round, i);
        place_func(color);
    }

    // 关闭翅膀，恢复机械臂
    wing_ctl(true, false);
    arm_standby();
}

void Taskpoint(void)
{
    ///************第一圈**********************/
    To_start();
    
    To_Scan();
    
    vision_subscribe(ps_comm_type_SCAN_QR_REQ);
    vision_sync(ps_comm_type_SCAN_QR);
    vision_subscribe(ps_comm_type_IDLE_REQ);

    to_materials();
    
    //执行 圆盘——》储存
    material_task(0);

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
    material_task(1);
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

void my_debug(uint8_t flag)
{
    if (flag == 5)
        vision_adjust_chassis(0, &POSE_TEMP_STORAGE);
    if (flag == 6)
    {
        process_place_task(1);
    }
    if (flag == 7)
    {
        material_task(0);
    }
    if (flag == 8)
    {
        storage_place_task(1);
    }
}
