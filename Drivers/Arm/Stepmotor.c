#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
//串口4为底盘四电机多机通信串口  921600
//串口7为滑轨通信串口						921600
/*	
	默认波特率 115200 但我修改成921600了 测试感觉反而稳定
	不知道什么原因在中断以外的地方串口发送，电机接收数据没反应
	上位机却正常，所以写在了control_logic.c下的tim2中断dma发送
	置send_flag标志位为1发送，send_num为发送字节长度
*/
uint8_t M_Tx_Data[20];//发送电机运动命令的数组
uint8_t M_Rx_Data[20];//接受数组
uint8_t send_flag=0;//发送标志位
uint8_t send_num=0;//发送字节长度
uint8_t Screwrod_send_flag=0;//丝杆发送标志位
uint8_t RobotArm_SendFlag=0;//机械臂发送标志位
#ifdef Emm_V5
/**
 * @brief 使能电机
 */
void Motor_Enable(Motor_Address Motor_Address)
{
	//01 F3 AB 01 00 6B
	

	M_Tx_Data[0]=Motor_Address;//0x00广播模式
	M_Tx_Data[1]=0xF3;//命令标志位
	M_Tx_Data[2]=0xAB;//命令标志位
	M_Tx_Data[3]=0x01;//使能状态
	M_Tx_Data[4]=0x00;//多机同步标志位
	M_Tx_Data[5]=0x6B;//校验位，本模式下恒为0x6b
	send_num=6;
	send_flag=1;	
	HAL_Delay(2);
}
/**
 * @brief 失能电机
 */
void Motor_Disable(Motor_Address Motor_Address)
{
	//01 F3 AB 00 00 6B

	M_Tx_Data[0]=Motor_Address;//0x00广播模式
	M_Tx_Data[1]=0xF3;//命令标志位
	M_Tx_Data[2]=0xAB;//命令标志位
	M_Tx_Data[3]=0x00;//使能状态
	M_Tx_Data[4]=0x00;//多机同步标志位
	M_Tx_Data[5]=0x6B;//校验位，本模式下恒为0x6b
	send_num=6;
	send_flag=1;
	HAL_Delay(2);
}

/**
 * @brief 设置速度，闭环
 * @brief 帧格式 地址 + 0xF6 + 符号 + 速度斜率 + 速度 + 多机同步标志 + 校验字节
 */
void Set_Speed(Motor_Address Motor_Address,Motor_Dir Dir,uint16_t Acceleration,uint16_t Target_Speed,MotorMulti_Motion_Mode Synchronous_Motion)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0xF6;
	//速度方向
	M_Tx_Data[2]=Dir;
	//速度
	M_Tx_Data[3]=Target_Speed>>8;
	M_Tx_Data[4]=Target_Speed;
	//加速度档位
	M_Tx_Data[5]=Acceleration;
	//同步标志位
	M_Tx_Data[6]=Synchronous_Motion;
	//校验字节
	M_Tx_Data[7]=0x6B;
	send_num=8;
	send_flag=1;
	HAL_Delay(2);
}

void Set_Position(Motor_Address Motor_Address,
				  Motor_Dir Dir,
				  uint16_t Acceleration,
				  uint16_t MAX_Speed,
				  uint32_t Pulse,
				  Motor_Angle_Mode Angle_mode,
				  MotorMulti_Motion_Mode Synchronous_Motion)
{
		//地址
	M_Tx_Data[0]=Motor_Address;
	//命令标志位
	M_Tx_Data[1]=0xFD;
	//方向
	M_Tx_Data[2]=Dir;
	//加速度
	M_Tx_Data[3]=MAX_Speed>>8;
	M_Tx_Data[4]=MAX_Speed;
	//加速度档位
	M_Tx_Data[5]=Acceleration;
	//脉冲
	M_Tx_Data[6]=Pulse>>24;
	M_Tx_Data[7]=Pulse>>16;
	M_Tx_Data[8]=Pulse>>8;
	M_Tx_Data[9]=Pulse;
	//角度模式
	M_Tx_Data[10]=Angle_mode;
	//同步标志位
	M_Tx_Data[11]=Synchronous_Motion;
	//校验字节
	M_Tx_Data[12]=0x6B;
//	HAL_UART_Transmit_DMA(&huart4,M_Tx_Data,16);
	
	send_num=13;
	send_flag=1;
	HAL_Delay(2);
}
void Motor_Emer_Stop(Motor_Address Motor_Address)
{
	//地址 + 0xFE + 0x98 + 多机同步标志 + 校验字节

	M_Tx_Data[0]=Motor_Address;
	M_Tx_Data[1]=0xFE;
	M_Tx_Data[2]=0x98;
	M_Tx_Data[3]=0x00;
	M_Tx_Data[4]=0x6B;
	
	send_num=5;
	send_flag=1;
	HAL_Delay(2);
}
/**
 * @brief 这个函数用来多机同步运动
 * @brief 电机在接收指令时，如果多机同步标志位为0x01,电机不会立刻运动而是等待多机同步命令后，所有电机同步执行此前存储的命令
 */
