#include "Arm.h"
#include "servor_ctl.h"

#define MotorSpeed 1500
#define WaitFlag 1

/*----------------------步进电机参数部分----------------------*/

// 减速比好像是1:3
// 存储
int16_t StepMotor_R_StorePosition[6] = {
	0,		  //0 一般状态
	1425, //1 红
	1425, //2 绿
	7575, //3 蓝
	4500, //4 物料识别位置（转180）
	9000  //5 限位
};
// 摆放
int16_t StepMotor_R_PlacePosition[6] = {
	0,		  // 0一般位置
	3074, // 1红
	4500, // 2绿
	5520, // 3蓝
	4500, // 4物料识别位置（转180）
	9000  // 限位
};

// 3200步为一圈
// 9000到底儿
int16_t StepMotor_Z_Position[7] = {
	500,		 //0 一般状态(最高点)
	5800,	 //1 从圆盘上取
	12000,	 //2 从地面上取
	12000,	 //3 放置到地面上
	6500,	 //4 堆放
	3700,	 //5 放到储物机构中高度
	3200 * 3 //6 限位——盲猜转四圈
};

int16_t StepMotor_Forward_Store[6]={
	200,	// 一般状态——d=0mm
	4600, // 红
	200,	// 绿
	200, // 蓝
	1600, // 取圆盘上物块的位置
	9600	// 完全伸出——d=115mm
};

int16_t StepMotor_Forward_Place[6]={
	200,		// 一般状态
	3200, // 红
	1600,	// 绿
	3200, // 蓝
};



/*---------------步进电机控制部分---------------------*/
extern uint8_t UART8_RxData[4];

void Arm_Rx(StepMotorStruct *motor)
{
	if(UART8_RxData[0]!=0)
	{
			motor->Step_Finish_Flag=1;
			motor->Step_Running_Flag=0;
	}
}

//绝对位置控制
// 0--地址
// 1--0xFD
// 2--方向
// 3--速度高八位
// 4--速度低八位
// 5--加速度
// 6--脉冲
// 7--脉冲
// 8--脉冲
// 9--脉冲
// 10--相对/绝对--1绝对，0相对
// 11--多机同步标志
// 12--校验字节
uint8_t StepMotor_ToStepPacket[13] = {00, 0xFD, 00, 00, 00, 0x0A, 00, 00, 00, 00, 01, 00, 0x6B};
void StepMotor_ToStepData(StepMotorStruct *motor,int32_t step,uint16_t speed,uint8_t flag)
{
	 motor->Steps_ToDir = step;
     motor->Steps_Tar = step-motor->Steps_Sum;
	 motor->Angle = (double)(motor->Steps_Sum * 360.0 / (3200.0 * motor->Ratio));
     motor->Speed_Tar=speed;
     motor->Slow_Flag=flag;
 }
void StepMotor_ToAngleData(StepMotorStruct *motor,int16_t angle,uint16_t speed,uint8_t flag)
{
	motor->Angle=angle;
	double stepDelta = (double)angle * (3200.0 * motor->Ratio) / 360.0;
	motor->Steps_Tar=(int32_t)stepDelta;
	motor->Speed_Tar=speed;
    motor->Slow_Flag=flag;
}

void StepMotor_ToStepControl(StepMotorStruct *motor)
    
