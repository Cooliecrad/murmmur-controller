#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
//����4Ϊ�����ĵ�����ͨ�Ŵ���  921600
//����7Ϊ����ͨ�Ŵ���						921600
/*	
	Ĭ�ϲ����� 115200 �����޸ĳ�921600�� ���Ըо������ȶ�
	��֪��ʲôԭ�����ж�����ĵط����ڷ��ͣ������������û��Ӧ
	��λ��ȴ����������д����control_logic.c�µ�tim2�ж�dma����
	��send_flag��־λΪ1���ͣ�send_numΪ�����ֽڳ���
*/
uint8_t M_Tx_Data[20];//���͵���˶����������
uint8_t M_Rx_Data[20];//��������
uint8_t send_flag=0;//���ͱ�־λ
uint8_t send_num=0;//�����ֽڳ���
uint8_t Screwrod_send_flag=0;//˿�˷��ͱ�־λ
uint8_t RobotArm_SendFlag=0;//��е�۷��ͱ�־λ
#ifdef Emm_V5
/**
 * @brief ʹ�ܵ��
 */
void Motor_Enable(Motor_Address Motor_Address)
{
	//01 F3 AB 01 00 6B
	

	M_Tx_Data[0]=Motor_Address;//0x00�㲥ģʽ
	M_Tx_Data[1]=0xF3;//�����־λ
	M_Tx_Data[2]=0xAB;//�����־λ
	M_Tx_Data[3]=0x01;//ʹ��״̬
	M_Tx_Data[4]=0x00;//���ͬ����־λ
	M_Tx_Data[5]=0x6B;//У��λ����ģʽ�º�Ϊ0x6b
	send_num=6;
	send_flag=1;	
	HAL_Delay(2);
}
/**
 * @brief ʧ�ܵ��
 */
void Motor_Disable(Motor_Address Motor_Address)
{
	//01 F3 AB 00 00 6B

	M_Tx_Data[0]=Motor_Address;//0x00�㲥ģʽ
	M_Tx_Data[1]=0xF3;//�����־λ
	M_Tx_Data[2]=0xAB;//�����־λ
	M_Tx_Data[3]=0x00;//ʹ��״̬
	M_Tx_Data[4]=0x00;//���ͬ����־λ
	M_Tx_Data[5]=0x6B;//У��λ����ģʽ�º�Ϊ0x6b
	send_num=6;
	send_flag=1;
	HAL_Delay(2);
}

/**
 * @brief �����ٶȣ��ջ�
 * @brief ֡��ʽ ��ַ + 0xF6 + ���� + �ٶ�б�� + �ٶ� + ���ͬ����־ + У���ֽ�
 */
void Set_Speed(Motor_Address Motor_Address,Motor_Dir Dir,uint16_t Acceleration,uint16_t Target_Speed,MotorMulti_Motion_Mode Synchronous_Motion)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0xF6;
	//�ٶȷ���
	M_Tx_Data[2]=Dir;
	//�ٶ�
	M_Tx_Data[3]=Target_Speed>>8;
	M_Tx_Data[4]=Target_Speed;
	//���ٶȵ�λ
	M_Tx_Data[5]=Acceleration;
	//ͬ����־λ
	M_Tx_Data[6]=Synchronous_Motion;
	//У���ֽ�
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
		//��ַ
	M_Tx_Data[0]=Motor_Address;
	//�����־λ
	M_Tx_Data[1]=0xFD;
	//����
	M_Tx_Data[2]=Dir;
	//���ٶ�
	M_Tx_Data[3]=MAX_Speed>>8;
	M_Tx_Data[4]=MAX_Speed;
	//���ٶȵ�λ
	M_Tx_Data[5]=Acceleration;
	//����
	M_Tx_Data[6]=Pulse>>24;
	M_Tx_Data[7]=Pulse>>16;
	M_Tx_Data[8]=Pulse>>8;
	M_Tx_Data[9]=Pulse;
	//�Ƕ�ģʽ
	M_Tx_Data[10]=Angle_mode;
	//ͬ����־λ
	M_Tx_Data[11]=Synchronous_Motion;
	//У���ֽ�
	M_Tx_Data[12]=0x6B;