void Synchronous_Motion(void)
{
	M_Tx_Data[0]=0x00;
	M_Tx_Data[1]=0xFF;
	M_Tx_Data[2]=0x66;
	M_Tx_Data[3]=0x6B;
	
	send_num=4;
	send_flag=1;
	HAL_Delay(2);
}

/**
 * @brief 将当前的位置角度清零
 */
void Clear_AngleCounter(Motor_Address Motor_Address)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0x0A;
	//命令字节
	M_Tx_Data[2]=0x6D;
	//校验字节
	M_Tx_Data[3]=0X6B;
	send_num=4;
	send_flag=1;
	HAL_Delay(2);
}
//滑轨电机使能
void Screwrod_Enable(void)
{
	M_Tx_Data[0]=MOTOR1;//0x00广播模式
	M_Tx_Data[1]=0xF3;//命令标志位
	M_Tx_Data[2]=0xAB;//命令标志位
	M_Tx_Data[3]=0x01;//使能状态
	M_Tx_Data[4]=0x00;//多机同步标志位
	M_Tx_Data[5]=0x6B;//校验位，本模式下恒为0x6b
	send_num=6;
	Screwrod_send_flag=1;	
	HAL_Delay(5);
}
//设置滑轨位置，绝对编码模式
void Screwrod_SetPositon(Motor_Dir Dir,
													uint16_t Acceleration,
													uint16_t MAX_Speed,
													uint32_t Pulse)
{
	//地址
	M_Tx_Data[0]=MOTOR1;
	//命令标志位
	M_Tx_Data[1]=0xFD;
	//方向
	M_Tx_Data[2]=Dir;
	//加速度
	M_Tx_Data[3]=MAX_Speed>>8;
	M_Tx_Data[4]=MAX_Speed;
	//加速度档位
	M_Tx_Data[5]=Acceleration;
	//脉冲
	M_Tx_Data[6]=Pulse>>24;
	M_Tx_Data[7]=Pulse>>16;
	M_Tx_Data[8]=Pulse>>8;
	M_Tx_Data[9]=Pulse;
	//角度模式
	M_Tx_Data[10]=RELATIVE;
	//同步标志位
	M_Tx_Data[11]=ASYNCHRONOUS;
	//校验字节
	M_Tx_Data[12]=0x6B;
//	HAL_UART_Transmit_DMA(&huart4,M_Tx_Data,16);
	
	send_num=13;
	Screwrod_send_flag=1;
	HAL_Delay(5);
}
//滑轨急停
void Screwrod_Emer_Stop(void)
{
	M_Tx_Data[0]=MOTOR1;
	M_Tx_Data[1]=0xFE;
	M_Tx_Data[2]=0x98;
	M_Tx_Data[3]=0x00;
	M_Tx_Data[4]=0x6B;
	
	send_num=5;
	Screwrod_send_flag=1;
	HAL_Delay(5);
}
//滑轨清除编码器值
void Screwrod_Clear_AngleCounter(void)
{
	//地址
	M_Tx_Data[0]=MOTOR1;	
	//命令字节
	M_Tx_Data[1]=0x0A;
	//命令字节
	M_Tx_Data[2]=0x6D;
	//校验字节
	M_Tx_Data[3]=0X6B;
	send_num=4;
	Screwrod_send_flag=1;
	HAL_Delay(2);
}
//解除堵住保护
//命令格式：地址 + 0x0E + 0x52 + 校验字节
void Screwrod_Blockage_protection_Disable(void)
{
	//地址
	M_Tx_Data[0]=MOTOR1;	
	//命令字节
	M_Tx_Data[1]=0x0E;
	//命令字节
	M_Tx_Data[2]=0x52;
	//校验字节
	M_Tx_Data[3]=0X6B;
	send_num=4;
	Screwrod_send_flag=1;
	HAL_Delay(2);	
}

