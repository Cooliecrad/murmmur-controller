#ifndef __EASY_FONT_H__
#define __EASY_FONT_H__

#include "font_render.h"

/***
 * @brief 定长的标签列表
 */
class EasyRender
{
public:
    using select_sector = void(*)(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    using write_mem = void(*)(ps::font::color_t *buffer, int len);
    EasyRender(select_sector select, write_mem write);
    /**
     * @brief 渲染字符串，长度必须小于等于length
     */
    void label(uint16_t x, uint16_t y, const char *text, ps::font::color_t color);

    /**
     * @brief 填充指定颜色
     */
    void fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ps::font::color_t color);
private:
    select_sector select_handle;
    write_mem write_handle;
    ps::font::FontRender fr;
};

#endif