//	HAL_UART_Transmit_DMA(&huart4,M_Tx_Data,16);
	
	send_num=13;
	send_flag=1;
	HAL_Delay(2);
}
void Motor_Emer_Stop(Motor_Address Motor_Address)
{
	//��ַ + 0xFE + 0x98 + ���ͬ����־ + У���ֽ�

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
 * @brief ��������������ͬ���˶�
 * @brief ����ڽ���ָ��ʱ��������ͬ����־λΪ0x01,������������˶����ǵȴ����ͬ����������е��ͬ��ִ�д�ǰ�洢������
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
 * @brief ����ǰ��λ�ýǶ�����
 */
void Clear_AngleCounter(Motor_Address Motor_Address)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0x0A;
	//�����ֽ�
	M_Tx_Data[2]=0x6D;
	//У���ֽ�
	M_Tx_Data[3]=0X6B;
	send_num=4;
	send_flag=1;
	HAL_Delay(2);
}
//������ʹ��
void Screwrod_Enable(void)
{
	M_Tx_Data[0]=MOTOR1;//0x00�㲥ģʽ
	M_Tx_Data[1]=0xF3;//�����־λ
	M_Tx_Data[2]=0xAB;//�����־λ
	M_Tx_Data[3]=0x01;//ʹ��״̬
	M_Tx_Data[4]=0x00;//���ͬ����־λ
	M_Tx_Data[5]=0x6B;//У��λ����ģʽ�º�Ϊ0x6b
	send_num=6;
	Screwrod_send_flag=1;	
	HAL_Delay(5);
}
//���û���λ�ã����Ա���ģʽ
void Screwrod_SetPositon(Motor_Dir Dir,
													uint16_t Acceleration,
													uint16_t MAX_Speed,
													uint32_t Pulse)
{
	//��ַ
	M_Tx_Data[0]=MOTOR1;
	//�����־λ
	M_Tx_Data[1]=0xFD;
	//����
	M_Tx_Data[2]=Dir;
	//���ٶ�
	M_Tx_Data[3]=MAX_Speed>>8;
	M_Tx_Data[4]=MAX_Speed;
	//���ٶȵ�λ
	M_Tx_Data[5]=Acceleration;
	//����
	M_Tx_Data[6]=Pulse>>24;
	M_Tx_Data[7]=Pulse>>16;
	M_Tx_Data[8]=Pulse>>8;
	M_Tx_Data[9]=Pulse;
	//�Ƕ�ģʽ
	M_Tx_Data[10]=RELATIVE;
	//ͬ����־λ
	M_Tx_Data[11]=ASYNCHRONOUS;
	//У���ֽ�
	M_Tx_Data[12]=0x6B;
//	HAL_UART_Transmit_DMA(&huart4,M_Tx_Data,16);
	
	send_num=13;
	Screwrod_send_flag=1;
	HAL_Delay(5);
}
//���켱ͣ
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
//�������������ֵ
void Screwrod_Clear_AngleCounter(void)
{
	//��ַ
	M_Tx_Data[0]=MOTOR1;	
	//�����ֽ�
	M_Tx_Data[1]=0x0A;
	//�����ֽ�
	M_Tx_Data[2]=0x6D;
	//У���ֽ�
	M_Tx_Data[3]=0X6B;
	send_num=4;
	Screwrod_send_flag=1;
	HAL_Delay(2);
}
//�����ס����
//�����ʽ����ַ + 0x0E + 0x52 + У���ֽ�
void Screwrod_Blockage_protection_Disable(void)
{
	//��ַ
	M_Tx_Data[0]=MOTOR1;	
	//�����ֽ�
	M_Tx_Data[1]=0x0E;
	//�����ֽ�
	M_Tx_Data[2]=0x52;
	//У���ֽ�
	M_Tx_Data[3]=0X6B;
	send_num=4;
	Screwrod_send_flag=1;
	HAL_Delay(2);	
}

/**
 * @brief ʹ�ܵ��
 */
