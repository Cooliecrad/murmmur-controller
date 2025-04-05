#ifndef __WS2812_H__
#define __WS2812_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_tim.h"

typedef struct __ws2812_define
{
    TIM_HandleTypeDef *htim; // 使用的定时器
    uint32_t Channel; // 使用的PWM输出通道

    struct __ws2812_instance // 私有成员，无需关心
    {
        union // 产生像素
        {
            const uint32_t *buffer; // 固定序列

            struct // 生成函数
            {
                uint32_t(*func)(int index, void *user);
                void *user;
            } generator;

            struct // 时间序列生成函数
            {
                uint32_t(*func)(int index, uint32_t time_index, void *user);
                void *user;
            } time_series_generator;
        };
        int type; // 0: 固定序列; 1: 生成函数; 2: 时间生成函数

        // 单次帧传输控制
        int px_index; // 传输完毕的像素数
        int length; // 当前传输的总像素数

        // 时间序列控制
        uint64_t time_us;

        // 亮度控制
        int on_off; // 0: off; 1: on
        int on_interval; // 亮时长，单位为 (TIM_UNIT*1ns)，如果为1则永远亮
        int off_interval; // 暗时长，单位为（TIM_UNIT*1ns），如果为1则永远不亮

    } instance;
} *ws2812_handle;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化WS2812
 */
void ws2812_init(ws2812_handle handle);

/**
 * @brief 向WS2812发送一帧像素数据，非阻塞发送
 * @param pixels 像素数据数组，采用RGB888
 * @param length 数组大小，即要发送的像素数量
 */
void ws2812_frame(ws2812_handle handle, const uint32_t *pixels, int length);

/**
 * @brief 向WS2812发送一帧像素数据，非阻塞发送
 * @param generator 像素数据生成函数。接收两个参数，第一个是像素的序号，第二个是用户自定义数据的指针
 * @param length 要发送的像素数量
 * @param user 用户数据，会传递到generator中
 */
void ws2812_frame_generator(ws2812_handle handle, uint32_t(*generator)(int index, void *user),
                            int length, void *user);

/**
 * @brief 向WS2812发送随时间变化的像素数据，非阻塞发送
 * @param generator 像素数据生成函数。接收三个参数，第一个是像素的序号，第二个是生成当前像素
 *                  距离ws2812_send_generator()函数经过了多长时间，第三个是用户自定义数据的指针
 * @param length 要发送的像素数量
 * @param user 用户数据，会传递到generator中
 * @note 关于time_ms溢出：约50天会产生溢出，本函数溢出不安全，但溢出行为是time_ms重新回到
 *       较小的值，不会有严重的问题。
 */
void ws2812_send_generator(ws2812_handle handle,
                           uint32_t(*generator)(int index, uint32_t time_ms, void *user),
                           int length, void *user);

/**
 * @brief 设置显示亮度
 * @param brightness 亮度，取值(0~1)。采用PWM调光，亮度过低会存在低频屏闪问题！
 */
void ws2812_set_brightness(ws2812_handle handle, float brightness);

/**
 * @brief 停止显示
 */
void ws2812_stop(ws2812_handle handle);

/**
 * @brief DMA中断处理函数，请放在对应的DMA中断函数中
 */
void ws2812_DMA_IT(ws2812_handle handle);

/**
 * @brief TIM中断处理函数，请放在对应的TIM中断函数中
 */
void ws2812_TIM_IT(ws2812_handle handle);

#ifdef __cplusplus
}
#endif

#endif