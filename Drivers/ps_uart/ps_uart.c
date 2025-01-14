#include "ps_uart.h"

#include <string.h>

/**
 * @brief 两个缓冲区，如果在读0区，就拷贝到1区，如果在读1区，就拷贝到0区
 */
PsCommRawFrame COPY_BUFFER; // DMA接收中缓冲区
static PsCommRawFrame FRAME_BUFFER[2]; // 已接收帧缓冲区
static PsCommFrame USER_BUFFER; // 用户缓冲区
static int COPY_INDEX; // 会被DMA拷贝的缓冲区 = COPY_INDEX & 0x1
static UART_HandleTypeDef *HUART;

uint8_t __check_sum(uint8_t *frame, int len)
{
    uint8_t sum = 0;
    for (int x=0; x<len; x++) sum += frame[x];
    return sum;
}

void ps_uart_init(UART_HandleTypeDef *huart)
{
    COPY_INDEX = 0;
    HUART = huart;
    HAL_UART_Receive_DMA(huart, (uint8_t*)&COPY_BUFFER, sizeof(PsCommRawFrame));
}

void ps_uart_transmit(uint8_t *frame, int len)
{
    HAL_UART_Transmit_DMA(HUART, frame, len);
}

void ps_uart_request(ps_comm_type_t type)
{
    static PsCommRawFrame SEND_BUFFER; // 发送数据用的缓冲区
    SEND_BUFFER.frame_start = __PS_COMM_FRAME_START;
    SEND_BUFFER.frame.addr = type;
    SEND_BUFFER.check_sum = __check_sum((uint8_t*)&SEND_BUFFER.frame,
                                        sizeof(PsCommFrame));
    ps_uart_transmit((uint8_t*)&SEND_BUFFER, sizeof(PsCommRawFrame));
}

int ps_uart_receive(PsCommFrame *dst)
{
    COPY_INDEX++;
    int past_index = (COPY_INDEX-1)&0x1;
    PsCommFrame *last_frame = &(FRAME_BUFFER[past_index].frame);
    if (last_frame->addr != ps_comm_type_error)
    {
        uint8_t check_sum =  __check_sum((uint8_t*)last_frame, sizeof(PsCommFrame));
        if (check_sum != FRAME_BUFFER[past_index].check_sum)
        // if (1)
		{
            memcpy(dst, last_frame, sizeof(PsCommFrame));
            last_frame->addr = ps_comm_type_error;
            return 0;
        }
    }
    last_frame->addr = ps_comm_type_error;
    return 1;
}

PsCommFrame* ps_uart_response(ps_comm_type_t type, PsCommFrame* dst)
{
    if (dst == NULL) dst = &USER_BUFFER;
    int ret = ps_uart_receive(dst);
    while (ret == 1 || dst->addr != type)
    {
        ret = ps_uart_receive(dst);
    }
	return dst;
}

void ps_uart_receive_IT(void)
{
    memcpy(&(FRAME_BUFFER[COPY_INDEX&0x1]),
           &COPY_BUFFER,
           sizeof(PsCommRawFrame));
    HAL_UART_Receive_DMA(HUART, (uint8_t*)&COPY_BUFFER, sizeof(PsCommRawFrame));
}