{
    if(motor->ToDistance_Flag==1)//距离控制
    {
        motor->ToStep_Flag=0;
        motor->ToAngle_Flag=0;
        motor->Steps_ToDir=motor->Distance / 32.0 * (3200.0 * motor->Ratio);//更新脉冲参数
        motor->Angle = (double)(motor->Steps_ToDir * 360.0 / (3200.0 * motor->Ratio));//更新角度参数
    }
    if(motor->ToAngle_Flag==1)//角度控制
    {
        motor->ToStep_Flag=0;
        motor->ToDistance_Flag=0;
        motor->Steps_ToDir = motor->Angle * (3200.0 * motor->Ratio) / 360.0;//更新脉冲参数
        motor->Distance = (double)(motor->Steps_ToDir / (3200.0 * motor->Ratio) * 3.2); //更新距离参数
    }
    
    motor->Steps_Sum+=motor->Steps_Tar;
    
    if(motor->ToStep_Flag==1)//脉冲控制
    {
        motor->ToAngle_Flag=0;
        motor->ToDistance_Flag=0;
        motor->Angle = (double)(motor->Steps_ToDir * 360.0 / (3200.0 * motor->Ratio));//更新角度参数
        motor->Distance = (double)(motor->Steps_ToDir / (3200.0 * motor->Ratio) * 3.2); //更新距离参数
    }
    //更改方向
     if(motor->Motor_Direction)     StepMotor_ToStepPacket[2]=01;
     else     StepMotor_ToStepPacket[2]=00;
     if(motor->Steps_ToDir<0)
     {
        StepMotor_ToStepPacket[2]=00;
         motor->Steps_ToDir=-motor->Steps_ToDir;
     }
     else
        StepMotor_ToStepPacket[2]=01;     
     StepMotor_ToStepPacket[0]=motor->ID;
     StepMotor_ToStepPacket[3]=(uint8_t)((uint16_t)motor->Speed_Tar >> 8);//高八位
     StepMotor_ToStepPacket[4]=(uint8_t)(motor->Speed_Tar);
     StepMotor_ToStepPacket[5]=(uint8_t)(motor->acceleration);
     StepMotor_ToStepPacket[6]=(uint8_t)(motor->Steps_ToDir >> 24);
     StepMotor_ToStepPacket[7]=(uint8_t)(motor->Steps_ToDir >> 16);
     StepMotor_ToStepPacket[8]=(uint8_t)(motor->Steps_ToDir >> 8);
     StepMotor_ToStepPacket[9]=(uint8_t)(motor->Steps_ToDir);
     StepMotor_ToStepPacket[10]=01;
     StepMotor_ToStepPacket[11]=0;
     StepMotor_ToStepPacket[12]=0x6B;
     HAL_UART_Receive_IT(&huart8,UART8_RxData, 4);
     HAL_UART_Transmit(motor->huart, StepMotor_ToStepPacket, 13,1000);
    motor->Step_Running_Flag=1;
    motor->Step_Finish_Flag=0;
}

void Forward_Store_Control(ArmTarget color)
{
	StepMotor_ToStepData(&Motor_X,StepMotor_Forward_Store[color],MotorSpeed,1);
	StepMotor_ToStepControl(&Motor_X);
}
void Forward_Place_Control(ArmTarget color)
{
	StepMotor_ToStepData(&Motor_X,StepMotor_Forward_Place[color],MotorSpeed,1);
	StepMotor_ToStepControl(&Motor_X);
}

//立即停止
// 0--地址
// 1--FE
// 2--0x98
// 3--多机同步
// 4--6B
 uint8_t StepMotor_StopPacket[5]={0,0xFE,0x98,0,0x6B};
 void StepMotor_Stop(StepMotorStruct *motor)
 {
	StepMotor_StopPacket[0]=motor->ID;
	HAL_UART_Transmit(motor->huart, StepMotor_StopPacket, 5,1000);
 }
 
//读取电机状态
// 0--地址
// 1--0x3A
// 2--0x6B
// 到位标志位：0x03&0x02 =0x01=true
uint8_t StepMotor_StatePacket[3]={0,0x3A,0x6B};
void StepMotor_GetState(StepMotorStruct *motor)
{
	StepMotor_StatePacket[0]=motor->ID;
	HAL_UART_Transmit(motor->huart, StepMotor_StatePacket, 3,1000);
}

//步进电机时间计算
int16_t StepMotor_Time(StepMotorStruct *motor)
{
	int16_t timeSec = (motor->Steps_Tar/(3200*(motor->Speed_Tar/60))*1000);
	return timeSec;
}

