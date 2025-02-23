#include "emm42_v5.h"
#include "emm42_v5_protocol.h"
#include "common.h"
#include <stdio.h>

const static int CHASSIS_POLL_INTERVAL = 0;
const static int EMM42_V5_QUERY_INTERVAL = 0;
const static int EMM42_V5_READ_POSITION_INTERVAL = 20;

/**
 * @brief 检测最后一次收发完成。每次发送请求之前都会检查上次是否完成接收
 */
static inline void emm42_received(emm42_handle_t handle)
{
#ifdef __EMM42_V5_CHECK_RESPONSE
    // 第一次发送不需要检查
    if (handle->instance.received)
    {
        handle->instance.received = false;
        return;
    }

    // 必须收到上一个指令的反馈才会继续下一个
    uint8_t *ret = (uint8_t*)ps_uart_acquire(handle->ps_uart_handle);
    while (   handle->ps_uart_handle->pHUART->hdmatx->State != HAL_DMA_STATE_READY
           || ret == NULL)
    {
        ps_uart_release(handle->ps_uart_handle);
        HAL_Delay(0);
        ret = (uint8_t*)ps_uart_acquire(handle->ps_uart_handle);
    }

#   ifdef __EMM42_V5_INDICATOR
        handle->instance.accept_sum++;
#   endif
    ps_uart_release(handle->ps_uart_handle);
#endif
}

/**
 * @brief 如果通过ps_uart_stop中断了通信，需要手动调用这个函数开启通信
 */
static inline void ps_uart_restart(emm42_handle_t handle)
{
    ps_uart_init(handle->ps_uart_handle);
    handle->instance.received = true; // 认为接收完毕，因为ps_uart不负责接收，没法控制上下文
}

void emm42_v5_init(emm42_handle_t handle)
{
    ps_uart_init(handle->ps_uart_handle);

    handle->instance.received = true;
    handle->instance.reached = true;
}

HAL_StatusTypeDef emm42_set_state(emm42_handle_t handle, uint8_t addr,
                                  uint8_t enable, uint8_t sync)
{
    emm42_received(handle);

    const static int LEN = 6;
    static uint8_t BUFFER[LEN] = {
        0x00, 0xF3, 0xAB,
    };
    BUFFER[0] = addr;
    BUFFER[3] = enable;
    BUFFER[4] = sync;
    BUFFER[5] = CHECKSUM(BUFFER, LEN);

    handle->instance.received = false;
    return HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, BUFFER,
                                 sizeof(BUFFER));
}

emm42_v5_state_t emm42_read_state(emm42_handle_t handle, uint8_t addr)
{
    emm42_received(handle);

    const static int LEN = 3;
    static uint8_t BUFFER[LEN] = {
        0x00, 0x3A,
    };
    BUFFER[0] = addr;
    BUFFER[2] = CHECKSUM(BUFFER, LEN);

    // 反复发送请求，并且反复查询请求。收发没有明确的先后次序
    uint8_t ret;
    bool received = false;
    while (!received)
    {
        HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, BUFFER, sizeof(BUFFER));
        uint8_t *recv = (uint8_t*)ps_uart_acquire(handle->ps_uart_handle);
        if (recv != NULL)
        {
            ret = recv[2];
            received = true;
        }
        ps_uart_release(handle->ps_uart_handle);
        HAL_Delay(EMM42_V5_QUERY_INTERVAL);
    }
    return ret;
}

HAL_StatusTypeDef emm42_speed_ctl(emm42_handle_t handle, uint8_t addr, 
                                  uint8_t dir, uint16_t RPM, 
                                  uint8_t ACC,   uint8_t sync)
{
    emm42_received(handle);

    const static int LEN = 8;
    static uint8_t BUFFER[LEN] = {
        0x00, 0xF6
    };
    BUFFER[0] = addr;
    BUFFER[2] = dir;
    BUFFER[3] = RPM >> 8;
    BUFFER[4] = RPM & 0xFF;
    BUFFER[5] = ACC;
    BUFFER[6] = sync;
    BUFFER[7] = CHECKSUM(BUFFER, LEN);

    handle->instance.received = false;
    return HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, BUFFER,
                                 sizeof(BUFFER));
}

HAL_StatusTypeDef emm42_pos_ctl(emm42_handle_t handle, uint8_t addr, uint8_t dir,
                                uint16_t RPM, uint8_t ACC,  uint32_t pulse,
                                uint8_t relate_abs, uint8_t sync)
{
    emm42_received(handle);
    const static int LEN = 13;
    static uint8_t BUFFER[LEN] = {
        0x00, 0xFD
    };
    BUFFER[0] = addr;
    BUFFER[2] = dir;
    BUFFER[3] = RPM >> 8;
    BUFFER[4] = RPM & 0xFF;
    BUFFER[5] = ACC;
    BUFFER[9] = pulse & 0xFF;
    pulse >>= 8;
    BUFFER[8] = pulse & 0xFF;
    pulse >>= 8;
    BUFFER[7] = pulse & 0xFF;
    pulse >>= 8;
    BUFFER[6] = pulse & 0xFF;
    BUFFER[10] = relate_abs;
    BUFFER[11] = sync;
    BUFFER[12] = CHECKSUM(BUFFER, LEN);

    handle->instance.reached = false;
    handle->instance.received = false;
    return HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, BUFFER, sizeof(BUFFER));
}

