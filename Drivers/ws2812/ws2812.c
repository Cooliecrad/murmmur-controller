#include "ws2812.h"
#include "stm32h750xx.h"
#include <string.h>

/**
 * @brief 调光控制循环：1.发送帧序列 2.等待一段时间 3.发送空白序列 4.等待一段时间
 * 发送帧序列循环：DMA发送 -> DMA中断: 发送 -> DMA中断: 发送结束
 */

const static int PIXLE_DEPTH = 24; // 色深
const static uint16_t DMA_UNIT_LENGTH = 4; // 单次DMA传输代表的bit数
const static int PIXLE_PERIDO = 1200; // 计时器完成一轮计时所需时间，单位ns
const static int TIM_ARR = 4; // 计时器一轮计数值（ARR寄存器的值）
const static int TIM_UNIT = PIXLE_PERIDO / TIM_ARR; // 计时器计时一个单位所需时间
// 两轮数据发送的最小时间间隔
const static int WS2812_TX_INTERVAL = 280000;
// 两轮数据发送的最小时间间隔，单位(TIM_UNIT*1ns)
const static int WS2812_TX_INTERVAL_UNIT = WS2812_TX_INTERVAL / TIM_UNIT;

const static int CODE_TABLE_SIZE = DMA_UNIT_LENGTH * DMA_UNIT_LENGTH * DMA_UNIT_LENGTH;

typedef uint16_t DMA_TX_WIDTH;
const static DMA_TX_WIDTH CODE_TABLE[CODE_TABLE_SIZE] = {
    1, 1, 1, 1,
    2, 1, 1, 1,
    1, 2, 1, 1,
    2, 2, 1, 1,
    1, 1, 2, 1,
    2, 1, 2, 1,
    1, 2, 2, 1,
    2, 2, 2, 1,
    1, 1, 1, 2,
    2, 1, 1, 2,
    1, 2, 1, 2,
    2, 2, 1, 2,
    1, 1, 2, 2,
    2, 1, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 2,
};

// 带上一个额外的空白位，即不输出高电平位
static DMA_TX_WIDTH PIXEL_BUFFER[PIXLE_DEPTH + 1] = {0};

void ws2812_init(ws2812_handle handle)
{
    // 初始化对象
    handle->instance.on_interval = 1;
    handle->instance.type = 0;
    handle->instance.buffer = NULL;
    handle->instance.px_index = 0;
    handle->instance.length = 0;
    handle->instance.time_us = 0;
}

/**
 * @brief 帧内子过程：获得像素数据、解码发送
 */
void __transmit_decode(ws2812_handle handle)
{
    uint32_t pixel;
    struct __ws2812_instance *pInstance = &handle->instance;
    // 产生当前帧的当前像素
    if (pInstance->on_off == 1)
    {
        switch (pInstance->type)
        {
        case 0:
            pixel = pInstance->buffer[pInstance->px_index];
            break;
        case 1:
            pixel = pInstance->generator.func(pInstance->px_index,
                                              pInstance->generator.user);
            break;
        case 2:
            pixel = pInstance->time_series_generator.func(pInstance->px_index,
                                                          pInstance->time_us / 1000,
                                                          pInstance->time_series_generator.user);
            break;
        }
        // RGB888 -> GRB888
        pixel = ( ( pixel & 0xFF00 ) << 8 ) | ( ( pixel & 0xFF0000 ) >> 8 ) | ( pixel & 0xFF );
    } else
        pixel = 0x000000;

    // 解析像素，产生缓冲区
    for (int index=0; index < PIXLE_DEPTH; index += DMA_UNIT_LENGTH)
    {
        int hf_byte = (pixel >> (PIXLE_DEPTH - index - DMA_UNIT_LENGTH)) & 0xF;
        memcpy(PIXEL_BUFFER + index, CODE_TABLE + hf_byte*DMA_UNIT_LENGTH,
               sizeof(DMA_TX_WIDTH)*DMA_UNIT_LENGTH);
    }

    // 发送像素
    pInstance->time_us += ((PIXLE_PERIDO * PIXLE_DEPTH) / 1000);
    HAL_TIM_PWM_Start_DMA(handle->htim, handle->Channel,
                          (uint32_t*)PIXEL_BUFFER, PIXLE_DEPTH+1);
}

