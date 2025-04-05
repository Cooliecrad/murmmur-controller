#include "aw_spi.h"

#include "stm32h7xx_hal.h"

SPI_HandleTypeDef *current_pHSPI = NULL;

void aw::delay(int ms)
{
    if (ms > 0) ms -= 1;
    HAL_Delay(ms);
}

void aw::delayMicroseconds(int us)
{
    us /= 1000;
    if (us > 0) us -= 1;
    HAL_Delay(us);
}

void aw::digitalWrite(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_PinState state)
{
    HAL_GPIO_WritePin(GPIOx, pin, state);
}

uint8_t aw::transfer(uint8_t data)
{
    static uint8_t tx_buffer, rx_buffer;
    tx_buffer = data;
    HAL_SPI_TransmitReceive_DMA(current_pHSPI, &tx_buffer, &rx_buffer, sizeof(uint8_t));
    return rx_buffer;
}

void aw::SPIinit(SPI_HandleTypeDef *pHSPI)
{
    current_pHSPI = pHSPI;
}

void aw::SPISettings(uint32_t max_speed, uint8_t firstbit, uint8_t mode)
{
    current_pHSPI->Init.FirstBit = firstbit;
    current_pHSPI->Init.CLKPhase = mode & 0b1;
    current_pHSPI->Init.CLKPolarity = mode & 0b10;
    HAL_SPI_DeInit(current_pHSPI);
    HAL_SPI_Init(current_pHSPI);
}