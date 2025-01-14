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
///************宏定义**********************/

const static uint16_t LINE_SPEED = 1500;
const static uint8_t LINE_ACC = 0;

#define targetpositionx 0
#define targetpositiony 0
#define __min(x, y) x < y ? x : y
#define __max(x, y) x > y ? x : y
///************物流搬运初赛场地上的点位**********************/
const static Point2f POINT_ZERO = {.x = 0, .y = 0};
const static Point2f POINT_START = {.x = 0.175, .y = 0.175};
const static Point2f POINT_MATERIAL = {.x = 1.5, .y = 0.175};
const static Point2f POINT_RIGHT_MID = {.x = 1.1, .y = 0.175};
const static Point2f POINT_TEMP_STORAGE = {.x = 1.1, .y = 1.95};
const static Point2f POINT_LEFT_UP = {.x = 1.95, .y = 1.95};
const static Point2f POINT_PROCESS = {.x = 1.95, .y = 1.13};
const static Point2f POINT_RIGHT_UP = {.x = 1.95, .y = 0.175};
const static Point2f POINT_MATERIAL2 = {.x = 1.505, .y = 0.175};
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
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_START);
}

void To_Rightmiddle(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_RIGHT_MID.x, POINT_RIGHT_MID.y);
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_RIGHT_MID);
}
void To_Leftup(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_LEFT_UP.x, POINT_LEFT_UP.y);
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_LEFT_UP);
}
void To_Ringtup(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_RIGHT_UP.x, POINT_RIGHT_UP.y);
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_RIGHT_UP);
}

void To_Materia(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_MATERIAL.x, POINT_MATERIAL.y);
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_MATERIAL);
}

void To_TPstore(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_TEMP_STORAGE.x, POINT_TEMP_STORAGE.y);
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_TEMP_STORAGE);
}

void To_store(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_PROCESS.x, POINT_PROCESS.y);
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_PROCESS);
}

void To_stop(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_ZERO);
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_ZERO);
}

void To_Materia2(void)
{
	// Chassis_To(&Chassis, LINE_ACC, LINE_SPEED, POINT_MATERIAL2.x, POINT_MATERIAL2.y);
	chassis_move_abs(LINE_ACC, LINE_SPEED, POINT_MATERIAL2);
}


///**
// * @description: 扫码任务：车辆运行至扫码点，向上位机发送扫码指令
// * @return {*}
// */
void Task_Scan(void)
{
	vision_subscribe(ps_comm_type_scan_qr);
//	vision_update();
}

///**
// * @description: 颜色任务：车辆运行至原料区，向上位机发送颜色识别指令
// * @return {*}
// */

void Task_colorget(void)
{
	vision_subscribe(ps_comm_type_color_detect);
//	vision_update();
}
///**
// * @description: 色环位置任务：车辆运行至暂存区或者存储区，向上位机发送色环位置识别指令
// * @return {*}
// */
void Task_positionget(void)
{
	vision_subscribe(ps_comm_type_ring_pos_detect);
//	vision_update();
}


///**
// * @description: 圆环角度矫正任务：车辆运行至暂存区或者存储区，向上位机发送色环角度矫正
// * @return {*}
// */
void Task_Ringangleget(void)
{
	vision_subscribe(ps_comm_type_ring_angle_detect);
//	vision_update();
}

// 视觉矫正位置任务

void Chassis_visionadjustposition(uint8_t position)
{
	Task_positionget();


	while (fabs(vision_info.ring_pos.x) > 0.001 || fabs(vision_info.ring_pos.y) > 0.001)
	{
		point_move.x = vision_info.ring_pos.x * 0.001;
		point_move.y = vision_info.ring_pos.y * 0.001;
		chassis_move(10, 10, point_move);

	}
	// 物料区
	if (position == 0)
	{
		pose_calibrate_XY(POINT_MATERIAL);
		// Chassis.Location.XDistancecurrent = CoordinatesTansformation(materiapoint.x, materiapoint.y, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 0);
		// Chassis.Location.YDistancecurrent = CoordinatesTansformation(materiapoint.x, materiapoint.y, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 1);
	}
	// 暂存区
	if (position == 1)
	{
		Point2f tmp = {1.091, 1.962};
		pose_calibrate_XY(tmp);
		// Chassis.Location.XDistancecurrent = CoordinatesTansformation(1.091, 1.962, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 0);
		// Chassis.Location.YDistancecurrent = CoordinatesTansformation(1.091, 1.962, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 1);
	}
	// 粗加工
	if (position == 2)
	{
		Point2f tmp = {1.995, 1.122};
		pose_calibrate_XY(tmp);
		// Chassis.Location.XDistancecurrent = CoordinatesTansformation(1.995, 1.122, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 0);
		// Chassis.Location.YDistancecurrent = CoordinatesTansformation(1.995, 1.122, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 1);
	}
}