int32_t Motor_TimeCNT[2];
void StepMotor_Wait(StepMotorStruct *motor,uint8_t flag)
{
	if(flag==1)
	{
		Motor_TimeCNT[0]=Motor_TimeCNT[1];
		while(motor->Step_Finish_Flag==0)
		{
			if(UART8_RxData[0]==1)
			{
				Motor_R.Step_Finish_Flag=1;
			}
			if(UART8_RxData[0]==2)
			{
				Motor_Z.Step_Finish_Flag=1;
			}
			if(UART8_RxData[0]==3)
			{
				Motor_X.Step_Finish_Flag=1;
			}
//			if(fabs(Motor_TimeCNT[0]-Motor_TimeCNT[1])>=5000)
//			{
//				memset(UART8_RxData, 0, sizeof(UART8_RxData));
//				HAL_UART_Receive_IT(&huart8,UART8_RxData, 4);
//				break;
//			}
		}
		motor->Step_Running_Flag=0;
	}
}

/*
机械臂扫码位置
Claw——张开
Forward——0
R——0
Z——最高处
*/
void Arm_Scan(void)
{
	servor_ctl(servor_object_CLAW, 1);
	Forward_Place_Control(0);
	StepMotor_Wait(&Motor_X,WaitFlag);
	//HAL_Delay(500);
	StepMotor_ToStepData(&Motor_Z, StepMotor_Z_Position[0], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_Z);
	StepMotor_Wait(&Motor_Z,WaitFlag);
	//HAL_Delay(StepMotor_Time(&Motor_Z));
	
	StepMotor_ToStepData(&Motor_R, StepMotor_R_PlacePosition[0], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_R);
	StepMotor_Wait(&Motor_R,WaitFlag);
	//HAL_Delay(500);
}

/*
机械臂物料识别位置
Claw——张开
Forward——0
R——180
Z——最高
*/
void Arm_Dtct(void)
{
	servor_ctl(servor_object_CLAW, 1);
	Forward_Place_Control(0);
	StepMotor_Wait(&Motor_X,WaitFlag);
	//HAL_Delay(500);
	
	StepMotor_ToStepData(&Motor_Z, StepMotor_Z_Position[0], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_Z);
	StepMotor_Wait(&Motor_Z,WaitFlag);
	//HAL_Delay(500);
	StepMotor_ToStepData(&Motor_R, StepMotor_R_PlacePosition[4], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_R);
	StepMotor_Wait(&Motor_R,WaitFlag);
	//HAL_Delay(500);
}

/*
机械臂从圆盘抓取物料位置
R——180
Z——从圆盘上取
*/
void Arm_PlateCapture(void)
{
	Forward_Place_Control(Dtct2);
	StepMotor_Wait(&Motor_X,WaitFlag);
	
	StepMotor_ToStepData(&Motor_R, StepMotor_R_PlacePosition[4], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_R);
	StepMotor_Wait(&Motor_R,WaitFlag);
	
	StepMotor_ToStepData(&Motor_Z, StepMotor_Z_Position[1], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_Z);
	StepMotor_Wait(&Motor_Z,WaitFlag);
}

/*
机械臂将物料存放到对应存储机构
*/
void Arm_Store(ArmTarget color)
{
	Forward_Store_Control(color);
	StepMotor_Wait(&Motor_X,WaitFlag);
	
	StepMotor_ToStepData(&Motor_R, StepMotor_R_StorePosition[color], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_R);
	StepMotor_Wait(&Motor_R,WaitFlag);
	
	StepMotor_ToStepData(&Motor_Z, StepMotor_Z_Position[5], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_Z);
	StepMotor_Wait(&Motor_Z,WaitFlag);

}

/*
机械臂摆放物料到圆环里
*/
void Arm_GroundPlace(ArmTarget color)
{
	Forward_Place_Control(color);
	StepMotor_Wait(&Motor_X,WaitFlag);
	
	StepMotor_ToStepData(&Motor_R, StepMotor_R_PlacePosition[color], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_R);
	StepMotor_Wait(&Motor_R,WaitFlag);
	
	StepMotor_ToStepData(&Motor_Z, StepMotor_Z_Position[3], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_Z);
	StepMotor_Wait(&Motor_Z,WaitFlag);
}

/*
机械臂码垛
*/
void Arm_Maduo(ArmTarget color)
{
	Forward_Place_Control(color);
	StepMotor_Wait(&Motor_X,WaitFlag);
	
	StepMotor_ToStepData(&Motor_R, StepMotor_R_PlacePosition[color], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_R);
	StepMotor_Wait(&Motor_R,WaitFlag);
	
	StepMotor_ToStepData(&Motor_Z, StepMotor_Z_Position[color], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_Z);
	StepMotor_Wait(&Motor_Z,WaitFlag);
}

