#ifndef __MP3_H
#define __MP3_H

#include "stm32H7xx_hal.h"  // 根据实际使用的STM32系列调整头文件
#include "usart.h"          // 包含UART相关头文件

// UART句柄
extern UART_HandleTypeDef huart4;  // 使用UART4

extern uint8_t volume_grade;	//音量;

// 命令起始码和结束码
#define BY8001_START_CODE 0x7E
#define BY8001_END_CODE   0xEF

// 命令操作码
#define BY8001_CMD_PLAY          0x01  // 播放
#define BY8001_CMD_PAUSE         0x02  // 暂停
#define BY8001_CMD_NEXT          0x03  // 下一曲
#define BY8001_CMD_PREV          0x04  // 上一曲
#define BY8001_CMD_VOL_UP        0x05  // 音量加
#define BY8001_CMD_VOL_DOWN      0x06  // 音量减
#define BY8001_CMD_STOP          0x0E  // 停止
#define BY8001_CMD_SET_VOLUME    0x31  // 设置音量
#define BY8001_CMD_SET_EQ        0x32  // 设置EQ
#define BY8001_CMD_SET_LOOP_MODE 0x33  // 设置循环模式
#define BY8001_CMD_PLAY_TRACK    0x41  // 播放指定曲目

// 函数声明
void BY8001_Play(void);  //开始播放
void BY8001_Pause(void); //暂停播放
void BY8001_Next(void);  //播放下一曲
void BY8001_Prev(void);  //播放上一曲
void BY8001_Stop(void);  //停止播放
void BY8001_SetVolume(uint8_t volume);	//设置音量（0-30）
void BY8001_PlayTrack(uint16_t track);  //播放指定曲目（1-65536）
void BY8001_Loop(uint8_t mood);					//循环指令 0-4(全盘/文件夹/单曲/随机/无循环)
	
#endif  /* __MP3_H */
