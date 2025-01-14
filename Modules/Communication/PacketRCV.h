/*
 * @Author: 21-Ӧ��-��� 1928684748@qq.com
 * @Date: 2023-10-28 15:37:42
 * @LastEditors: jianming 1928684748@qq.com
 * @LastEditTime: 2024-04-29 23:48:07
 * @FilePath: \MDK-ARMe:\������\Դ��\����\���ִ���\Communication\PacketRCV.h
 * @Description: 
 * 
 * Copyright (c) 2024 by jianming, All Rights Reserved. 
 */
#ifndef __PACKETRCV_H__
#define __PACKETRCV_H__

#include "stm32h7xx_hal.h"

typedef struct
{
	uint8_t Mode;
	uint8_t Head[4];
	uint8_t Tail[4];
	uint8_t counter;
	uint8_t HeadLenth;
	uint8_t TailLenth;
	uint8_t RxPacketIndex;
	uint8_t RxState;
	uint8_t PacketUnpacketFlag;
	uint8_t RxData;

	uint8_t Packet[10];
	uint8_t PacketLenth;

} RxPacketTypedef;
enum RxPacketMode
{
	Head=0,
	HeadAandTail=1,
};

void PacketStructInit(RxPacketTypedef *rxPacketStruct,uint8_t packetLenth,uint8_t mode,uint8_t headLenth,uint8_t tailLenth);
void Packet_Receive(RxPacketTypedef *rxPacketStruct, uint8_t rxData);

#endif 

