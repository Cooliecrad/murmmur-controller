#include "screen.h"
#include "ili9341.h"
#include "easy_font.h"

SPI_HandleTypeDef *pHSPI;
EasyRender *pEasyRender = NULL;

void screen_init(SPI_HandleTypeDef *pHSPI)
{
    pHSPI = pHSPI;
    ili9341_init(pHSPI);
    pEasyRender = new EasyRender {ili9341_set_window, ili9341_write_GRAM};
}

void screen_clear()
{
    if (pEasyRender) pEasyRender->label(0, 0, "+++++++", 0xffff);
}

void screen_show(const color_t colors[6])
{
    char buffer[7] = {};
    for (int x=0; x<6; x++)
    {
        auto num = (uint8_t)colors[x];
        // 如果存在问题，就不要解析了
        if (num < 1 || num > 3) return;
        int index = x < 3 ? x : x+1;
        buffer[index] = num + '0';
    }
    buffer[3] = '+';
    if (pEasyRender) pEasyRender->label(0, 0, buffer, 0xFFFF);
}