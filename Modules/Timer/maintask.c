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
const static Point2f POINT_MATERIAL2 = {.x = 1.51, .y = 0.175};
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

///**
// * @description: 运行到点位
// * @return {*}
// */
void To_start(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, start.x, start.y);
	chassis_to(LINE_ACC, LINE_SPEED, POINT_START);
}

void To_Rightmiddle(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_RIGHT_MID.x, POINT_RIGHT_MID.y);
	chassis_to(LINE_ACC, LINE_SPEED, POINT_RIGHT_MID);
}
void To_Leftup(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_LEFT_UP.x, POINT_LEFT_UP.y);
	chassis_to(LINE_ACC, LINE_SPEED, POINT_LEFT_UP);
}
void To_Ringtup(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_RIGHT_UP.x, POINT_RIGHT_UP.y);
	chassis_to(LINE_ACC, LINE_SPEED, POINT_RIGHT_UP);
}

void To_Scan(void)
{
	chassis_to(LINE_ACC,LINE_SPEED,POINT_SCANQR);
}

void To_Materia(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_MATERIAL.x, POINT_MATERIAL.y);
	chassis_to(LINE_ACC, LINE_SPEED, POINT_MATERIAL);
}

void To_TPstore(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POSE_TEMP_STORAGE.x, POSE_TEMP_STORAGE.y);
	chassis_to(LINE_ACC, LINE_SPEED, POSE_TEMP_STORAGE.xy);
}

void To_store(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POSE_PROCESS.x, POSE_PROCESS.y);
	chassis_to(LINE_ACC, LINE_SPEED, POSE_PROCESS.xy);
}

void To_stop(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_ZERO);
	chassis_to(LINE_ACC, LINE_SPEED, POINT_ZERO);
}

void To_Materia2(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_MATERIAL2.x, POINT_MATERIAL2.y);
	chassis_to(LINE_ACC, LINE_SPEED, POINT_MATERIAL2);
}

void Chassis_materiatask(uint8_t round)
{
	Arm_Action(Dtct, Ordinary2);
	// 翅膀打开
	Arm_Action(Store_Open, Ordinary2);

	// 看颜色是否与期望顺序一致
	for (int i = 0; i < 3; i++)
	{
		color_t color = vision_info.order[i + round * ITEM_COUNT];
		vision_subscribe_item(color);
		vision_sync(ps_comm_type_ITEM_DETECT);
		ArmTarget target = COLOR_ARM_TARGET_MAP[color];
		Arm_Action(Plate_To_Store, target);
	}
	// 翅膀关闭
	Arm_Action(Store_Close, Ordinary2);
	
	Arm_Scan();
    

}

void Chassis_TPplacetask(uint8_t position)
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
	Arm_Action(Store_Open, Ordinary2);
	vision_subscribe_rings();
	Arm_X_Zip();
	// 取物块，放物块，拿物块
	for (int i = 0; i < 3; i++)
	{
		color_t color = vision_info.order[i + color_pos * ITEM_COUNT];
		ArmTarget target = COLOR_ARM_TARGET_MAP[color];
		Arm_Action(Store_To_Ground, target);
	}
		HAL_Delay(300);
	for (int i = 0; i < 3; i++)
	{
		color_t color = vision_info.order[i + color_pos * ITEM_COUNT];
		ArmTarget target = COLOR_ARM_TARGET_MAP[color];
		Arm_Action(Ground_To_Store, target);
	}

	// 翅膀关闭
	Arm_Action(Store_Close, Ordinary2);

}


void Chassis_storeplacetask(uint8_t position)
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
}



///**
// * @description: 车辆运行逻辑
// * @return {*}
// */
// void Task(void)
// {

// 	///************第一圈**********************/
// 	To_start();
// 	Arm_Action(Store_Close,0);
// 	HAL_Delay(2000);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
// 	vision_info.order[0]=1;
// 	vision_info.order[1]=2;
// 	vision_info.order[2]=3;
// 	vision_info.order[3]=3;
// 	vision_info.order[4]=2;
// 	vision_info.order[5]=1;
	
// 	To_Materia();
// 	HAL_Delay(4000);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
// 	To_Rightmiddle();
// 	HAL_Delay(3000);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

// 	To_TPstore();
// 	HAL_Delay(5000);
	
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
	
// 	Chassis_TPplacetask(0);

// 	To_TPstore();
// 	HAL_Delay(1500);
	

