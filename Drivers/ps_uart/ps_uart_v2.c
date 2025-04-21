#include "ps_uart_v2.h"
#include "stm32h750xx.h"
#include <string.h>

/**
 * @note 一些定义：
 * DMA：DMA中断的视角，从中断中看待程序
 * busy_buffer：即buffer[2]
 * 
 * @note 如何保证用户和DMA不会使用同一个缓冲区？
 * index 指示DMA当前正在使用的缓冲区，用户访问 (index+1)&0x1 即可
 * 
 * @note 如何保证index不会出现抢夺现象
 * 一般情况下下，只有DMA才有index的修改权，由于DMA是中断期间工作的，所以DMA对index的
 * 修改是安全的。此时DMA就主导了接收缓冲区的位置
 * 
 * @note 如何保证index不会在用户使用缓冲区期间被更改？
 * 用户访问缓冲区时，需要置busy标志位。当busy为真，DMA将不会修改index，所有信息会在
 * 目前index指向的缓冲区被接收
 * 
 * @note 如果busy位一直被置位，如何访问最新收到的数据？
 * 当busy == 1，DMA不会接收数据到index指定的buffer，而是使用busy_buffer，当完整接收
 * 到一帧数据，才会将数据交换到index指定的buffer。
 * 从用户的视角来看，index指定的buffer中要么没有数据，要么有完整的数据。
 * 
 * 当用户连续在一整个接受周期占用busy位（recv_busy == 1 && busy == 1），可以认为
 * 期间有可能发生了上述交换行为，并且busy置位时，DMA不会修改index，用户可以在此时修改
 * index，使其指向刚刚有可能接收到数据的缓冲区。
 */

/**
 * @addindex 单次轮询的间隔时间
 */
static const uint32_t POLL_INTERVAL = 0;


void ps_uart_init(ps_uart_handle_t handle)
{
    ps_uart_instance_t *instance = &handle->instance;
    instance->copy_length[0] = 0;
    instance->copy_length[1] = 0;
    instance->index = 0;
    instance->busy = false;
    instance->recv_busy = false;

#   ifdef __PS_UART_INDICATOR
        instance->accept_sum = 0;
        instance->adjust = 0;
#   endif
    HAL_UARTEx_ReceiveToIdle_DMA(handle->pHUART, handle->buffer[0], handle->recv_len);
}

void ps_uart_transmit(ps_uart_handle_t handle, const void *pData, int len)
{
    HAL_StatusTypeDef status = ps_uart_transmit_nowait(handle, pData, len);
    while (status != HAL_OK)
    {
        HAL_Delay(POLL_INTERVAL);
        status = ps_uart_transmit_nowait(handle, pData, len);
    }
}

HAL_StatusTypeDef ps_uart_transmit_nowait(ps_uart_handle_t handle,
                                          const void *pData, int len)
{
    return HAL_UART_Transmit_DMA(handle->pHUART, pData, len);
}

bool ps_uart_get_nowait(ps_uart_handle_t handle, void *dst)
{
    void *bf = ps_uart_acquire(handle);
    bool status = false;
    if (bf)
    {
        memcpy(dst, bf, handle->recv_len);
        status = true;
    }
    ps_uart_release(handle);
    return status;
}

void ps_uart_get(ps_uart_handle_t handle, void* dst)
{
    bool recv = ps_uart_get_nowait(handle, dst);
    while (!recv)
    {
        HAL_Delay(POLL_INTERVAL);
        recv = ps_uart_get_nowait(handle, dst);
    }
}

void* ps_uart_acquire(ps_uart_handle_t handle)
{
    handle->instance.busy = true; // 设置为忙状态
    // 忙状态下，可以保证recv_busy不会从true变成false
    if (handle->instance.recv_busy)
    { // 如果recv_busy，意味着另一个缓冲区有完整的数据可以读取
        handle->instance.index++;
    }
    // 可以读取copy_index了，因为此时copy_index不会改变
    uint8_t index = (handle->instance.index+1) & 0x1;
    if (handle->instance.copy_length[index] > 0)
        return handle->buffer[index];

    return NULL;
}

void ps_uart_receive_IDLE_IT(ps_uart_handle_t handle)
{
    if ((READ_REG(handle->pHUART->Instance->ISR) & USART_ISR_IDLE) == 0) return;
    
    if (__HAL_DMA_GET_COUNTER(handle->pHUART->hdmarx) != handle->recv_len)
    {
#       ifdef __PS_UART_INDICATOR
            handle->instance.adjust++;
#       endif
        HAL_UART_DMAStop(handle->pHUART);
        /**
         * @brief 空闲中断直接丢弃重收
         */
        if (handle->instance.busy)
        {
            handle->instance.recv_busy = true;
            HAL_UARTEx_ReceiveToIdle_DMA(handle->pHUART, handle->buffer[2], handle->recv_len);
        } else
        {
            handle->instance.recv_busy = false;
            if ((handle->instance.index & 0x1) == 0)
                HAL_UARTEx_ReceiveToIdle_DMA(handle->pHUART, handle->buffer[0], handle->recv_len);
            else
                HAL_UARTEx_ReceiveToIdle_DMA(handle->pHUART, handle->buffer[1], handle->recv_len);
        }
    }
}

void ps_uart_receive_DMA_IT(ps_uart_handle_t handle)
{
    if (handle->pHUART->hdmarx->State != HAL_DMA_STATE_READY) return;

#       ifdef __PS_UART_INDICATOR
            handle->instance.accept_sum++;
#       endif

    uint8_t index = handle->instance.index & 0x1; // 用来接收数据的缓冲区
    /**
     * 当recv_busy == true, 说明上一次接收使用的是busy_buffer，需要交换内存
     * copy_buffer来保证约束
     */
    if (handle->instance.recv_busy)
    {
        void *tmp = handle->buffer[index];
        handle->buffer[index] = handle->buffer[2];
        handle->buffer[2] = tmp;
    }

    handle->instance.copy_length[index] = handle->recv_len;

    /**
     * 如果当前处于busy状态，说明copy_index+1正在被访问，不应该增加copy_index，否则
     * 会覆盖用户的内容。因此需要将接收缓冲区 
     */
    if (handle->instance.busy)
    {
        handle->instance.recv_busy = true;
        HAL_UARTEx_ReceiveToIdle_DMA(handle->pHUART,
                                     handle->buffer[2],
                                     handle->recv_len);
    } else
    {
        handle->instance.recv_busy = false;
        handle->instance.index++;
        HAL_UARTEx_ReceiveToIdle_DMA(handle->pHUART,
                                     handle->buffer[handle->instance.index & 0x1],
                                     handle->recv_len);
    }
    // 调用用户的回调
    if (handle->recv_callback) handle->recv_callback();
}