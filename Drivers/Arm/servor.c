#include "servor.h"


/*----------------------舵机参数部分-------------------------*/
const static float SERVOR_ANGLE = 180;
static UART_HandleTypeDef *HUART;

// position=500+2000*Angle/180
static inline uint16_t angle_to_position(float angle)
{
    return 500 + 2000 * angle / SERVOR_ANGLE;
}

/*----------------------舵机控制部分---------------------*/
void servor_init(UART_HandleTypeDef *pHUART)
{
    HUART = pHUART;
}

// 单个舵机控制包
uint8_t Servor_Packet[10] = {0x55, 0x55, 0x08, 0x03, 0x01, 0, 0, 0, 0, 0};
// 5——时间低八位		6——时间高八位
// 7——舵机ID
// 8——位置低八位		9——位置高八位
HAL_StatusTypeDef servor_rotate(uint16_t duration, uint8_t addr, float angle)
{
    Servor_Packet[5] = duration & 0xFF;
    Servor_Packet[6] = duration >> 8;

    uint16_t position = angle_to_position(angle);
    Servor_Packet[7] = addr;
    Servor_Packet[8] = position & 0xFF;
    Servor_Packet[9] = position >> 8;
    return HAL_UART_Transmit_DMA(HUART, Servor_Packet, sizeof(Servor_Packet));
}

// 两个舵机控制包
uint8_t Servor_Packet_2[13] = {0x55, 0x55, 0x0B, 0x03, 0x02, 0, 0, 0, 0, 0, 0, 0};
// 10——舵机ID
// 11——位置低八位	12——位置高八位
HAL_StatusTypeDef servor_rotate_2(uint16_t duration, uint8_t addr1, float angle1,
                                  uint8_t addr2, float angle2)
{
    servor_rotate(duration, addr1, angle1);
    HAL_Delay(10);
    return servor_rotate(duration, addr2, angle2);
    // Servor_Packet[5] = duration & 0xFF;
    // Servor_Packet[6] = duration >> 8;

    // uint16_t position = angle_to_position(angle1);
    // Servor_Packet[7] = addr1;
    // Servor_Packet[8] = position & 0xFF;
    // Servor_Packet[9] = position >> 8;

    // position = angle_to_position(angle2);
    // Servor_Packet[7] = addr2;
    // Servor_Packet[8] = position & 0xFF;
    // Servor_Packet[9] = position >> 8;
    // return HAL_UART_Transmit_DMA(HUART, Servor_Packet, sizeof(Servor_Packet));
}

//__HAL_TIM_SetCompare(&htim, Channel, (uint16_t)pwm_duty_pulse);