/*
Z轴归中
*/
void Arm_Z_Middle(void)
{
	StepMotor_ToStepData(&Motor_Z, StepMotor_Z_Position[0], MotorSpeed, 1);
	StepMotor_ToStepControl(&Motor_Z);
	StepMotor_Wait(&Motor_Z,WaitFlag);
}

/*--------------------------运行控制--------------------------------------*/

/*
机械臂从圆盘抓取物料并存放
*/
void Arm_Action_GetPlate(ArmTarget color)
{
	servor_ctl(servor_object_CLAW, 1);	 // 张抓
	Arm_PlateCapture();	 // 转到抓取位置
	servor_ctl(servor_object_CLAW, 2); // 闭
	HAL_Delay(300);
	Arm_Z_Middle();	  // Z抬高
	Arm_Store(color); // 摆放到存储机构
	servor_ctl(servor_object_CLAW, 1); // 张抓
	HAL_Delay(300);
	Arm_Dtct();		// 归位为检测位置
}

/*
机械臂从存储机构取出物料并放置在圆环
*/
void Arm_Action_Store_To_Ground(ArmTarget color)
{
	servor_ctl(servor_object_CLAW, 1);	 // 张爪
	Arm_Z_Middle();		 // Z抬高
	Arm_Store(color);	 // 移到存储处对应位置
	servor_ctl(servor_object_CLAW, 2); // 闭
	HAL_Delay(300);
	Arm_Z_Middle();			// Z抬高
	Arm_GroundPlace(color); // 移到对应圆环位置
	servor_ctl(servor_object_CLAW, 1);		// 张爪
	HAL_Delay(300);
	Arm_Z_Middle(); // Z抬高
}

/*
码垛动作
*/
void Arm_Action_Maduo(ArmTarget color)
{
	servor_ctl(servor_object_CLAW, 1);	 // 张爪
	Arm_Z_Middle();		 // Z抬高
	Arm_Store(color);	 // 移到对应存储机构位置
	servor_ctl(servor_object_CLAW, 2); // 闭
	HAL_Delay(300);
	Arm_Z_Middle();		// Z抬高
	Arm_Maduo(color);	// 移到对应码垛位置
	servor_ctl(servor_object_CLAW, 1); // 张爪
	HAL_Delay(300);
	Arm_Z_Middle(); // Z抬高
}

/*
从地面拾取物料并存放到对应存储机构
*/
void Arm_Action_Ground_To_Store(ArmTarget color)
{
	servor_ctl(servor_object_CLAW, 1);		// 张爪
	Arm_Dtct();				//移到检测位置
	Arm_GroundPlace(color); // 移到对应圆环位置
	servor_ctl(servor_object_CLAW, 2);	// 闭
	HAL_Delay(1000);
	Arm_Z_Middle();		// Z抬高
	Arm_Store(color);	// 移到对应存储机构位置
	servor_ctl(servor_object_CLAW, 1); // 张爪
	HAL_Delay(1000);
	Arm_Z_Middle(); // Z抬高
}

// 机械臂总控
void Arm_Action(ArmAction action, ArmTarget color)
{
	switch (action)
	{
	case Ordinary3: // 一般位置
		Arm_Dtct();
		break;
	case Scan: // 扫码位置
		Arm_Scan();
		break;
	case Dtct: // 在圆盘上检测位置
		Arm_Dtct();
		break;
	case Plate_To_Store: // 圆盘-->存储
		Arm_Action_GetPlate(color);
		break;
	case Store_To_Ground: // 存储-->地面
		Arm_Action_Store_To_Ground(color);
		break;
	case Ground_To_Store: // 地面-->存储
		Arm_Action_Ground_To_Store(color);
		break;
	case Maduo: // 码垛
		Arm_Action_Maduo(color);
		break;
	case Store_Open: // 存储机构开关
		servor_ctl(servor_object_PLATES, 1);;
		break;
	case Store_Close:
		servor_ctl(servor_object_PLATES, 2);;
		break;
	}
}