void RobotArm_Motor_Enable(Motor_Address Motor_Address)
{
	//01 F3 AB 01 00 6B
	

	M_Tx_Data[0]=Motor_Address;//0x00�㲥ģʽ
	M_Tx_Data[1]=0xF3;//�����־λ
	M_Tx_Data[2]=0xAB;//�����־λ
	M_Tx_Data[3]=0x01;//ʹ��״̬
	M_Tx_Data[4]=0x00;//���ͬ����־λ
	M_Tx_Data[5]=0x6B;//У��λ����ģʽ�º�Ϊ0x6b
	send_num=6;
	RobotArm_SendFlag=1;	
	HAL_Delay(2);
}
/**
 * @brief ʧ�ܵ��
 */
void RobotArm_Motor_Disable(Motor_Address Motor_Address)
{
	//01 F3 AB 00 00 6B

	M_Tx_Data[0]=Motor_Address;//0x00�㲥ģʽ
	M_Tx_Data[1]=0xF3;//�����־λ
	M_Tx_Data[2]=0xAB;//�����־λ
	M_Tx_Data[3]=0x00;//ʹ��״̬
	M_Tx_Data[4]=0x00;//���ͬ����־λ
	M_Tx_Data[5]=0x6B;//У��λ����ģʽ�º�Ϊ0x6b
	send_num=6;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}

/**
 * @brief �����ٶȣ��ջ�
 * @brief ֡��ʽ ��ַ + 0xF6 + ���� + �ٶ�б�� + �ٶ� + ���ͬ����־ + У���ֽ�
 */
void RobotArm_Set_Speed(Motor_Address Motor_Address,Motor_Dir Dir,uint16_t Acceleration,uint16_t Target_Speed,MotorMulti_Motion_Mode Synchronous_Motion)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0xF6;
	//�ٶȷ���
	M_Tx_Data[2]=Dir;
	//�ٶ�
	M_Tx_Data[3]=Target_Speed>>8;
	M_Tx_Data[4]=Target_Speed;
	//���ٶȵ�λ
	M_Tx_Data[5]=Acceleration;
	//ͬ����־λ
	M_Tx_Data[6]=Synchronous_Motion;
	//У���ֽ�
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
		//��ַ
	M_Tx_Data[0]=Motor_Address;
	//�����־λ
	M_Tx_Data[1]=0xFD;
	//����
	M_Tx_Data[2]=Dir;
	//���ٶ�
	M_Tx_Data[3]=MAX_Speed>>8;
	M_Tx_Data[4]=MAX_Speed;
	//���ٶȵ�λ
	M_Tx_Data[5]=Acceleration;
	//����
	M_Tx_Data[6]=Pulse>>24;
	M_Tx_Data[7]=Pulse>>16;
	M_Tx_Data[8]=Pulse>>8;
	M_Tx_Data[9]=Pulse;
	//�Ƕ�ģʽ
	M_Tx_Data[10]=Angle_mode;
	//ͬ����־λ
	M_Tx_Data[11]=Synchronous_Motion;
	//У���ֽ�
	M_Tx_Data[12]=0x6B;
//	HAL_UART_Transmit_DMA(&huart4,M_Tx_Data,16);
	
	send_num=13;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}
void RobotArm_Emer_Stop(Motor_Address Motor_Address)
{
	//��ַ + 0xFE + 0x98 + ���ͬ����־ + У���ֽ�

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
 * @brief ��������������ͬ���˶�
 * @brief ����ڽ���ָ��ʱ��������ͬ����־λΪ0x01,������������˶����ǵȴ����ͬ����������е��ͬ��ִ�д�ǰ�洢������
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
 * @brief ����ǰ��λ�ýǶ�����
 */
void RobotArm_Clear_AngleCounter(Motor_Address Motor_Address)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0x0A;
	//�����ֽ�
	M_Tx_Data[2]=0x6D;
	//У���ֽ�
	M_Tx_Data[3]=0X6B;
	send_num=4;
	RobotArm_SendFlag=1;
	HAL_Delay(2);
}

