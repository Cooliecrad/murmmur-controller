/*
 * @Author: 21-Ӧ��-��� 1928684748@qq.com
 * @Date: 2023-11-07 22:26:25
 * @LastEditors: jianming 1928684748@qq.com
 * @LastEditTime: 2024-04-29 23:47:01
 * @FilePath: \MDK-ARMe:\������\Դ��\����\���ִ���\Communication\Communication.c
 * @Description: 
 * 
 * Copyright (c) 2024 by jianming, All Rights Reserved. 
 */
#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "Communication.h"
#include "stm32h7xx_hal_dma.h"
#include "StepMotorGPIO.h"

uint8_t UART1_RxData;
// uint8_t UART2_RxData;
uint8_t UART3_RxData;
uint8_t UART4_RxData;
// uint8_t UART8_RxData[4];

//RxPacketTypedef LanYaPacket;
// RxPacketTypedef HWT101_RxPacket;
RxPacketTypedef	stepmotor_rxpacket;
RxPacketTypedef stepmotor_rxpacket2;
//RxPacketTypedef VisionPacket;
//RxPacketTypedef QR_CodePacket;
//RxPacketTypedef ColorPacket;
//RxPacketTypedef ErroPacket;ui
//uint8_t Stepreceivedata[4];


//RxPacketTypedef TestVisionPackt;

// void UART_Test(RxPacketTypedef* packet)
// {
// 	if(packet->PacketUnpacketFlag)
// 	{
// 		printf("UART1:");

// 		for(uint8_t i=0;i<packet->PacketLenth;i++)
// 		{
// 			printf("%x",packet->Packet[i]);
// 			printf("	");
// 		}
// 		printf("\r\n");	
// 	}

// }

void Communication_Start(void)
{

	// HAL_UART_Receive_IT(&huart3,&UART3_RxData, 1);
	// HAL_UART_Receive_IT(&huart2,&UART2_RxData,1);
	HAL_UART_Receive_IT(&huart4,&UART4_RxData, 1);
	// HAL_UART_Receive_IT(&huart8,UART8_RxData, 4);
	
}
void Communication_PacketInit(void)
{
	
//	LanYaPacket.Head[0]=0XA5;
	// HWT101_RxPacket.Head[0]=0X55;
//	QR_CodePacket.Head[0]=0X51;
	// stepmotor_rxpacket.Head[0]=0x03;
	// stepmotor_rxpacket.Head[1]=0xFD;
	// stepmotor_rxpacket2.Head[0]=0x04;
	// stepmotor_rxpacket2.Head[1]=0xFD;
	// stepmotor_rxpacket.Tail[0]=0x6B;
	// stepmotor_rxpacket2.Tail[0]=0x6B;
	
	
	// PacketStructInit(&HWT101_RxPacket,10,0,1,0);
	
	// PacketStructInit(&stepmotor_rxpacket,1,1,2,1);
	// PacketStructInit(&stepmotor_rxpacket2,1,1,2,1);


}
void UART1_PacketUnpacket(void)
{

}

void UART2_PacketUnpacket(void)
{

}

void UART3_PacketUnpacket(void)
{
	
}

void UART4_PacketUnpacket(void)
{
//	if(UART4_RxPacket.PacketUnpacketFlag)
//	{


//	}
}

void UART4_RxCallback(UART_HandleTypeDef *huart)
{
if(huart==&huart4)
{

}
}

// void UART8_RxCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart==&huart8)
// 	{
// 		if(UART8_RxData[1]==0xFD && UART8_RxData[2]==0x9F && UART8_RxData[3]==0x6B)
// 		{
// 			if(UART8_RxData[0]==1)
// 			{
// 				Motor_R.Step_Finish_Flag=1;
// 			}
// 			if(UART8_RxData[0]==2)
// 			{
// 				Motor_Z.Step_Finish_Flag=1;
// 			}
// 			if(UART8_RxData[0]==3)
// 			{
// 				Motor_X.Step_Finish_Flag=1;
// 			}
// 			memset(UART8_RxData, 0, sizeof(UART8_RxData));
// 		}
// 		HAL_UART_Receive_IT(&huart8,UART8_RxData, 4);
// }
// }

//5������ư�01 45
//6��ݮ��

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
	
//	UART1_RxCallback(huart);
	// UART3_RxCallback(huart);
	UART4_RxCallback(huart);
	// UART8_RxCallback(huart);
	Communication_Start();
}



//HAL_DMA_CallbackIDTypeDef


/* �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)           /* ʹ��AC6������ʱ */
__asm(".global __use_no_semihosting\n\t"); /* ������ʹ�ð�����ģʽ */
__asm(".global __ARM_use_no_argv \n\t");   /* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */

#else
/* ʹ��AC5������ʱ, Ҫ�����ﶨ��__FILE �� ��ʹ�ð�����ģʽ */
#pragma import(__use_no_semihosting)

struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

#endif

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
int _ttywrch(int ch)
{
	
  ch = ch;
  return ch;
	
}


/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
  x = x;
}

char *_sys_command_string(char *cmd, int len)
{
  return NULL;
}

/* FILE �� stdio.h���涨��. */
FILE __stdout;

/* �ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
int fput(const char *stream, FILE *fp)
{
//	if (fp == stdout)
//	{
//		HAL_UART_Transmit_DMA(&huart1, stream, sizeof(stream));
//	}
//   while ((printfUART->ISR & 0X40) == 0)
//     ; /* �ȴ���һ���ַ�������� */

//   printfUART->TDR = (uint8_t)ch; /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
//   return ch;
    return 0;
}
#endif
