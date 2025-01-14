#include "font_render.h"
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cmath>

#define __bitmap_line_shift(u8_src, b_shift) u8_src[(b_shift)/8] & UINT8_BIT_MASK_LE[(b_shift)%8]

using namespace std;
using namespace ps::font;

namespace
{
    color_t COLOR_BUFFER[__PSAF_MAX_FONT_SIZE];
    const uint8_t UINT8_BIT_MASK_LE[8] = { // 掩码，用于取得一个字节中某一位的值
        0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
    };
}

CharAdapter::CharAdapter(char_size_t f_size)
{
    buffer = (color_t**)malloc(sizeof(color_t*)*f_size);
    if (buffer == nullptr)
    {
        // printf("[ERROR] Out of memory\n");
    }
}

CharAdapter::~CharAdapter()
{
    free(buffer);
}

void CharAdapter::adapt(color_t *raw_bf, int line_w, int height)
{
    if (buffer)
    {
        this->height = height;
        buffer[0] = raw_bf;
        for (int y=1; y<height; y++)
            buffer[y] = buffer[y-1] + line_w;
    }
}

void CharAdapter::shift(int x_shift)
{ 
    for (int y=0; y<height; y++) buffer[y] += x_shift;
}


CharLineFont::CharLineFont(const uint8_t *src, char_size_t size)
    : f_size{size}, src {(char_line*)src}, line_shift {size/2},
      font_shift {line_shift*size}
{
}

void CharLineFont::write(const CharBlock& c_block, color_t **dst)
{
    // 设置颜色缓冲区
    if (c_block.color != COLOR_BUFFER[0])
    {
        for (int x=0; x<__PSAF_MAX_FONT_SIZE; x++) COLOR_BUFFER[x] = c_block.color;
    }
    // 按照字体信息完成渲染
    const char_line *ptr = src
                           + font_shift*c_block.font_num
                           + line_shift*c_block.y;
    for (int y=0; y<c_block.height; y++, ptr+=line_shift)
    {
        char_line ctrl = src[0]; // 遍历到的“拷贝控制块”
        for (int x=0; ctrl.end != 0 && x < line_shift; x++, ctrl = src[x])
        {
            ctrl.start -= c_block.x; // 计算控制的相对起点
            ctrl.end -= c_block.x;
            if (ctrl.start < c_block.width && ctrl.end > 0)
            { // 存在重叠
                ctrl.start = max<char_size_t>(0, ctrl.start); // 计算起点
                ctrl.end = min<char_size_t>(ctrl.end-1, c_block.width); // 计算宽度
                memcpy(dst[y]+ctrl.start, COLOR_BUFFER, sizeof(color_t)*ctrl.end);
            }
        }
    }
}


BitmapFont::BitmapFont(const uint8_t *src, char_size_t size)
    : f_size {size}, src {src}, 
      line_shift {int(ceil(float(size)/16))}, font_shift {line_shift*size}
{
}

void BitmapFont::write(const CharBlock& c_block, color_t **dst)
{
    // 设置颜色缓冲区
    if (c_block.color != COLOR_BUFFER[0])
    {
        for (int x=0; x<__PSAF_MAX_FONT_SIZE; x++) COLOR_BUFFER[x] = c_block.color;
    }
    // 按照字体信息完成渲染
    const uint8_t *ptr = src
                       + font_shift*c_block.font_num
                       + line_shift*c_block.y;
    for (int y=0; y<c_block.height; y++, ptr+=line_shift)
    {
        for (int x=0; x<c_block.width; x++)
        {
            if (__bitmap_line_shift(ptr, x+c_block.x))
                dst[y][x] = c_block.color;
        }
    }
}

void FontRender::render(color_t color, char_size_t size, const char *text,
                        color_t *raw_bf, int bf_width, PxBlock block)
{
    // 检查字库有无特定字号，没有就不渲染了
    if (!fb.has_size(size)) return;
    // 偏移是否有效，无效也不渲染
    if (block.x < 0 || (block.y < 0 || block.y >= size)) return;
    // 载入缓冲区
    adapter.adapt(raw_bf, bf_width, block.height);
    const int swidth = size/2;
    text += block.x/swidth; // str移动到正确的起点
    block.x = block.x%swidth;

    CharBlock cblock {color, 0, block};
    while (block.width && *text != '\0') // 只要还有空位，一直渲染
    {
        cblock.font_num = latin_cvt(size, *text);
        cblock.width = min<decltype(cblock.width)>(swidth, block.width);
        fb[size].write(cblock, adapter.buffer);
        // 移动指针
        text++;
        block.x = 0;
        block.width -= cblock.width;
        adapter.shift(cblock.width);
    }
}