/**
 * @brief 使能电机
 */
void RobotArm_Motor_Enable(Motor_Address Motor_Address)
{
	//01 F3 AB 01 00 6B
	

	M_Tx_Data[0]=Motor_Address;//0x00广播模式
	M_Tx_Data[1]=0xF3;//命令标志位
	M_Tx_Data[2]=0xAB;//命令标志位
	M_Tx_Data[3]=0x01;//使能状态
	M_Tx_Data[4]=0x00;//多机同步标志位
	M_Tx_Data[5]=0x6B;//校验位，本模式下恒为0x6b
	send_num=6;
	RobotArm_SendFlag=1;	
	HAL_Delay(2);
}
/**
 * @brief 失能电机
 */
void RobotArm_Motor_Disable(Motor_Address Motor_Address)
{
	//01 F3 AB 00 00 6B

	M_Tx_Data[0]=Motor_Address;//0x00广播模式
	M_Tx_Data[1]=0xF3;//命令标志位
	M_Tx_Data[2]=0xAB;//命令标志位
	M_Tx_Data[3]=0x00;//使能状态
	M_Tx_Data[4]=0x00;//多机同步标志位
	M_Tx_Data[5]=0x6B;//校验位，本模式下恒为0x6b
	send_num=6;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}

/**
 * @brief 设置速度，闭环
 * @brief 帧格式 地址 + 0xF6 + 符号 + 速度斜率 + 速度 + 多机同步标志 + 校验字节
 */
void RobotArm_Set_Speed(Motor_Address Motor_Address,Motor_Dir Dir,uint16_t Acceleration,uint16_t Target_Speed,MotorMulti_Motion_Mode Synchronous_Motion)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0xF6;
	//速度方向
	M_Tx_Data[2]=Dir;
	//速度
	M_Tx_Data[3]=Target_Speed>>8;
	M_Tx_Data[4]=Target_Speed;
	//加速度档位
	M_Tx_Data[5]=Acceleration;
	//同步标志位
	M_Tx_Data[6]=Synchronous_Motion;
	//校验字节
	M_Tx_Data[7]=0x6B;
	send_num=8;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}

void RobotArm_Set_Position(Motor_Address Motor_Address,
				  Motor_Dir Dir,
				  uint16_t Acceleration,
				  uint16_t MAX_Speed,
				  uint32_t Pulse,
				  Motor_Angle_Mode Angle_mode,
				  MotorMulti_Motion_Mode Synchronous_Motion)
{
		//地址
	M_Tx_Data[0]=Motor_Address;
	//命令标志位
	M_Tx_Data[1]=0xFD;
	//方向
	M_Tx_Data[2]=Dir;
	//加速度
	M_Tx_Data[3]=MAX_Speed>>8;
	M_Tx_Data[4]=MAX_Speed;
	//加速度档位
	M_Tx_Data[5]=Acceleration;
	//脉冲
	M_Tx_Data[6]=Pulse>>24;
	M_Tx_Data[7]=Pulse>>16;
	M_Tx_Data[8]=Pulse>>8;
	M_Tx_Data[9]=Pulse;
	//角度模式
	M_Tx_Data[10]=Angle_mode;
	//同步标志位
	M_Tx_Data[11]=Synchronous_Motion;
	//校验字节
	M_Tx_Data[12]=0x6B;
//	HAL_UART_Transmit_DMA(&huart4,M_Tx_Data,16);
	
	send_num=13;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}
void RobotArm_Emer_Stop(Motor_Address Motor_Address)
{
	//地址 + 0xFE + 0x98 + 多机同步标志 + 校验字节

	M_Tx_Data[0]=Motor_Address;
	M_Tx_Data[1]=0xFE;
	M_Tx_Data[2]=0x98;
	M_Tx_Data[3]=0x00;
	M_Tx_Data[4]=0x6B;
	
	send_num=5;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}
/**
 * @brief 这个函数用来多机同步运动
 * @brief 电机在接收指令时，如果多机同步标志位为0x01,电机不会立刻运动而是等待多机同步命令后，所有电机同步执行此前存储的命令
 */
void RobotArm_Synchronous_Motion(void)
{
	M_Tx_Data[0]=0x00;
	M_Tx_Data[1]=0xFF;
	M_Tx_Data[2]=0x66;
	M_Tx_Data[3]=0x6B;
	
	send_num=4;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}

/**
 * @brief 将当前的位置角度清零
 */
