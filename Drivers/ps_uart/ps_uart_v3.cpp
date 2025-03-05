#include "ps_uart_v3.h"
#include <cstring>

using namespace ps;

/**
 * @note 一些定义：
 * DMA：DMA中断的视角，从中断中看待程序
 * busy_buffer：即buffer[2]
 *
 * @note 三个缓冲区的含义？
 * buffer[0]: 用户正在访问的缓冲区
 * buffer[1]: 存储了最新数据的缓冲区
 * buffer[2]: DMA正在使用的缓冲区
 * 
 * @note 如何获取当前缓冲区已有数据长度？
 * 缓冲区第一个字节储存了数据长度
 * 
 * @note 如何保证三个缓冲区操作的互斥性？
 * 1. 只有用户能控制读取buffer[0]还是buffer[1]：用户控制read_zero标志
 * 2. 中断能够交换不用的那个缓冲区，中断通过read_zero标志来交换缓冲区
 */

UART::UART(UART_HandleTypeDef *pHUART, uint8_t *(buffer[3]), uint8_t recv_max_length,
           uint32_t poll_interval)
    : pHUART {pHUART}, recv_max_len {recv_max_length}, poll_interval {poll_interval}
    , read_zero {false}
{
    for (int x=0; x<3; x++)
    {
        this->buffer[x] = new UART::Buffer {0, buffer[x]};
    }
}

HAL_StatusTypeDef UART::transmit_nowait(const uint8_t *data, uint16_t len) const
{
    return HAL_UART_Transmit_DMA(pHUART, data, len);
}

void UART::transmit(const uint8_t *data, uint16_t len) const
{
    HAL_StatusTypeDef status = transmit_nowait(data, len);
    while (status != HAL_OK)
    {
        HAL_Delay(poll_interval);
        status = transmit_nowait(data, len);
    }
}

const void* UART::get_nowait(int *length)
{
    read_zero = !read_zero; // 读取另一个内存，具体原理见文件开头的“如何保证互斥”
    auto ptr = buffer[read_zero ? 0 : 1];
    if (length) *length = ptr->length;
    if (ptr->length > 0) // 确实收到了数据
    {
        ptr->length = 0;
        return ptr->ptr;
    }
    return NULL;
}

const void* UART::get(int *length)
{
    const void* ret;
    do
    {
        ret = get_nowait(length);
        HAL_Delay(poll_interval);
    } while (ret);
    return ret;
}

void UART::validate_swap(uint8_t length)
{
    if (validate == NULL || validate(*this, buffer[2]->ptr, length)) // 验证成功
    {
#       ifdef __PS_UART_INDICATOR
            accept_sum++;
#       endif
        if (receive_cb) receive_cb(*this, buffer[2]->ptr, length); // 先交给用户使用，再交给get()使用
        buffer[2]->length = length; // 进入DMA中断了就说明收满了
        // 交换两个缓冲区
        auto& swap_bf = buffer[read_zero ? 1 : 0], tmp = swap_bf;
        swap_bf = buffer[2];
        buffer[2] = tmp;
    }
}

void UART::receive_DMA_IT()
{
    // 确保是接收中断
    if (pHUART->hdmarx->State != HAL_DMA_STATE_READY) return;

#   ifdef __PS_UART_INDICATOR
    recv_DMA_sum++;
#   endif
    // 验证并处理数据
    validate_swap(recv_max_len);

    // 开始下一轮接收
    HAL_UARTEx_ReceiveToIdle_DMA(pHUART, buffer[2]->ptr, recv_max_len);
}

void UART::receive_IDLE_IT()
{
    // 确保是空闲中断
    if ((READ_REG(pHUART->Instance->ISR) & USART_ISR_IDLE) == 0) return;
    uint8_t recv_len = recv_max_len - __HAL_DMA_GET_COUNTER(pHUART->hdmarx);
    // 如果没有收到，当然不处理
    // 如果接收缓冲区满了，交给DMA中断一起处理即可
    if (recv_len == 0 || recv_len == recv_max_len) return;
    
    // 验证并处理数据
#   ifdef __PS_UART_INDICATOR
    recv_IDLE_sum++;
#   endif
    validate_swap(recv_len);

    // 开始下一轮
    HAL_UART_DMAStop(pHUART);
    HAL_UARTEx_ReceiveToIdle_DMA(pHUART, buffer[2]->ptr, recv_max_len);
}