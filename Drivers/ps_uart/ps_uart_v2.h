#ifndef __PS_UART_V2_H__
#define __PS_UART_V2_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "stm32h7xx_hal.h"
#include "usart.h"

/**
 * @brief DEBUG使用指示器
 */
// #define __PS_UART_INDICATOR

typedef struct
{
    uint8_t copy_length[2]; // 指示缓冲区接收到的数据长度
    uint8_t index; // 指示DMA当前使用的缓冲区
    bool busy; // 忙标志。当用户访问copy_buffer时，busy置为1
    bool recv_busy; // 接收忙标志。当DMA使用忙缓冲区时，recv_busy置为1
#   ifdef __PS_UART_INDICATOR
        uint32_t accept_sum;
        uint32_t adjust; // 调整标志。当产生了不足长度的数据包接收（串口空闲中断），被置位
#   endif

} ps_uart_instance_t;

/**
 * @brief ps_uart使用的结构体
 * @note 除了instance，其他的成员变量必须用户在初始化前填入
 */
typedef struct
{
    // 三个缓冲区长度必须等于recv_length
    void *(buffer[3]); // 读取缓冲区
    uint8_t recv_len; // 单次接收长度

    UART_HandleTypeDef *pHUART;
    void (*recv_callback)(void); // DMA接收完毕调用的函数
    ps_uart_instance_t instance;
} ps_uart_t;

typedef ps_uart_t* ps_uart_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化ps_uart的循环接收模式
 */
void ps_uart_init(ps_uart_handle_t handle);

/**
 * @brief 停止ps_uart的循环接收模式，允许用户自己接收数据
 * @note 只能使用非中断的方式接收数据，也就是说只能用HAL_UART_Receive()
 */
inline void ps_uart_stop(ps_uart_handle_t handle)
{
    HAL_UART_DMAStop(handle->pHUART);
}

/**
 * @brief 阻塞的发送数据
 */
void ps_uart_transmit(ps_uart_handle_t handle, const void *pData, int len);

/**
 * @brief 非阻塞的发送数据
 */
HAL_StatusTypeDef ps_uart_transmit_nowait(ps_uart_handle_t handle,
                                          const void *pData, int len);

/**
 * @brief 取出最近收到的数据
 * @param dst 拷贝的目标位置
 * @return 是否取出数据
 */
bool ps_uart_get_nowait(ps_uart_handle_t handle, void *dst);

/**
 * @brief 阻塞的取出最近收到的数据
 * @param dst 拷贝的目标位置
 * @return 取出的数据长度
 */
void ps_uart_get(ps_uart_handle_t handle, void* dst);

/**
 * @brief 获得最近收到数据的地址的访问权
 * @return 收到的数据的地址，如果没有收到数据，返回NULL
 * @note 使用完毕后，必须调用ps_uart_release释放当前地址的数据。单个handle的acquire
 * 必须互斥，如果多次调用，则无法保证除了最后一次以外的内存完整性。
 */
void* ps_uart_acquire(ps_uart_handle_t handle);

/**
 * @brief 释放最近调用acquire地址的访问权
 */
inline static void ps_uart_release(ps_uart_handle_t handle)
{
    handle->instance.copy_length[handle->instance.index+1 & 0x1] = 0;
    handle->instance.busy = false;
}

/**
 * @brief DMA中断，触发DMA中断说明接收到完整的数据包
 * @note 需要放到对应DMA的中断处理函数中
 */
void ps_uart_receive_DMA_IT(ps_uart_handle_t handle);

/**
 * @brief 空闲中断，触发空闲中断就说明数据包接受不完整
 * @note 需要放在对应的UART中断处理函数中，必须在恢复IDLE标志位之前调用本函数
 */
void ps_uart_receive_IDLE_IT(ps_uart_handle_t handle);

/**
 * @brief 辅助函数，计算累加校验和（按字节累加）
 */
inline uint8_t ps_uart_sigma_check_sum(uint8_t *frame, int len)
{
    uint8_t sum = 0;
    for (int x=0; x<len; x++) sum += frame[x];
    return sum;
}

#ifdef __cplusplus
}
#endif

#endif