// 	To_Leftup();
// 	HAL_Delay(3000);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

// 	To_store();
// 	HAL_Delay(4000);
	
	
// 	Chassis_storeplacetask(0);

// 	To_store();
// 	HAL_Delay(1000);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

// 	To_Ringtup();
// 	HAL_Delay(3000);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	///************第二圈**********************/

//	To_Materia2();
//	HAL_Delay(2000);
//	Chassis.Location.XDistancecurrent=CoordinatesTansformation(materiapoint.x,materiapoint.y,Chassis.coord.Angle.AngleTarget + Chassis.coord.Angle.Angle_between_Word_Chassis,0);
//	Chassis.Location.YDistancecurrent=CoordinatesTansformation(materiapoint.x,materiapoint.y,Chassis.coord.Angle.AngleTarget + Chassis.coord.Angle.Angle_between_Word_Chassis,1);
//	Chassis_ChangeAngle2(&Chassis, 0);

//	To_Rightmiddle();
//	HAL_Delay(3000);
//	Chassis_ChangeAngle(&Chassis, 90);
//	Chassis_ChangeAngle2(&Chassis, 90);

//	To_TPstore();
//	HAL_Delay(5000);
//	
//	Chassis_ChangeAngle(&Chassis, 180);
//	Chassis_ChangeAngle2(&Chassis, 180);

//	Chassis_TPplacetask(1);
//	To_TPstore();
//	HAL_Delay(1000);
//	

//	To_Leftup();
//	HAL_Delay(3000);
//	Chassis_ChangeAngle(&Chassis, 90);
//	Chassis_ChangeAngle2(&Chassis, 90);

//	To_store();
//	HAL_Delay(3000);
//	Chassis_storeplacetask(1);
//	To_store();
//	HAL_Delay(1500);
//	Chassis_ChangeAngle2(&Chassis, 90);

//	To_Ringtup();
//	HAL_Delay(3000);
//	Chassis_ChangeAngle(&Chassis, 0);
//	Chassis_ChangeAngle2(&Chassis, 0);
	
// 	To_start();
// 	HAL_Delay(5000);
// 	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);

// 	To_stop();
// 	HAL_Delay(1000);
// }

void Taskpoint(void)
{
	///************第一圈**********************/
	To_start();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	To_Scan();
	chassis_rotate_abs(LINE_ACC,LINE_SPEED,0);
	
	vision_subscribe(ps_comm_type_SCAN_QR_REQ);
	vision_sync(ps_comm_type_SCAN_QR);
	vision_subscribe(ps_comm_type_IDLE_REQ);

	To_Materia();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	//执行 圆盘——》储存
	Chassis_materiatask(0);

	
	To_Rightmiddle();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_TPstore();
	
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
	
	vision_adjust_chassis(0, &POSE_TEMP_STORAGE);
	//执行 储存——》圆环——》储存
    Chassis_TPplacetask(1);

	vision_adjust_chassis(0, &POSE_TEMP_STORAGE);
	Arm_Scan();

	To_Leftup();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);


	To_store();
	vision_adjust_chassis(1, &POSE_PROCESS);
	
	
	//执行 存储——》圆环
	Chassis_storeplacetask(0);
	
	vision_adjust_chassis(1, &POSE_PROCESS);
	Arm_Scan();

	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_Ringtup();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	///************第二圈**********************/

	To_Materia2();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	//执行 圆盘——》存储
	Chassis_materiatask(1);

	To_Rightmiddle();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_TPstore();
	
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);

	vision_adjust_chassis(0, &POSE_TEMP_STORAGE);
	
	Chassis_TPplacetask(2);
	vision_adjust_chassis(0, &POSE_TEMP_STORAGE);
	Arm_Scan();

	To_Leftup();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_store();
	
	
	//执行 码垛
	Chassis_storeplacetask(1);
	
	Arm_Scan();
	To_Ringtup();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	To_start();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);

	To_stop();
}

void task_vision_update_loop()
{
	while (1)
	{
		// vision_update();
		HWT101_update();
	}
}

void my_debug(uint8_t flag)
{
	if (flag == 5)
		vision_adjust_chassis(0, &POSE_TEMP_STORAGE);
	if (flag == 6)
	{
		Chassis_TPplacetask(1);
	}
	if (flag == 7)
	{
		Chassis_materiatask(0);
	}
	if (flag == 8)
	{
		Chassis_storeplacetask(1);
	}
}