void RobotArm_Clear_AngleCounter(Motor_Address Motor_Address)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0x0A;
	//命令字节
	M_Tx_Data[2]=0x6D;
	//校验字节
	M_Tx_Data[3]=0X6B;
	send_num=4;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}

//解除堵住保护
//命令格式：地址 + 0x0E + 0x52 + 校验字节
void RobotArm_Blockage_protection_Disable(Motor_Address Motor_Address)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0x0E;
	//命令字节
	M_Tx_Data[2]=0x52;
	//校验字节
	M_Tx_Data[3]=0X6B;
	send_num=4;
	RobotArm_SendFlag=1;
	HAL_Delay(2);	
}

#endif

#ifdef ZDTX42

/**
 * @brief 使能电机
 */
void Motor_Enable(Motor_Address Motor_Address)
{
	//01 F3 AB 01 00 6B
	

	M_Tx_Data[0]=Motor_Address;//0x00广播模式
	M_Tx_Data[1]=0xF3;//命令标志位
	M_Tx_Data[2]=0xAB;//命令标志位
	M_Tx_Data[3]=0x01;//使能状态
	M_Tx_Data[4]=0x00;//多机同步标志位
	M_Tx_Data[5]=0x6B;//校验位，本模式下恒为0x6b
	send_num=6;
	send_flag=1;
	HAL_Delay(2);
}
/**
 * @brief 失能电机
 */
void Motor_Disable(Motor_Address Motor_Address)
{
	//01 F3 AB 00 00 6B

	M_Tx_Data[0]=Motor_Address;//0x00广播模式
	M_Tx_Data[1]=0xF3;//命令标志位
	M_Tx_Data[2]=0xAB;//命令标志位
	M_Tx_Data[3]=0x00;//使能状态
	M_Tx_Data[4]=0x00;//多机同步标志位
	M_Tx_Data[5]=0x6B;//校验位，本模式下恒为0x6b
	send_num=6;
	send_flag=1;
	HAL_Delay(2);
}
/**
 * @brief 这个函数用来电机制动
 * @param Motor_Address 传入电机的地址
 */
void Motor_Emer_Stop(Motor_Address Motor_Address)
{
	//地址 + 0xFE + 0x98 + 多机同步标志 + 校验字节

	M_Tx_Data[0]=Motor_Address;
	M_Tx_Data[1]=0xFE;
	M_Tx_Data[2]=0x98;
	M_Tx_Data[3]=0x00;
	M_Tx_Data[4]=0x6B;
	
	send_num=5;
	send_flag=1;
	HAL_Delay(2);
}
/**
 * @brief 这个函数用来多机同步运动
 * @brief 电机在接收指令时，如果多机同步标志位为0x01,电机不会立刻运动而是等待多机同步命令后，所有电机同步执行此前存储的命令
 */
void Synchronous_Motion(void)
{
	M_Tx_Data[0]=0x00;
	M_Tx_Data[1]=0xFF;
	M_Tx_Data[2]=0x66;
	M_Tx_Data[3]=0x6B;
	
	send_num=4;
	send_flag=1;
	HAL_Delay(2);
}

/**
 * @brief 梯形曲线位置模式控制 
 * @brief 帧格式 地址 +0xFD+ 符号（方向） + 加速加速度 + 减速加速度 + 最大速度+ 位置角度 + 相对位置/绝对位置标志 + 多机同步标志 + 0x6b
 * @param Motor_Address 电机地址
 * @param Dir 方向
 * @param Acceleration 最大加速度
 * @param Deceleration 最大减速度
 * @param MAX_Speed 最大速度
 * @param Angle 旋转角度
 * @param Angle_mode 角度模式 0x00相对位置，0x01绝对位置
 * @param Synchronous_Motion 多机同步标志位
 */
