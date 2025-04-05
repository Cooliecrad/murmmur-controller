#ifndef __PS_COOL_RGB_H__
#define __PS_COOL_RGB_H__

#include "ws2812.h"

extern ws2812_handle cool_rgb_handle;

#ifdef __cplusplus
extern "C" {
#endif

void cool_rgb_init(TIM_HandleTypeDef *htim, uint32_t Channel, int rgb_length);

void cool_rgb_static(double brightness);

void cool_rgb_dynamic(double brightness, uint32_t interval_ms);

#ifdef __cplusplus
}
#endif

#endif