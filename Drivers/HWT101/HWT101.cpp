#include "HWT101.h"
#include "HWT101_protocol.h"
#include <math.h>

const static int HWT101_HEADER = 0x55;
const static int CHECK_SUM_LENGTH = 10;
const static uint8_t CMD_SET_HZ[5] = {0xFF, 0xAA, 0x03, 0x0C, 0x00};
const static uint8_t CMD_SET_BAUD[5] = {0xFF, 0xAA, 0x04, 0x07, 0x00};
const static uint8_t CMD_CALIYAW[5] = {0xFF, 0xAA, 0x76, 0x00, 0x00};

/**
 * @note 关于为什么是两个数据包：
 * 陀螺仪是一口气发两个数据包的
 * 使用串口空闲中断处理数据接收时，如果只接收一个数据包，另一个数据包会直接溢出
 * @note 气死我了，两天才发现这个简单的问题，一直接收不完整
 */
HWT101_protocol_t HWT101_BF0[2];
HWT101_protocol_t HWT101_BF1[2];
HWT101_protocol_t HWT101_BF2[2];
uint8_t *(HWT101_BUFFER[3]) = {(uint8_t*)HWT101_BF0,
                               (uint8_t*)HWT101_BF1,
                               (uint8_t*)HWT101_BF2};

ps::UART *HWT101_uart_handle;
volatile HWT101DataStruct HWT101Data;

bool HWT101_validate(ps::UART& self, const uint8_t *data, uint8_t length);
void HWT101_receive_cb(ps::UART& self, const uint8_t *data, uint8_t lenght);

void HWT101_init(UART_HandleTypeDef *pHUART)
{
    // 初始化数据
    HWT101Data.YawZ = 0;
    HWT101Data.Yaw_calib = 0;
    HWT101Data.WY = 0;
    HWT101Data.WZ = 0;
#	ifdef __PS_UART_INDICATOR
		HWT101Data.accept_sum = 0;
#	endif

    // 初始化通信
    HWT101_uart_handle = new ps::UART {pHUART, HWT101_BUFFER,
                                       sizeof(HWT101_protocol_t)*2};
    HWT101_uart_handle->validate = HWT101_validate;
    HWT101_uart_handle->receive_cb = HWT101_receive_cb;
    HWT101_uart_handle->start();

    // 归零
    HWT101_yaw_reset();
    // 设置波特率
    // ps_uart_transmit(HWT101_uart_handle, CMD_SET_BAUD, 5);
    // 设置速率
    // ps_uart_transmit(HWT101_uart_handle, CMD_SET_HZ, 5);
}

bool HWT101_validate(ps::UART& self, const uint8_t *data, uint8_t length)
{
    auto bf = (const HWT101_protocol_t*)data;
    // 验证长度，应该等于两个数据包
    if (length != sizeof(HWT101_protocol_t)*2) return false;

    // 验证包头
    if (bf[0].header != HWT101_HEADER) return false;
    if (bf[1].header != HWT101_HEADER) return false;

    // 验证校验和
    uint8_t check_sum = ps::sigma_check_sum((uint8_t*)(bf),
                                            CHECK_SUM_LENGTH);
    if (check_sum != bf[0].checksum) return false;
    check_sum = ps::sigma_check_sum((uint8_t*)(bf+1),
                                    CHECK_SUM_LENGTH);
    if (check_sum != bf[1].checksum) return false;

    return true;
}

void HWT101_receive_cb(ps::UART& self, const uint8_t *data, uint8_t lenght)
{
#   ifdef __PS_UART_INDICATOR
        HWT101Data.accept_sum++;
#   endif

    const HWT101_protocol_t *bf = (const HWT101_protocol_t*)data;
    for (int index=0; index<2; index++)
    {
        float tmp;
        // 储存数据
        switch (bf[index].type)
        {
        case HWT101_cmd_W:
            HWT101Data.WY = ((float)bf[index].W.WY)/32768*2000;
            HWT101Data.WZ = ((float)bf[index].W.WZ)/32768*2000;
            break;
        case HWT101_cmd_Yaw:
                tmp = ((float)bf[index].yaw.yaw)/32768*180;
                HWT101Data.YawZ = fmodf(tmp, 180.); // 控制在-180~180
            break;
        }
    }
}

void HWT101_calibrate(float dst)
{
    HWT101Data.Yaw_calib = angle_normal(dst - HWT101Data.YawZ);
}

void HWT101_yaw_reset(void)
{
    HWT101_uart_handle->transmit(CMD_CALIYAW, sizeof(CMD_CALIYAW));
}

float HWT101_read_yaw(void)
{
	float angle = HWT101Data.YawZ + HWT101Data.Yaw_calib;
    if (angle > 180) return angle - 360;
    else if (angle < -180) return 360 + angle;
    return angle;
}