HAL_StatusTypeDef emm42_halt(emm42_handle_t handle, uint8_t addr, uint8_t sync)
{
    emm42_received(handle);

    const static int LEN = 5;
    static uint8_t BUFFER[LEN] = {
        0x00, 0xFE, 0x98
    };
    BUFFER[0] = addr;
    BUFFER[3] = sync;
    BUFFER[4] = CHECKSUM(BUFFER, LEN);

    handle->instance.received = false;
    return HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, BUFFER, sizeof(BUFFER));
}

HAL_StatusTypeDef emm42_sync(emm42_handle_t handle)
{
    emm42_received(handle);

    const static int LEN = 4;
    static uint8_t BUFFER[LEN] = {
        0x00, 0xFF, 0x66
    };
    BUFFER[3] = CHECKSUM(BUFFER, LEN);

    handle->instance.received = false;
    return HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, BUFFER, sizeof(BUFFER));
}

HAL_StatusTypeDef emm42_reset(emm42_handle_t handle)
{
    emm42_received(handle);

    const static int LEN = 4;
    static uint8_t BUFFER[LEN] = {
        0x00, 0x0A, 0x6D
    };
    BUFFER[3] = CHECKSUM(BUFFER, LEN);

    handle->instance.received = false;
    return HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, BUFFER, sizeof(BUFFER));
}

/**
 * @brief 从步进电机中读取驱动参数
 * @note 和__emm42_write_drive_config()配合使用
 */
void __emm42_read_drive_config(emm42_handle_t handle, uint8_t addr,
                               emm42_v5_drive_t *rx)
{
    /**
     * @note 有意思的地方：收发的实际数据包一样大，所以可以通用
     */
    // 暂停接收
    ps_uart_stop(handle->ps_uart_handle);

    rx->dummy[0] = addr;
    rx->dummy[1] = 0x42;
    rx->dummy[2] = 0x6C;
    rx->dummy[3] = CHECKSUM(rx, 4);
    HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, (uint8_t*)rx, 4);
    HAL_UART_Receive(handle->ps_uart_handle->pHUART, (uint8_t*)rx,
                     sizeof(emm42_v5_drive_t), HAL_MAX_DELAY);
}

/**
 * @brief 从步进电机中写入参数
 * @note 和__emm42_read_drive_config()配合使用
 */
void __emm42_write_drive_config(emm42_handle_t handle, uint8_t addr,
                                emm42_v5_drive_t *tx)
{
    // 修改参数
    tx->tx.addr = addr;
    tx->tx.cmd[0] = 0x48;
    tx->tx.cmd[1] = 0xD1;
    tx->tx._0x01 = 0x01;
    HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, (uint8_t*)tx, sizeof(emm42_v5_drive_t));
    ps_uart_restart(handle);
    HAL_Delay(10); // 驱动设置相关，需要一个长延时
}

void emm42_set_response(emm42_handle_t handle, uint8_t addr, bool reached)
{
    emm42_received(handle);

    static emm42_v5_drive_t BUFFER;
    __emm42_read_drive_config(handle, addr, &BUFFER);
    BUFFER.tx.config.Response = reached ? 3 : 1;
    __emm42_write_drive_config(handle, addr, &BUFFER);
}

void emm42_set_reach_wnd(emm42_handle_t handle, uint8_t addr, uint16_t deg)
{
    emm42_received(handle);

    static emm42_v5_drive_t BUFFER;
    __emm42_read_drive_config(handle, addr, &BUFFER);
    BUFFER.tx.config.reach_wnd = deg;
    __emm42_write_drive_config(handle, addr, &BUFFER);
}

void emm42_arrived(emm42_handle_t handle, uint8_t addr)
{
    // 轮询该地址是否完成即可
    emm42_v5_state_t state;
    do {
        state = emm42_read_state(handle, addr);
    } while (   ((state & emm42_v5_state_ENABLE) == 0)
             || ((state & emm42_v5_state_REACHED) == 0));
}

float emm42_read_position(emm42_handle_t handle, uint8_t addr)
{
    emm42_received(handle);

    ps_uart_stop(handle->ps_uart_handle);
    static emm42_v5_read_position_t BUFFER;
    HAL_StatusTypeDef status = HAL_TIMEOUT;
    while (status != HAL_OK)
    {
        uint8_t *tx_buffer = (uint8_t*)&BUFFER; // 顺便用来作为发送缓冲区
        tx_buffer[0] = addr;
        tx_buffer[1] = 0x36;
        tx_buffer[2] = CHECKSUM(tx_buffer, 3);
        HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, tx_buffer, 3);
        status = HAL_UART_Receive(handle->ps_uart_handle->pHUART,
                                  (uint8_t*)&BUFFER,
                                  sizeof(emm42_v5_read_position_t),
                                  EMM42_V5_READ_POSITION_INTERVAL);
    }
    uint8_t *data = (uint8_t*)&BUFFER.pulse;
    float pulse = (float)( data[3] + (data[2]<<8)+ (data[1]<<16) + (data[0]<<24) );
    ps_uart_restart(handle);

    return BUFFER.symbol == 0 ? ( pulse * 360. ) / 65536. : -( pulse * 360. ) / 65536.; 
}