// 视觉矫正角度任务

void Chassis_visionadjustangle()
{
	static const float anglestep=0.5;
	vision_subscribe(ps_comm_type_ring_angle_detect);

	while(fabs(vision_info.ring_angle)>0.1)
	{
		if(vision_info.ring_angle>0)
			chassis_rotate_abs(LINE_ACC, LINE_SPEED, HWT101Data.YawZ+anglestep);
		else
			chassis_rotate_abs(LINE_ACC, LINE_SPEED, HWT101Data.YawZ-anglestep);
	}

}

void Chassis_visionadjustsum(uint8_t position)
{
	vision_subscribe(ps_comm_type_ring_angle_detect);
	
	// 获得误差值
	// decode_vision(&vision_info, &pose_error);
	while(fabs(vision_info.ring_angle)>0.1||fabs(pose_error.xy.x) > 0.001 || fabs(pose_error.xy.y) > 0.001)
	{
		PID_Caclulate(&PID_LOCATION_JUST_X, 0, -(float)vision_info.ring_pos.x);
		PID_Caclulate(&PID_LOCATION_JUST_Y, 0, -(float)vision_info.ring_pos.y);	
		PID_Caclulate(&ANGLE_PID2,0, -vision_info.ring_angle);
		Pose2f speed;
		speed.xy.x = 0.5*PID_LOCATION_JUST_X.OUT;
		speed.xy.x = 0.5*PID_LOCATION_JUST_Y.OUT;
		speed.angle = ANGLE_PID2.OUT;
		chassis_move_speed(LINE_ACC, &speed);

		// decode_vision(&vision_info, &pose_error);
	}
	
	emm42_halt(chassis_uart_handle, 0, 0);
	// 物料区
	if (position == 0)
	{
		pose_calibrate_XY(POINT_MATERIAL);
		// Chassis.Location.XDistancecurrent = CoordinatesTansformation(materiapoint.x, materiapoint.y, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 0);
		// Chassis.Location.YDistancecurrent = CoordinatesTansformation(materiapoint.x, materiapoint.y, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 1);
	}
	// 暂存区
	if (position == 1)
	{
		Point2f tmp = {1.121, 2};
		pose_calibrate_XY(tmp);
		// Chassis.Location.XDistancecurrent = CoordinatesTansformation(1.091, 1.962, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 0);
		// Chassis.Location.YDistancecurrent = CoordinatesTansformation(1.091, 1.962, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 1);
	}
	// 粗加工
	if (position == 2)
	{
		Point2f tmp = {1.98, 1.117};
		pose_calibrate_XY(tmp);
		// Chassis.Location.XDistancecurrent = CoordinatesTansformation(1.995, 1.122, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 0);
		// Chassis.Location.YDistancecurrent = CoordinatesTansformation(1.995, 1.122, chassis->coord.Angle.AngleTarget + chassis->coord.Angle.Angle_between_Word_Chassis, 1);
	}
	

}




void Chassis_materiatask(uint8_t position)
{
	Arm_Action(Dtct, Ordinary2);
	// 翅膀打开
	Arm_Action(Store_Open, Ordinary2);
	Task_colorget();
	// 看颜色是否与期望顺序一致
	for (int i = 0; i < 3; i++)
	{
		while (vision_info.item_color != vision_info.order[i + position * ITEM_COUNT])
		{
			Task_colorget();
		}

		// 是期望颜色
		// 抓取到对应的存储区
		ArmTarget target = COLOR_ARM_TARGET_MAP[vision_info.order[i + position * ITEM_COUNT]];
		Arm_Action(Plate_To_Store, target);
	}
	// 翅膀关闭
	Arm_Action(Store_Close, Ordinary2);
}

