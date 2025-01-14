/*
 * @Author: 21-Ӧ��-��� 1928684748@qq.com
 * @Date: 2023-10-28 15:37:51
 * @LastEditors: jianming 1928684748@qq.com
 * @LastEditTime: 2024-04-29 23:32:53
 * @FilePath: \MDK-ARMe:\������\Դ��\����\���ִ���\Module\HWT101_CT.h
 * @Description:
 *
 * Copyright (c) 2024 by jianming, All Rights Reserved.
 */
#ifndef __HWT101_CT_H__
#define __HWT101_CT_H__

#include "PacketRCV.h"

typedef struct
{
	float Yaw;
	float YawInitial;

	float YawAcc;
} HWT101DataStruct;

extern HWT101DataStruct HWT101Data;

#ifdef __cplusplus
extern "C" {
#endif

void HWT101_SetZeroOffSet(void);
void HWT101_YawSetZero(void);
void HWT101_DataUnpacket(RxPacketTypedef *hwt101Packet);

#ifdef __cplusplus
}
#endif

#endif