//�����ס����
//�����ʽ����ַ + 0x0E + 0x52 + У���ֽ�
void RobotArm_Blockage_protection_Disable(Motor_Address Motor_Address)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0x0E;
	//�����ֽ�
	M_Tx_Data[2]=0x52;
	//У���ֽ�
	M_Tx_Data[3]=0X6B;
	send_num=4;
	RobotArm_SendFlag=1;
	HAL_Delay(2);	
}

#endif

#ifdef ZDTX42

/**
 * @brief ʹ�ܵ��
 */
void Motor_Enable(Motor_Address Motor_Address)
{
	//01 F3 AB 01 00 6B
	

	M_Tx_Data[0]=Motor_Address;//0x00�㲥ģʽ
	M_Tx_Data[1]=0xF3;//�����־λ
	M_Tx_Data[2]=0xAB;//�����־λ
	M_Tx_Data[3]=0x01;//ʹ��״̬
	M_Tx_Data[4]=0x00;//���ͬ����־λ
	M_Tx_Data[5]=0x6B;//У��λ����ģʽ�º�Ϊ0x6b
	send_num=6;
	send_flag=1;
	HAL_Delay(2);
}
/**
 * @brief ʧ�ܵ��
 */
void Motor_Disable(Motor_Address Motor_Address)
{
	//01 F3 AB 00 00 6B

	M_Tx_Data[0]=Motor_Address;//0x00�㲥ģʽ
	M_Tx_Data[1]=0xF3;//�����־λ
	M_Tx_Data[2]=0xAB;//�����־λ
	M_Tx_Data[3]=0x00;//ʹ��״̬
	M_Tx_Data[4]=0x00;//���ͬ����־λ
	M_Tx_Data[5]=0x6B;//У��λ����ģʽ�º�Ϊ0x6b
	send_num=6;
	send_flag=1;
	HAL_Delay(2);
}
/**
 * @brief ���������������ƶ�
 * @param Motor_Address �������ĵ�ַ
 */
void Motor_Emer_Stop(Motor_Address Motor_Address)
{
	//��ַ + 0xFE + 0x98 + ���ͬ����־ + У���ֽ�

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
 * @brief ��������������ͬ���˶�
 * @brief ����ڽ���ָ��ʱ��������ͬ����־λΪ0x01,������������˶����ǵȴ����ͬ����������е��ͬ��ִ�д�ǰ�洢������
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
 * @brief ��������λ��ģʽ���� 
 * @brief ֡��ʽ ��ַ +0xFD+ ���ţ����� + ���ټ��ٶ� + ���ټ��ٶ� + ����ٶ�+ λ�ýǶ� + ���λ��/����λ�ñ�־ + ���ͬ����־ + 0x6b
 * @param Motor_Address �����ַ
 * @param Dir ����
 * @param Acceleration �����ٶ�
 * @param Deceleration �����ٶ�
 * @param MAX_Speed ����ٶ�
 * @param Angle ��ת�Ƕ�
 * @param Angle_mode �Ƕ�ģʽ 0x00���λ�ã�0x01����λ��
 * @param Synchronous_Motion ���ͬ����־λ
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
	//��ַ
	M_Tx_Data[0]=Motor_Address;
	//�����־λ
	M_Tx_Data[1]=0xFD;
	//����
	M_Tx_Data[2]=Dir;
	//���ٶ�
	M_Tx_Data[3]=Acceleration>>8;
	M_Tx_Data[4]=Acceleration;
	//���ٶ�
	M_Tx_Data[5]=Deceleration>>8;
	M_Tx_Data[6]=Deceleration;
	//����ٶ�
	M_Tx_Data[7]=MAX_Speed>>8;
	M_Tx_Data[8]=MAX_Speed;
	//�Ƕ�
	M_Tx_Data[9]=Angle>>24;
	M_Tx_Data[10]=Angle>>16;
	M_Tx_Data[11]=Angle>>8;
	M_Tx_Data[12]=Angle;
	//�Ƕ�ģʽ
	M_Tx_Data[13]=Angle_mode;
	//ͬ����־λ
	M_Tx_Data[14]=Synchronous_Motion;
	//У���ֽ�
	M_Tx_Data[15]=0x6B;
//	HAL_UART_Transmit_DMA(&huart4,M_Tx_Data,16);
	send_num=16;
	send_flag=1;
	HAL_Delay(2);
}
/**
 * @brief ֱͨ����λ��ģʽ����
 * @brief ������ٶȴﵽָ��λ��
 * @brief ֡��ʽ ��ַ + 0xFB + ���ţ����� + �ٶ� + λ�ýǶ� + ���λ��/����λ�ñ�־ + ���ͬ����־ + У���ֽ�
 */
