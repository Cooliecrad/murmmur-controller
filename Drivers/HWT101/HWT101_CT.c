/*
 * @Author: 21-Ӧ��-��� 1928684748@qq.com
 * @Date: 2023-10-29 09:54:01
 * @LastEditors: jianming 1928684748@qq.com
 * @LastEditTime: 2024-04-29 22:47:36
 * @FilePath: \MDK-ARMe:\������\Դ��\����\���ִ���\Module\HWT101_CT.c
 * @Description:
 *
 * Copyright (c) 2024 by jianming, All Rights Reserved.
 */
#include "chassis.h"
#include "HWT101_CT.h"
#include "usart.h"

HWT101DataStruct HWT101Data;

uint8_t YawZeroCommandArry[5] = {0XFF, 0XAA, 0X76, 0, 0};
uint8_t YawSetZeroOffsetCommandArry[5] = {0XFF, 0XAA, 0X48, 02, 0};

float FirstAngle;
uint8_t FirstAngleFlag = 1;

uint8_t tempflag3;
void HWT101_DataUnpacket(RxPacketTypedef *hwt101Packet)
{
	uint8_t sum;
	if (hwt101Packet->PacketUnpacketFlag)
	{
		if (hwt101Packet->Packet[0] == 0X53)
		{
			sum = 0x55 + 0x53 + hwt101Packet->Packet[5] + hwt101Packet->Packet[6] + hwt101Packet->Packet[7] + hwt101Packet->Packet[8];

			if (hwt101Packet->Packet[9] == sum)
			{
				HWT101Data.Yaw = (float)((hwt101Packet->Packet[6] << 8) | hwt101Packet->Packet[5]) / 32768.0 * 180.0;
				if (FirstAngleFlag == 1)
				{
					FirstAngle = HWT101Data.Yaw;
					FirstAngleFlag = 0;
				}
				Chassis.coord.Angle.AngleNow = HWT101Data.Yaw - FirstAngle;
                if(Chassis.coord.Angle.AngleNow<-180)
                {
                    Chassis.coord.Angle.AngleNow=(Chassis.coord.Angle.AngleNow+360);
                }
			}
		}

		hwt101Packet->PacketUnpacketFlag = 0;
	}
}

void HWT101_YawSetZero(void)
{
	for (uint8_t i = 0; i < 10; i++)
		for (uint8_t i = 0; i < 5; i++)
			HAL_UART_Transmit(&huart3, &YawZeroCommandArry[i], 1, 1000);
}

void HWT101_SetZeroOffSet(void)
{
	for (uint8_t i = 0; i < 5; i++)
		for (uint8_t i = 0; i < 5; i++)
			HAL_UART_Transmit(&huart3, &YawSetZeroOffsetCommandArry[i], 1, 1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);

	YawSetZeroOffsetCommandArry[3] = 0;
	for (uint8_t i = 0; i < 5; i++)
		for (uint8_t i = 0; i < 5; i++)
			HAL_UART_Transmit(&huart3, &YawSetZeroOffsetCommandArry[i], 1, 1000);
	YawSetZeroOffsetCommandArry[3] = 0x02;
}
