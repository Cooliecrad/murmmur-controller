const unsigned char FONTBASE[] = {
    0x00,0x00,0x00,0x00,0x58,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x0F,0xF0,0x00,0x00,0x00,0x00,0x3F,0xF0,0x00,0x00,0x00,0x00,0xFF,0xF0,0x00,0x00,
    0x00,0x03,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xFF,0xF0,0x00,0x00,0x00,0x3F,0xFF,0xF0,
    0x00,0x00,0x00,0xFF,0xFF,0xF0,0x00,0x00,0x03,0xFF,0xEF,0xF0,0x00,0x00,0x03,0xFF,
    0x8F,0xF0,0x00,0x00,0x03,0xFE,0x0F,0xF0,0x00,0x00,0x03,0xF8,0x0F,0xF0,0x00,0x00,
    0x01,0xE0,0x0F,0xF0,0x00,0x00,0x01,0x80,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,
    0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,
    0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,
    0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,
    0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,
    0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,
    0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,
    0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,
    0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFE,0x00,
    0x01,0xFF,0xFF,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0xFF,
    0xFE,0x00,0x01,0xFF,0xFF,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"1",0*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x03,
    0xFF,0xF0,0x00,0x00,0x00,0x0F,0xFF,0xFC,0x00,0x00,0x00,0x3F,0xFF,0xFF,0x00,0x00,
    0x00,0x7F,0xFF,0xFF,0x80,0x00,0x01,0xFF,0xFF,0xFF,0xC0,0x00,0x03,0xFF,0x80,0xFF,
    0xC0,0x00,0x01,0xFE,0x00,0x3F,0xE0,0x00,0x01,0xF8,0x00,0x1F,0xF0,0x00,0x00,0xE0,
    0x00,0x0F,0xF0,0x00,0x00,0x40,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x07,0xF0,0x00,
    0x00,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,0x00,0x07,
    0xF8,0x00,0x00,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,
    0x00,0x07,0xF0,0x00,0x00,0x00,0x00,0x07,0xF0,0x00,0x00,0x00,0x00,0x07,0xF0,0x00,
    0x00,0x00,0x00,0x0F,0xE0,0x00,0x00,0x00,0x00,0x0F,0xE0,0x00,0x00,0x00,0x00,0x1F,
    0xE0,0x00,0x00,0x00,0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,0x7F,0x80,0x00,0x00,0x00,
    0x00,0xFF,0x80,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x01,0xFE,0x00,0x00,
    0x00,0x00,0x03,0xFC,0x00,0x00,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,0x00,0x1F,0xF0,
    0x00,0x00,0x00,0x00,0x3F,0xE0,0x00,0x00,0x00,0x00,0x7F,0xC0,0x00,0x00,0x00,0x00,
    0xFF,0x80,0x00,0x00,0x00,0x01,0xFF,0x00,0x00,0x00,0x00,0x03,0xFE,0x00,0x00,0x00,
    0x00,0x07,0xFC,0x00,0x00,0x00,0x00,0x0F,0xF8,0x00,0x00,0x00,0x00,0x1F,0xE0,0x00,
    0x00,0x00,0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,0x7F,0x80,0x00,0x00,0x00,0x00,0xFF,
    0x00,0x00,0x00,0x00,0x01,0xFE,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFE,0x00,
    0x03,0xFF,0xFF,0xFF,0xFE,0x00,0x03,0xFF,0xFF,0xFF,0xFE,0x00,0x03,0xFF,0xFF,0xFF,
    0xFE,0x00,0x03,0xFF,0xFF,0xFF,0xFE,0x00,0x03,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"2",1*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x1F,
    0xFF,0xE0,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,
    0x00,0xFF,0xFF,0xFF,0x80,0x00,0x00,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0xF8,0x01,0xFF,
    0xC0,0x00,0x00,0x80,0x00,0x3F,0xE0,0x00,0x00,0x00,0x00,0x1F,0xE0,0x00,0x00,0x00,
    0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x07,0xF0,0x00,
    0x00,0x00,0x00,0x07,0xF0,0x00,0x00,0x00,0x00,0x07,0xF0,0x00,0x00,0x00,0x00,0x07,
    0xF0,0x00,0x00,0x00,0x00,0x07,0xF0,0x00,0x00,0x00,0x00,0x0F,0xE0,0x00,0x00,0x00,
    0x00,0x0F,0xE0,0x00,0x00,0x00,0x00,0x1F,0xC0,0x00,0x00,0x00,0x00,0x3F,0x80,0x00,
    0x00,0x00,0x00,0xFF,0x80,0x00,0x00,0x00,0x07,0xFE,0x00,0x00,0x00,0x07,0xFF,0xFC,
    0x00,0x00,0x00,0x07,0xFF,0xF0,0x00,0x00,0x00,0x07,0xFF,0xFE,0x00,0x00,0x00,0x07,
    0xFF,0xFF,0x80,0x00,0x00,0x07,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0xFF,0xE0,0x00,
    0x00,0x00,0x00,0x1F,0xF0,0x00,0x00,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,0x00,0x03,
    0xFC,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0x00,0x00,0x00,0x01,0xFC,0x00,0x00,0x00,
    0x00,0x01,0xFC,0x00,0x00,0x00,0x00,0x01,0xFC,0x00,0x00,0x00,0x00,0x01,0xFC,0x00,
    0x00,0x00,0x00,0x01,0xFC,0x00,0x00,0x00,0x00,0x01,0xFC,0x00,0x00,0x00,0x00,0x01,
    0xFC,0x00,0x00,0x00,0x00,0x03,0xF8,0x00,0x00,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,
    0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x1F,0xF0,0x00,0x03,0x00,0x00,0xFF,0xE0,0x00,
    0x03,0xFF,0xFF,0xFF,0xC0,0x00,0x03,0xFF,0xFF,0xFF,0x80,0x00,0x03,0xFF,0xFF,0xFE,
    0x00,0x00,0x03,0xFF,0xFF,0xF8,0x00,0x00,0x03,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x1F,
    0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"3",2*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,
    0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,
    0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,
    0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,
    0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,
    0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,
    0x1F,0x80,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0x80,0x1F,0xFF,0xFF,0xFF,0xFF,0x80,
    0x1F,0xFF,0xFF,0xFF,0xFF,0x80,0x1F,0xFF,0xFF,0xFF,0xFF,0x80,0x1F,0xFF,0xFF,0xFF,
    0xFF,0x80,0x1F,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,
    0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,
    0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,
    0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,
    0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,
    0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x1F,0x80,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"+",3*/
};