void Direct_speed_limit_position(Motor_Address Motor_Address,
								 Motor_Dir Dir,
								 uint16_t Speed,
								 uint32_t Angle,
								 Motor_Angle_Mode Angle_mode,
								 MotorMulti_Motion_Mode Synchronous_Motion)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0xFB;
	//�ٶȷ���
	M_Tx_Data[2]=Dir;
	//�ٶ�
	M_Tx_Data[3]=Speed>>8;
	M_Tx_Data[4]=Speed;
	//�Ƕ�
	M_Tx_Data[5]=Angle>>24;
	M_Tx_Data[6]=Angle>>16;
	M_Tx_Data[7]=Angle>>8;
	M_Tx_Data[8]=Angle;
	//�Ƕ�ģʽ
	M_Tx_Data[9]=Angle_mode;
	//ͬ����־λ
	M_Tx_Data[10]=Synchronous_Motion;
	//У���ֽ�
	M_Tx_Data[11]=0x6B;
	send_num=12;
	send_flag=1;
	HAL_Delay(2);
}


/**
 * @brief �����ٶȣ��ջ�
 * @brief ֡��ʽ ��ַ + 0xF6 + ���� + �ٶ�б�� + �ٶ� + ���ͬ����־ + У���ֽ�
 */
void Set_Speed(Motor_Address Motor_Address,Motor_Dir Dir,uint16_t Acceleration,uint16_t Target_Speed,MotorMulti_Motion_Mode Synchronous_Motion)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0xF6;
	//�ٶȷ���
	M_Tx_Data[2]=Dir;
	//���ٶ�
	M_Tx_Data[3]=Acceleration>>8;
	M_Tx_Data[4]=Acceleration;
	//�ٶ�
	M_Tx_Data[5]=Target_Speed>>8;
	M_Tx_Data[6]=Target_Speed;
	//ͬ����־λ
	M_Tx_Data[7]=Synchronous_Motion;
	//У���ֽ�
	M_Tx_Data[8]=0x6B;
	send_num=9;
	send_flag=1;
	HAL_Delay(2);
}

/**
 * @brief ����ǰ��λ�ýǶ�����
 */
void Clear_AngleCounter(Motor_Address Motor_Address)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0x0A;
	//�����ֽ�
	M_Tx_Data[2]=0x6D;
	//У���ֽ�
	M_Tx_Data[3]=0X6B;
	send_num=4;
	send_flag=1;
	HAL_Delay(2);
}

/**
 * @brief ����ģʽ
 * @brief ֡��ʽ ����ַ + 0xF5 + ���� + ����б�� + ���� + ���ͬ����־ + У���ֽ�
 */
void Torque_Mode_Ctrl(Motor_Address Motor_Address,
					  Motor_Dir Dir,
					  uint16_t Torque_Acc,
					  uint16_t Torque_target,
					  MotorMulti_Motion_Mode Synchronous_Motion)
{
	//��ַ
	M_Tx_Data[0]=Motor_Address;	
	//�����ֽ�
	M_Tx_Data[1]=0xF5 ;
	//�ٶȷ���
	M_Tx_Data[2]=Dir;
	//���ٶ�
	M_Tx_Data[3]=Torque_Acc>>8;
	M_Tx_Data[4]=Torque_Acc;
	//�ٶ�
	M_Tx_Data[5]=Torque_target>>8;
	M_Tx_Data[6]=Torque_target;
	//ͬ����־λ
	M_Tx_Data[7]=Synchronous_Motion;
	//У���ֽ�
	M_Tx_Data[8]=0x6B;
	send_num=9;
	send_flag=1;
	HAL_Delay(2);
}
#endif
