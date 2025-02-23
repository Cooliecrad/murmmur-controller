#include "maintask.h"

#include "vision.h"
#include "HWT101.h"
#include "string.h"
#include "Chassis.h"
#include "Arm.h"
#include "Communication.h"
#include "driver_middle.h"
#include "common.h"
#include "vision_adjust.h"
#include "using_PID.h"
#include "servor_ctl.h"
///************宏定义**********************/

const static uint16_t LINE_SPEED = 650;
const static uint8_t LINE_ACC = 180;

#define targetpositionx 0
#define targetpositiony 0
#define __min(x, y) x < y ? x : y
#define __max(x, y) x > y ? x : y
///************物流搬运初赛场地上的点位**********************/
const static Point2f POINT_ZERO = {.x = 0, .y = 0};
const static Point2f POINT_START = {.x = 0.175, .y = 0.175};
const static Point2f POINT_SCANQR = {.x = 0.65,.y = 0.175};
const static Point2f POINT_MATERIAL = {.x = 1.5, .y = 0.175}; //0.175};
const static Point2f POINT_RIGHT_MID = {.x = 1.1, .y = 0.175};
const static Pose2f POSE_TEMP_STORAGE = {.xy = {.x = 1.1, .y = 1.95}, .angle = 180};
const static Point2f POINT_LEFT_UP = {.x = 1.98, .y = 1.95};
const static Pose2f POSE_PROCESS = {.xy = {.x = 1.98, .y = 1.13}, .angle = 90};
const static Point2f POINT_RIGHT_UP = {.x = 1.98, .y = 0.175};
const static Point2f POINT_MATERIAL2 = {.x = 1.585, .y = 0.175};
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
	chassis_arrived();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
}

void to_right_middle(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_RIGHT_MID.x, POINT_RIGHT_MID.y);
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
	chassis_rotate_abs(LINE_ACC,LINE_SPEED,0);
}

/**
 * @brief 前往原料区
 */
void To_Materia(void)
{
	chassis_to(LINE_ACC, LINE_SPEED, POINT_MATERIAL);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
}

/**
 * @brief 前往物料处理区
 */
void to_process(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POSE_TEMP_STORAGE.x, POSE_TEMP_STORAGE.y);
	chassis_to(LINE_ACC, LINE_SPEED, POSE_TEMP_STORAGE.xy);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
	vision_adjust_chassis(0, &POSE_TEMP_STORAGE);
}

/**
 * @brief 前往储存区
 */
void To_store(void)
{
	chassis_to(LINE_ACC, LINE_SPEED, POSE_PROCESS.xy);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
	vision_adjust_chassis(1, &POSE_PROCESS);
}

void To_stop(void)
{
	chassis_to(LINE_ACC, LINE_SPEED, POINT_ZERO);
}

void To_Materia2(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_MATERIAL2.x, POINT_MATERIAL2.y);
	chassis_to(LINE_ACC, LINE_SPEED, POINT_MATERIAL2);
    Point2f new_dst = coordinate_transform_Z_rotate(POINT_MATERIAL, -90);
	CHASSIS.pos = new_dst;
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
}

void material_task(uint8_t round)
{
	Arm_Action(Dtct, Ordinary2);
	// 翅膀打开
	Arm_Action(Store_Open, Ordinary2);
	arm_move_sync();
	
	// 看颜色是否与期望顺序一致
	for (int i = 0; i < 3; i++)
	{
		color_t color = vision_info.order[i + round * ITEM_COUNT];
		vision_subscribe_item(color);
		vision_sync(ps_comm_type_ITEM_DETECT);
		vision_subscribe(ps_comm_type_IDLE_REQ);
		ArmTarget target = COLOR_ARM_TARGET_MAP[color];
		Arm_Action(Plate_To_Store, target);
	}
    vision_item_detect_stop(); // 通知上位机检测结束

	// 翅膀关闭
	Arm_Action(Store_Close, Ordinary2);
	Arm_Scan();
}

void process_place_task(uint8_t position)
{
	uint8_t color_pos;
	if (position == 1)
	{
		color_pos = 0;
	} else if (position == 2)
	{
		color_pos = 1;
	}

	// 翅膀打开,调整位置
	Arm_Action(Dtct, Ordinary2);
	Arm_Action(Store_Scan, Ordinary2);
	HAL_Delay(300);
	vision_subscribe_rings();
	Arm_Action(Store_Open, Ordinary2);
	Arm_X_Zip();
	// 取物块，放物块，拿物块
	for (int i = 0; i < 3; i++)
	{
		color_t color = vision_info.order[i + color_pos * ITEM_COUNT];
		ArmTarget target = COLOR_ARM_TARGET_MAP[color];
		Arm_Action(Store_To_Ground, target);
	}
	for (int i = 0; i < 3; i++)
	{
		color_t color = vision_info.order[i + color_pos * ITEM_COUNT];
		ArmTarget target = COLOR_ARM_TARGET_MAP[color];
		Arm_Action(Ground_To_Store, target);
	}


    Arm_Z_Middle();								  // Z抬高
    HAL_Delay(150);
    Arm_X_Zip();
	stepmotor_rotate(&pARM_DEFINE->motor_r, 0);

	// 翅膀关闭, 机械臂关闭
	Arm_Action(Store_Close, Ordinary2);
}


void storage_place_task(uint8_t position)
{
	// 翅膀打开,调整位置
	Arm_Action(Dtct, Ordinary2);
	Arm_Action(Store_Scan, Ordinary2);
	Arm_Action(Store_Open, Ordinary2);
	vision_subscribe_rings();
	Arm_X_Zip();

	// 取物块，放物块，拿物块
	if(position==0)
	{
		for (int i = 0; i < 3; i++)
		{
			color_t color = vision_info.order[i + position * ITEM_COUNT];
			ArmTarget target = COLOR_ARM_TARGET_MAP[color];
			Arm_Action(Store_To_Ground, target);
		}
	}
	else if(position==1)
	{
		// 取物块，放物块(码垛)
		for (int i = 0; i < 3; i++)
		{
			ArmTarget target = COLOR_ARM_TARGET_MAP[vision_info.order[i + position * ITEM_COUNT]];
			Arm_Action(Maduo, target);
		}
	}
	Arm_Action(Store_Close, Ordinary2);
	stepmotor_rotate(&pARM_DEFINE->motor_r, 0);
}

void Taskpoint(void)
{
	///************第一圈**********************/
	To_start();
	
	To_Scan();
	
	vision_subscribe(ps_comm_type_SCAN_QR_REQ);
	vision_sync(ps_comm_type_SCAN_QR);
	vision_subscribe(ps_comm_type_IDLE_REQ);

	To_Materia();
	
	//执行 圆盘——》储存
	material_task(0);

	to_right_middle();

	to_process();
	
	//执行 储存——》圆环——》储存
    process_place_task(1);

	To_Leftup();

	To_store();
	
	//执行 存储——》圆环
	storage_place_task(0);
	
	to_right_up();
	
	///************第二圈**********************/

	//执行 圆盘——》存储
	To_Materia2();
	material_task(1);

	to_right_middle();

	to_process();
	
	process_place_task(2);

	To_Leftup();

	To_store();
	
	//执行 码垛
	storage_place_task(1);
	
	to_right_up();
	
	To_start();

	To_stop();
}

void task_vision_update_loop()
{
	while (1)
	{
		HWT101_update();
	}
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
