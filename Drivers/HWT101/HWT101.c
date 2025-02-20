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
HWT101_protocol_t HWT101_COPY_BUFFER0[2];
HWT101_protocol_t HWT101_COPY_BUFFER1[2];
HWT101_protocol_t HWT101_BUSY_BUFFER[2];

volatile HWT101DataStruct HWT101Data;
ps_uart_t HWT101_uart;
ps_uart_handle_t HWT101_uart_handle;

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
    HWT101_uart.buffer[0] = &HWT101_COPY_BUFFER0[0];
    HWT101_uart.buffer[1] = &HWT101_COPY_BUFFER1[0];
    HWT101_uart.buffer[2] = &HWT101_BUSY_BUFFER[0];
    HWT101_uart.recv_len = sizeof(HWT101_protocol_t)*2;
    HWT101_uart.pHUART = pHUART;
    HWT101_uart.recv_callback = NULL;
    HWT101_uart_handle = &HWT101_uart;
    ps_uart_init(&HWT101_uart);

    // 归零
    HWT101_yaw_reset();
    // 设置波特率
    // ps_uart_transmit(HWT101_uart_handle, CMD_SET_BAUD, 5);
    // 设置速率
    // ps_uart_transmit(HWT101_uart_handle, CMD_SET_HZ, 5);
}

void HWT101_calibrate(float dst)
{
    HWT101Data.Yaw_calib = angle_normal(dst - HWT101Data.YawZ);
}

bool HWT101_update_nowait(void)
{
    bool status = false;
    HWT101_protocol_t *bf = (HWT101_protocol_t*)ps_uart_acquire(HWT101_uart_handle);
    if (bf)
    {
        bool accept = true; // 上一个数据包有效
        // 检测包头
        for (int index=0; index<2 && accept; index++)
        {
            if (bf[index].header != HWT101_HEADER)
            {
                accept = false;
                continue;
            }
            uint8_t check_sum = ps_uart_sigma_check_sum((uint8_t*)(bf+index),
                                                        CHECK_SUM_LENGTH);
            if (check_sum != bf[index].checksum)
            {
                accept = false;
                continue;
            }

#	        ifdef __PS_UART_INDICATOR
		        HWT101Data.accept_sum++;
#	        endif

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
        status = accept;
    }

    ps_uart_release(HWT101_uart_handle);
    return status;
}

void HWT101_update(void)
{
    bool ret = HWT101_update_nowait();
    while (!ret) ret = HWT101_update_nowait();
}

void HWT101_yaw_reset(void)
{
    ps_uart_transmit(HWT101_uart_handle, CMD_CALIYAW, 5);
}

float HWT101_read_yaw(void)
{
	float angle = HWT101Data.YawZ + HWT101Data.Yaw_calib;
    if (angle > 180) return angle - 360;
    else if (angle < -180) return 360 + angle;
    return angle;
}