/*
 * @Author: 21-Ӧ��-��� 1928684748@qq.com
 * @Date: 2023-10-28 15:37:42
 * @LastEditors: jianming 1928684748@qq.com
 * @LastEditTime: 2024-04-29 23:47:32
 * @FilePath: \MDK-ARMe:\������\Դ��\����\���ִ���\Communication\Communication.h
 * @Description: 
 * 
 * Copyright (c) 2024 by jianming, All Rights Reserved. 
 */
#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <math.h>
#include "common.h"

#define printfUART USART1

extern RxPacketTypedef	stepmotor_rxpacket;
extern RxPacketTypedef stepmotor_rxpacket2;

#ifdef __cplusplus
extern "C" {
#endif

void Communication_PacketInit(void);
void Communication_Start(void);

#ifdef __cplusplus
}
#endif


#endif