/**
 * @brief 发送新的一帧
 */
static inline void __transmit_send_frame(ws2812_handle handle)
{
    handle->instance.px_index = 0;
    __transmit_decode(handle);
}

void ws2812_frame(ws2812_handle handle, const uint32_t *buffer, int length)
{
    struct __ws2812_instance *pInstance = &handle->instance;
    pInstance->length = length;
    pInstance->type = 0;
    pInstance->buffer = buffer;

    pInstance->on_off = true;
    __transmit_send_frame(handle);
}

void ws2812_frame_generator(ws2812_handle handle, uint32_t(*generator)(int index, void *user),
                            int length, void *user)
{
    struct __ws2812_instance *pInstance = &handle->instance;
    pInstance->length = length;
    pInstance->type = 1;
    pInstance->generator.func = generator;
    pInstance->generator.user = user;

    pInstance->on_off = true;
    __transmit_send_frame(handle);
}

void ws2812_send_generator(ws2812_handle handle,
                           uint32_t(*generator)(int index, uint32_t time_ms, void *user),
                           int length, void *user)
{
    struct __ws2812_instance *pInstance = &handle->instance;
    pInstance->length = length;
    pInstance->type = 2;
    pInstance->time_series_generator.func = generator;
    pInstance->time_series_generator.user = user;

    pInstance->on_off = true;
    __transmit_send_frame(handle);
}

void ws2812_set_brightness(ws2812_handle handle, float brightness)
{
    if (brightness > 1 || brightness < 0)
    {
        return; // 无效亮度
    }
    if (brightness == 0) // 亮度为0，暂停
    {
        ws2812_stop(handle);
        return;
    } else if (brightness == 1) // 需要开启一轮新的发送
    {
        handle->instance.on_interval = 1;
        handle->instance.off_interval = 0;
    } else if (brightness < 0.5)
    {
        handle->instance.on_interval = WS2812_TX_INTERVAL_UNIT;
        handle->instance.off_interval = (int)(((1-brightness)/brightness) * (float)WS2812_TX_INTERVAL_UNIT);
    } else if (brightness > 0.5)
    {
        handle->instance.off_interval = WS2812_TX_INTERVAL_UNIT;
        handle->instance.on_interval = (int)((brightness/(1-brightness)) * (float)WS2812_TX_INTERVAL_UNIT);
    }
    __transmit_decode(handle);
}

void ws2812_stop(ws2812_handle handle)
{
}

void ws2812_DMA_IT(ws2812_handle handle)
{
    if (handle == NULL) return;

    TIM_HandleTypeDef *htim = handle->htim;
    if (htim->hdma[handle->Channel]->State != HAL_DMA_STATE_READY) return;

    struct __ws2812_instance *pInstance = &handle->instance;
    if (pInstance->px_index == pInstance->length) // 帧结束，开中断以调光
    {
        int on_off = !pInstance->on_off, on_int = pInstance->on_interval,
            off_int = pInstance->off_interval; // 临时取出，避免反复取址

        pInstance->on_off = on_off; // 亮暗控制
        htim->Instance->DIER |= TIM_DIER_CC1IE; // 开TIM中断
        htim->Instance->ARR = on_off ? on_int : off_int; // 写入TIM等待时长
        pInstance->time_us += on_off ? (( on_int * TIM_UNIT ) / 1000)
                                     : (( off_int * TIM_UNIT ) / 1000); // 记录等待时间
    }
    else // 未完毕，发送下一个像素
        __transmit_decode(handle);
}

void ws2812_TIM_IT(ws2812_handle handle)
{
    TIM_HandleTypeDef *htim = handle->htim;
    // 等待结束，准备下一轮传输
    htim->Instance->DIER &= (~(uint32_t)TIM_DIER_CC1IE);
    htim->Instance->ARR = TIM_ARR;
    handle->instance.on_off = false;
    __transmit_send_frame(handle);
}
