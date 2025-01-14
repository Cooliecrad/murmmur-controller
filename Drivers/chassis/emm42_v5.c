#include "emm42_v5.h"
#include "emm42_v5_protocol.h"
#include "common.h"
#include <stdio.h>

const static int CHASSIS_POLL_INTERVAL = 0;
const static int EMM42_V5_QUERY_INTERVAL = 0;

/**
 * @brief 最后一次收发完成
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
        0x00, 0xF3
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

HAL_StatusTypeDef emm42_response(emm42_handle_t handle, uint8_t addr, bool reached)
{
    emm42_received(handle);

    static uint8_t BUFFER[] = {
        0x00, 0x48, 0xD1, 0x01, 0x19, 0x02, 0x02, 0x02, 0x01, 0x10,
        0x01, 0x00, 0x03, 0xE8, 0x09, 0x60, 0x13, 0x88, 0x07, 0x07,
        0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x08, 0x98, 0x07, 0xD0,
        0x00, 0x05, 0x6B
    };
    BUFFER[0] = addr;
    BUFFER[22] = reached ? 3 : 1;
    BUFFER[sizeof(BUFFER)-1] = CHECKSUM(BUFFER, sizeof(BUFFER));

    handle->instance.received = false;
    return HAL_UART_Transmit_DMA(handle->ps_uart_handle->pHUART, BUFFER, sizeof(BUFFER));
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