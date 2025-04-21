#ifndef __ARDUINO_HAL_SPI_WRAPPER_H__
#define __ARDUINO_HAL_SPI_WRAPPER_H__

#include "stm32h7xx_hal.h"
#include "stm32h750xx.h"
#include "stm32h7xx_hal_spi.h"
#include <cstdint>

#define MSBFIRST SPI_FIRSTBIT_MSB
#define LSBFIRST SPI_FIRSTBIT_LSB
#define SPI_MODE3 (0x3)
#define LOW GPIO_PIN_RESET
#define HIGH GPIO_PIN_SET

namespace aw
{

using boolean = bool;

void delay(int ms);

void delayMicroseconds(int us);

void digitalWrite(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_PinState state);

uint8_t transfer(uint8_t data);

void SPIinit(SPI_HandleTypeDef *pHSPI);

void SPISettings(uint32_t max_speed, uint8_t firstbit, uint8_t mode);

};

#endif