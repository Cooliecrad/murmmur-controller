#ifndef PS_2_LEA
#define PS_2_LEA

#include "stm32h7xx_hal.h"
#include <stdint.h>

static uint8_t PS2_ReadWriteData(uint8_t cmd);
void PS2OriginalValueGet(void);
uint8_t Deter_Press(void);
void ButtonValueGet(void);
void RockerValueGet(void);
void PS2OriginalValueClear(void);
void PS2AllValueUpdate(void);
uint8_t Work_Green(void);
uint8_t Work_Red(void);
uint8_t Start(void);
uint8_t Select(void);
uint8_t lJoy_Left(void);
uint8_t lJoy_Right(void);
uint8_t lJoy_Up(void);
uint8_t lJoy_Down(void);
uint8_t lJoy_Press(void);
uint8_t rJoy_Left(void);
uint8_t rJoy_Right(void);
uint8_t rJoy_Up(void);
uint8_t rJoy_Down(void);
uint8_t rJoy_Press(void);
uint8_t Dir_Left(void);
uint8_t Dir_Right(void);
uint8_t Dir_Up(void);
uint8_t Dir_Down(void);
uint8_t Fun_Left(void);
uint8_t Fun_Right(void);
uint8_t Fun_Up(void);
uint8_t Fun_Down(void);
uint8_t lSho_Up(void);
uint8_t lSho_Down(void);
uint8_t rSho_Up(void);
uint8_t rSho_Down(void);



#endif
