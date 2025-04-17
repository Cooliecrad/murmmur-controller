#ifndef __PS_SCREEN_H__
#define __PS_SCREEN_H__

#include "common.h"
#include "stm32h7xx_hal.h"

void screen_init(SPI_HandleTypeDef *pHSPI);

void screen_clear();

void screen_show(const color_t colors[6]);

#endif