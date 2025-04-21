#include "cool_rgb.h"

struct __ws2812_define cool_rgb;
ws2812_handle cool_rgb_handle = NULL;

static int RGB_LENGTH = 1;
static uint32_t RGB_INTERVAL_MS = 1;

void cool_rgb_init(TIM_HandleTypeDef *htim, uint32_t Channel, int rgb_length)
{
    cool_rgb_handle = &cool_rgb;
    RGB_LENGTH = rgb_length;

    cool_rgb.htim = htim;
    cool_rgb.Channel = Channel;
    ws2812_init(cool_rgb_handle);
}

uint32_t __rgb_time(int index, uint32_t time_ms, void *user)
{
    // h ∈ [0, 2]
    double h = ((double)index / (double)RGB_LENGTH) + ((double)time_ms / (double)RGB_INTERVAL_MS);
    h = h > 1 ? h-1 : h; // 有可能超过1

    double r = -fabs(h*2) + 1,
           g = -fabs(h*2-1) + 1,
           b = -fabs(h*2-2) + 1;
    uint8_t R = r > 0 ? r * 0xFF : 0,
            G = g > 0 ? g * 0xFF : 0,
            B = b > 0 ? b * 0xFF : 0;
    return ( R<<16 ) | ( G<<8 ) | B;
}

uint32_t __rgb(int index, void *user)
{
    return __rgb_time(index, 0, user);
}

void cool_rgb_static(double brightness)
{
    ws2812_frame_generator(cool_rgb_handle, __rgb, RGB_LENGTH, NULL);
    ws2812_set_brightness(cool_rgb_handle, brightness);
}

void cool_rgb_dynamic(double brightness, uint32_t interval_ms)
{
    RGB_INTERVAL_MS = interval_ms;
    ws2812_send_generator(cool_rgb_handle, __rgb_time, RGB_LENGTH, NULL);
    ws2812_set_brightness(cool_rgb_handle, brightness);
}