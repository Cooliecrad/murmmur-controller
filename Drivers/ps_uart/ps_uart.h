#ifndef __PS_COMM_CTL_H__
#define __PS_COMM_CTL_H__

#include "vision_protocol.h"
#include "stm32h7xx_hal.h"
#include "usart.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化UART
 */
void ps_uart_init(UART_HandleTypeDef *uart_handle);

/**
 * @brief 发送数据
 */
void ps_uart_transmit(uint8_t *frame, int len);

/**
 * @brief 发送请求
 */
void ps_uart_request(ps_comm_type_t type);

/**
 * @brief 取出最近收到的数据。如果数据无效或没有新的数据，则不会拷贝到dst
 * @return 取出失败返回1
 */
int ps_uart_receive(PsCommFrame *dst);

/**
 * @brief 阻塞的等待指定类型的返回值。获得特定值将拷贝到dst
 * @param type 需要获取的类型
 * @param dst 需要拷贝到的位置，如果为NULL，则拷贝到公共缓冲区。公共缓冲区的数据必须
 * 在下次ps_uart_receive()前转移完毕
 * @return 如果dst为NULL，返回指向公共缓冲区的指针 
 */
PsCommFrame* ps_uart_response(ps_comm_type_t type, PsCommFrame* dst);

/**
 * @brief 接收中断，需要放到对应UART的中断处理函数中
 */
void ps_uart_receive_IT(void);

#ifdef __cplusplus
}
#endif

#endif