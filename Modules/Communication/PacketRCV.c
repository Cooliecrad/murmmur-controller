/*
 * @Author: 21-应物-李建明 1928684748@qq.com
 * @Date: 2023-10-28 15:37:42
 * @LastEditors: jianming 1928684748@qq.com
 * @LastEditTime: 2024-04-29 23:47:35
 * @FilePath: \MDK-ARMe:\社区赛\源码\麦轮\麦轮代码\Communication\PacketRCV.c
 * @Description:
 *
 * Copyright (c) 2024 by jianming, All Rights Reserved.
 */
#include "PacketRCV.h"

void PacketStructInit(RxPacketTypedef *rxPacketStruct, uint8_t packetLenth, uint8_t mode, uint8_t headLenth, uint8_t tailLenth)
{
	rxPacketStruct->PacketLenth = packetLenth;
	rxPacketStruct->Mode = mode;
	rxPacketStruct->HeadLenth = headLenth;
	rxPacketStruct->TailLenth = tailLenth;
}

void Packet_Receive(RxPacketTypedef *rxPacketStruct, uint8_t rxData)
{
	rxPacketStruct->RxData = rxData;
	
	
	
	if (rxPacketStruct->RxState == 0)
	{
		if (rxData == rxPacketStruct->Head[rxPacketStruct->counter])
		{
			rxPacketStruct->counter++;
			if (rxPacketStruct->counter == rxPacketStruct->HeadLenth)
			{
				rxPacketStruct->PacketUnpacketFlag = 0;
				rxPacketStruct->RxState = 1;
				rxPacketStruct->counter = 0;
			}
		}
		else
			rxPacketStruct->counter = 0;
	}

	else if (rxPacketStruct->RxState == 1)
	{
		rxPacketStruct->Packet[rxPacketStruct->RxPacketIndex] = rxData;
		rxPacketStruct->RxPacketIndex++;
		if (rxPacketStruct->RxPacketIndex == rxPacketStruct->PacketLenth)
		{
			if (rxPacketStruct->Mode == HeadAandTail)
			{
				rxPacketStruct->RxState = 2;
			}
			else
				rxPacketStruct->RxState = 3;
		}
	}

	else if (rxPacketStruct->RxState == 2)
	{
		if (rxData == rxPacketStruct->Tail[rxPacketStruct->counter])
		{
			rxPacketStruct->counter++;
			if (rxPacketStruct->counter == rxPacketStruct->TailLenth)
			{
				if (rxPacketStruct->RxPacketIndex == rxPacketStruct->PacketLenth)
					rxPacketStruct->RxState = 3;
				else
					rxPacketStruct->RxState = 4;
			}
		}
		else
			rxPacketStruct->RxState = 4;
	}

	if (rxPacketStruct->RxState == 3)
	{
		rxPacketStruct->RxState = 0;
		rxPacketStruct->PacketUnpacketFlag = 1;
		rxPacketStruct->RxPacketIndex = 0;
		rxPacketStruct->counter = 0;
	}

	else if (rxPacketStruct->RxState == 4)
	{
		rxPacketStruct->RxPacketIndex = 0;
		rxPacketStruct->RxState = 0;
		rxPacketStruct->counter = 0;
	}
}
