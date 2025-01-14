#ifndef __PS_FONT_RENDER_H__
#define __PS_FONT_RENDER_H__

#include <cstdlib>
#include <cstdint>
#include <string>

#define __PSAF_MIN_FONT_SIZE 12 // 当前字库驱动的最小字号
#define __PSAF_MAX_FONT_SIZE 128 // 当前字库驱动的最大字号，决定颜色缓冲区大小

namespace ps
{
namespace font
{

using color_t = uint16_t;
using char_size_t = uint8_t; // 单个字体最大大小，2^8 = 256号
using font_size_t = uint16_t; // 字库中最大的字符对应的编号大小，例如ascii对应uint8_t

struct PxBlock
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t width = 0;
    int16_t height = 0;
};

struct CharBlock : public PxBlock
{
    CharBlock(color_t c, font_size_t s, PxBlock b) : PxBlock {b}, color {c}, font_num {s} {}
    color_t color;
    font_size_t font_num;
};


class CharAdapter
{
public:
    CharAdapter(char_size_t f_size);
    ~CharAdapter();
    void adapt(color_t *raw_bf, int line_w, int height);
    void shift(int x_shift);
    color_t **buffer = NULL;
private:
    int height = 0;
};

/**
 * @brief 最大支持256号的字体
 */
class CharLineFont
{
public:
    struct char_line
    // 拷贝控制块
    {
        char_size_t start; // 线的起始位置
        char_size_t end; // 线的长度
    };

    /**
     * @brief 加载字库，需要指定字库指针和字库的字号
     */
    CharLineFont(const uint8_t *src, char_size_t size);
    void write(const CharBlock& c_block, color_t **dst);
    const int f_size;
private:
    const char_line *src;
    const int line_shift; // 字一行所拥有的拷贝控制块
    const int font_shift; // 一个字所拥有的拷贝控制块
};


class BitmapFont
{
public:
    /**
     * @brief 加载字库，需要指定字库指针和字库的字号
     */
    BitmapFont(const uint8_t *src, char_size_t size);
    void write(const CharBlock& c_block, color_t **dst);
    const int f_size;
private:
    const uint8_t *src;
    const int line_shift; // 字一行对应的比特数
    const int font_shift; // 一个字所拥有的比特数
};


/**
 * @brief 加载字库的类
 */
template<typename _Ft>
class FontBase
{
public:
    /**
     * @brief 字库分为多个字号区，每一个区域都有一个头结构
     */
    struct font_sector
    {
        size_t next_size; // 距离下一个字号区的偏移量，单位Byte
        char_size_t font_size; // 当前字体的大小
        uint8_t first_char; // 第一个字体的第一个单元
    };

    /**
     * @brief 加载字库
     * @param src 字库入口地址
     */
    FontBase(const uint8_t *src)
    {
        if (src == nullptr)
        {
            // printf("[ERROR] Invalid font-base start at '%p'\n", src);
            return;
        }

        // 计算字号表的最大和最小字号
        font_sector *fb_ptr = (font_sector*)src;
        size_t total_shift = 0;
        bool flag = true; // 只有遇到过一次next_size==0且执行完毕，才下一个

        while (flag)
        {
            if (  fb_ptr->font_size < __PSAF_MIN_FONT_SIZE
                ||fb_ptr->font_size > __PSAF_MAX_FONT_SIZE)
            { // 小于最小字号或者大于最大字号，认为无效
                // printf("[ERROR] Invalid font size '%d', 位于+%ud\n",
                    //    fb_ptr->font_size, total_shift);
            } else
            {
                min_size = std::min<char_size_t>(fb_ptr->font_size, min_size);
                max_size = std::max<char_size_t>(fb_ptr->font_size, max_size);
            }
            // 迭代控制
            if (fb_ptr->next_size == 0) flag = false;
            total_shift += fb_ptr->next_size;
            fb_ptr = (font_sector*)(src+total_shift);
        }
        font_size_table = (_Ft**)calloc(max_size-min_size+1, sizeof(_Ft*));

        // 划分字号区
        fb_ptr = (font_sector*)src, total_shift = 0;
        flag = true;
        while (flag)
        {
            if (   fb_ptr->font_size >= __PSAF_MIN_FONT_SIZE
                && fb_ptr->font_size <= __PSAF_MAX_FONT_SIZE)
            {
                font_size_table[fb_ptr->font_size - min_size] = new _Ft {&fb_ptr->first_char,
                                                                         fb_ptr->font_size};
                // printf("[INFO] Font loaded, size = %d\n", fb_ptr->font_size);
            }
            // 迭代控制
            if (fb_ptr->next_size == 0) flag = false;
            total_shift += fb_ptr->next_size;
            fb_ptr = (font_sector*)(src+total_shift);
        }
    }

    char_size_t min() {return min_size;}
    char_size_t max() {return max_size;}
    // 字库中是否有特定字号
    bool has_size(char_size_t size) {
        return    (size >= min_size && size <= max_size)
               && font_size_table[size - min_size] != nullptr;
    }
    _Ft& operator[](int index) {return *font_size_table[index-min_size];}
private:
    char_size_t min_size = -1, max_size = 0; // 字号范围
    _Ft** font_size_table; // 字号区映射表
};


/**
 * @brief 字体渲染器类
 */
class FontRender
{
public:
    using _Ft = BitmapFont;

    FontRender(const uint8_t *src) : fb {src}, adapter {fb.max()} {}

    void render(color_t color, char_size_t size, const char *text,
                color_t *raw_bf, int bf_width, PxBlock block);

    // 将原字符转换成目标字库编码
    font_size_t latin_cvt(char_size_t size, const char src)
	{
		switch (src)
		{
			case '1':
				return 0;
			case '2':
				return 1;
			case '3':
				return 2;
			case '+':
				return 3;
		}
        return 0;
	}
private:
    FontBase<_Ft> fb;
    CharAdapter adapter;
};

}

}
#endif