void Chassis_TPplacetask(uint8_t position)
{

	// 翅膀打开,调整位置
	Arm_Action(Dtct,0);
	Arm_Action(Store_Open,0);
	Chassis_visionadjustposition(1);
	Chassis_visionadjustangle();	

	// 取物块，放物块，拿物块
	for (int i = 0; i < 3; i++)
	{

		ArmTarget target = COLOR_ARM_TARGET_MAP[vision_info.order[i + position * ITEM_COUNT]];
		Arm_Action(Store_To_Ground, target);
	}

	for (int i = 0; i < 3; i++)
	{
		ArmTarget target = COLOR_ARM_TARGET_MAP[vision_info.order[i + position * ITEM_COUNT]];
		Arm_Action(Ground_To_Store, target);
	}

	// 翅膀关闭
	Arm_Action(Store_Close, Ordinary2);
	Arm_Action(Scan,0);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);


}

void Chassis_storeplacetask(uint8_t position)
{
	
	// 翅膀打开,调整位置
	Arm_Action(Dtct,0);
	Arm_Action(Store_Open,0);
	Chassis_visionadjustposition(2);
	Chassis_visionadjustangle();
	if(position==0)
	{
			// 取物块，放物块
		for (int i = 0; i < 3; i++)
		{
			ArmTarget target = COLOR_ARM_TARGET_MAP[vision_info.order[i + position * ITEM_COUNT]];
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

	// 翅膀关闭
	Arm_Action(Store_Close, Ordinary2);
	Arm_Action(Scan,0);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
}



///**
// * @description: 车辆运行逻辑
// * @return {*}
// */
void Task(void)
{

	///************第一圈**********************/
	To_start();
	Arm_Action(Store_Close,0);
	HAL_Delay(2000);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	vision_info.order[0]=1;
	vision_info.order[1]=2;
	vision_info.order[2]=3;
	vision_info.order[3]=3;
	vision_info.order[4]=2;
	vision_info.order[5]=1;
	
	To_Materia();
	HAL_Delay(4000);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	To_Rightmiddle();
	HAL_Delay(3000);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_TPstore();
	HAL_Delay(5000);
	
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
	
	Chassis_TPplacetask(0);

	To_TPstore();
	HAL_Delay(1500);
	

	
	To_Leftup();
	HAL_Delay(3000);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_store();
	HAL_Delay(4000);
	
	
	Chassis_storeplacetask(0);

	To_store();
	HAL_Delay(1000);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_Ringtup();
	HAL_Delay(3000);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
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
	
	To_start();
	HAL_Delay(5000);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);

	To_stop();
	HAL_Delay(1000);
}

void Taskpoint(void)
{
	///************第一圈**********************/
	To_start();
	
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	To_Materia();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	
	To_Rightmiddle();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);


	To_TPstore();
	
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
//	Chassis_TPplacetask(0);

	Chassis_visionadjustsum(1);
	To_TPstore();
	
	To_Leftup();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_store();
	Chassis_visionadjustsum(2);

	To_store();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_Ringtup();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
	///************第二圈**********************/

	To_Materia2();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);

	To_Rightmiddle();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_TPstore();
	
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 180);

	Chassis_visionadjustsum(1);
	To_TPstore();
	

	To_Leftup();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_store();
	Chassis_visionadjustsum(2);
	To_store();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 90);

	To_Ringtup();
	chassis_rotate_abs(LINE_ACC, LINE_SPEED, 0);
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
	if (flag == 1)
		Task_colorget();
	if (flag == 2)
		Task_Scan();
	if (flag == 3)
		Task_positionget();
	if (flag == 4)
		Chassis_visionadjustangle();
	if (flag == 5)
		Chassis_visionadjustposition(0);
	if (flag == 6)
	{
		Pose2f pose;
		pose.angle = 0;
		pose.xy.x = 0;
		pose.xy.y = 0;
		vision_adjust_chassis(&pose);
	}
}
