#include "easy_font.h"
#include "font_base.h"
#include <cstring>

using namespace ps::font;

#define min(x, y) (x) < (y) ? (x) : (y)

namespace
{
    const char_size_t FONT_SIZE = 88;
    const int BF_LINES = 4;
    const int LABEL_LEN = 7;
    constexpr int LINE_PIXELS = LABEL_LEN*FONT_SIZE/2;
    color_t RENDER_BF[LINE_PIXELS*BF_LINES] = {0};
    char TEXT_BF[LABEL_LEN];
}

EasyRender::EasyRender(select_sector select, write_mem write)
    : fr {FONTBASE}, select_handle {select}, write_handle {write}
{
}

void EasyRender::label(uint16_t x, uint16_t y, const char *text, color_t color)
{
    PxBlock range {};
    int len = min(strlen(text), LABEL_LEN);
    range.width = FONT_SIZE/2*len;
    for (int x=0; x < len && x < LABEL_LEN; x++)
        TEXT_BF[x] = fr.latin_cvt(FONT_SIZE, text[x]);

    select_handle(x, y, range.width, FONT_SIZE);

    while (range.y < FONT_SIZE)
    {
        range.height = min(FONT_SIZE-range.y, BF_LINES);
        fr.render(color, FONT_SIZE, text, RENDER_BF, LINE_PIXELS, range);
        write_handle(RENDER_BF, range.height*range.width);
        memset(RENDER_BF, 0x0, range.height*range.width*sizeof(color_t));
        range.y += range.height;
    }
}

void EasyRender::fill(uint16_t x, uint16_t y,
                      uint16_t w, uint16_t h, color_t color)
{
    int total = w * h, tx_count = 0; // 总长度 & 已发送长度
    int single_tx = LINE_PIXELS*BF_LINES; // 单次可以发送的量
    for (int x=0; x<single_tx; x++) RENDER_BF[x] = color;

    select_handle(x, y, w, h);

    while (tx_count < total)
    {
        int delta = min(total - tx_count, single_tx);
        write_handle(RENDER_BF, delta);
        tx_count += delta;    
    }
}