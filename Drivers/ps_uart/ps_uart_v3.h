#ifndef __PS_UART_V3_H__
#define __PS_UART_V3_H__

#include <cstdint>
#include <cstddef>
#include "stm32h7xx_hal.h"
#include "usart.h"

#define __PS_UART_INDICATOR

namespace ps
{

class UART
{
public:
    /**
     * @brief 初始化UART接收
     */
    UART(UART_HandleTypeDef *pHUART, uint8_t *(buffer[3]), uint8_t recv_max_length,
         uint32_t poll_interval = 10);

    /**
     * @brief 开始循环接收数据
     */
    void start() const
    {
        HAL_UARTEx_ReceiveToIdle_DMA(pHUART, buffer[2]->ptr, recv_max_len);
    }

    /**
     * @brief 停止ps_uart的循环接收，允许用户自行接收数据
     */
    void stop() const
    {
        HAL_UART_DMAStop(pHUART);
        for (int x=0; x<3; x++) delete buffer[x];
    }
    
    /**
     * @brief 非阻塞的发送数据
     */
    HAL_StatusTypeDef transmit_nowait(const uint8_t *data, uint16_t len) const;

    /**
     * @brief 阻塞的发送数据
     */
    void transmit(const uint8_t *data, uint16_t len) const;
    
    /**
     * @brief 非阻塞的获取最新接收到的数据，如果没有数据被接收，返回NULL
     */
    const void* get_nowait(int *length = NULL);

    /**
     * @brief 阻塞的获取最新接收到的数据
     */
    const void* get(int *length = NULL);

    /**
     * @brief DMA接收中断，触发接收中断说明存在数据包超过不定长接收的风险
     * @note 需要放在对于的DMA接收中断处理函数中
     */
    void receive_DMA_IT();

    /**
     * @brief 空闲中断，触发空闲中断就说明数据包接受不完整
     * @note 需要放在对应的UART中断处理函数中，必须在恢复IDLE标志位之前调用本函数
     */
    void receive_IDLE_IT();

    const uint32_t poll_interval;
    
    /**
     * @note 请在start()调用前或者stop()调用后修改下面的成员
     */

    bool (*validate)(UART& self, const uint8_t *data, uint8_t length) = NULL; // 验证函数
    void (*receive_cb)(UART& self, const uint8_t *data, uint8_t length) = NULL; // 接收成功回调函数
protected:
    UART_HandleTypeDef *pHUART;

private:
    /**
     * @brief 缓冲区实现
     */
    struct Buffer
    {
        uint8_t length;
        uint8_t *ptr;
    };

    /**
     * @brief 子过程：验证并交换数组
     */
    void validate_swap(uint8_t length);

    bool read_zero = false; // get_nowait读取buffer[0]，用来控制互斥
    UART::Buffer *buffer[3];
    uint8_t recv_max_len; // 单次接收长度

#   ifdef __PS_UART_INDICATOR
        uint32_t recv_DMA_sum = 0;
        uint32_t recv_IDLE_sum = 0;
        uint32_t accept_sum = 0; // 总接受数据包个数
#   endif
};


/**
 * @brief 辅助函数，计算累加校验和（按字节累加）
 */
inline uint8_t sigma_check_sum(uint8_t *frame, int len)
{
    uint8_t sum = 0;
    for (int x=0; x<len; x++) sum += frame[x];
    return sum;
}

}

#endif