void Trapezoidal_Curve_Position_Mode(Motor_Address Motor_Address,
									 Motor_Dir Dir,
									 uint16_t Acceleration,
									 uint16_t Deceleration,
									 uint16_t MAX_Speed,
									 uint32_t Angle,
									 Motor_Angle_Mode Angle_mode,
									 MotorMulti_Motion_Mode Synchronous_Motion)
{ 
	//地址
	M_Tx_Data[0]=Motor_Address;
	//命令标志位
	M_Tx_Data[1]=0xFD;
	//方向
	M_Tx_Data[2]=Dir;
	//加速度
	M_Tx_Data[3]=Acceleration>>8;
	M_Tx_Data[4]=Acceleration;
	//减速度
	M_Tx_Data[5]=Deceleration>>8;
	M_Tx_Data[6]=Deceleration;
	//最大速度
	M_Tx_Data[7]=MAX_Speed>>8;
	M_Tx_Data[8]=MAX_Speed;
	//角度
	M_Tx_Data[9]=Angle>>24;
	M_Tx_Data[10]=Angle>>16;
	M_Tx_Data[11]=Angle>>8;
	M_Tx_Data[12]=Angle;
	//角度模式
	M_Tx_Data[13]=Angle_mode;
	//同步标志位
	M_Tx_Data[14]=Synchronous_Motion;
	//校验字节
	M_Tx_Data[15]=0x6B;
//	HAL_UART_Transmit_DMA(&huart4,M_Tx_Data,16);
	send_num=16;
	send_flag=1;
	HAL_Delay(2);
}
/**
 * @brief 直通限速位置模式控制
 * @brief 以最快速度达到指定位置
 * @brief 帧格式 地址 + 0xFB + 符号（方向） + 速度 + 位置角度 + 相对位置/绝对位置标志 + 多机同步标志 + 校验字节
 */
void Direct_speed_limit_position(Motor_Address Motor_Address,
								 Motor_Dir Dir,
								 uint16_t Speed,
								 uint32_t Angle,
								 Motor_Angle_Mode Angle_mode,
								 MotorMulti_Motion_Mode Synchronous_Motion)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0xFB;
	//速度方向
	M_Tx_Data[2]=Dir;
	//速度
	M_Tx_Data[3]=Speed>>8;
	M_Tx_Data[4]=Speed;
	//角度
	M_Tx_Data[5]=Angle>>24;
	M_Tx_Data[6]=Angle>>16;
	M_Tx_Data[7]=Angle>>8;
	M_Tx_Data[8]=Angle;
	//角度模式
	M_Tx_Data[9]=Angle_mode;
	//同步标志位
	M_Tx_Data[10]=Synchronous_Motion;
	//校验字节
	M_Tx_Data[11]=0x6B;
	send_num=12;
	send_flag=1;
	HAL_Delay(2);
}


/**
 * @brief 设置速度，闭环
 * @brief 帧格式 地址 + 0xF6 + 符号 + 速度斜率 + 速度 + 多机同步标志 + 校验字节
 */
void Set_Speed(Motor_Address Motor_Address,Motor_Dir Dir,uint16_t Acceleration,uint16_t Target_Speed,MotorMulti_Motion_Mode Synchronous_Motion)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0xF6;
	//速度方向
	M_Tx_Data[2]=Dir;
	//加速度
	M_Tx_Data[3]=Acceleration>>8;
	M_Tx_Data[4]=Acceleration;
	//速度
	M_Tx_Data[5]=Target_Speed>>8;
	M_Tx_Data[6]=Target_Speed;
	//同步标志位
	M_Tx_Data[7]=Synchronous_Motion;
	//校验字节
	M_Tx_Data[8]=0x6B;
	send_num=9;
	send_flag=1;
	HAL_Delay(2);
}

/**
 * @brief 将当前的位置角度清零
 */
void Clear_AngleCounter(Motor_Address Motor_Address)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0x0A;
	//命令字节
	M_Tx_Data[2]=0x6D;
	//校验字节
	M_Tx_Data[3]=0X6B;
	send_num=4;
	send_flag=1;
	HAL_Delay(2);
}

/**
 * @brief 力矩模式
 * @brief 帧格式 ：地址 + 0xF5 + 符号 + 电流斜率 + 电流 + 多机同步标志 + 校验字节
 */
void Torque_Mode_Ctrl(Motor_Address Motor_Address,
					  Motor_Dir Dir,
					  uint16_t Torque_Acc,
					  uint16_t Torque_target,
					  MotorMulti_Motion_Mode Synchronous_Motion)
{
	//地址
	M_Tx_Data[0]=Motor_Address;	
	//命令字节
	M_Tx_Data[1]=0xF5 ;
	//速度方向
	M_Tx_Data[2]=Dir;
	//加速度
	M_Tx_Data[3]=Torque_Acc>>8;
	M_Tx_Data[4]=Torque_Acc;
	//速度
	M_Tx_Data[5]=Torque_target>>8;
	M_Tx_Data[6]=Torque_target;
	//同步标志位
	M_Tx_Data[7]=Synchronous_Motion;
	//校验字节
	M_Tx_Data[8]=0x6B;
	send_num=9;
	send_flag=1;
	HAL_Delay(2);
}
#endif
