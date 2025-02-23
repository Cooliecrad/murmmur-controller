#include "servor_ctl.h"


// 爪子——4
float CLAW_ANGLE[] = {
    90.0f, // 一般状态
    21.0f,  // 关
    21.0f   // 完全关闭
};
servor_t CLAW_SERVOR = {.Num = 3, .Time = 300, .angle = CLAW_ANGLE};


// 左存储——0
float PLATE_LEFT_ANGLE[] = {
    170.0f, // 一般状态——减小则外展
    74.0f, // 开
    170.0f  // 关
};
servor_t Plate_L = {.Num = 0, .Time = 700, .angle = PLATE_LEFT_ANGLE};

// 右存储——1
float PLATE_RIGHT_ANGLE[] = {
    0.0f,  // 一般状态
    96.f, // 开
    0.0f   // 关
};
servor_t Plate_R = {.Num = 1, .Time = 700, .angle = PLATE_RIGHT_ANGLE};

void servor_ctl_init(UART_HandleTypeDef *pHUART)
{
    servor_init(pHUART);
}

void servor_ctl(servor_object_t object, int state, bool sync)
{
    switch (object)
    {
        case servor_object_CLAW:
            servor_rotate(CLAW_SERVOR.Time, CLAW_SERVOR.Num,
                          CLAW_SERVOR.angle[(int)state]);
            if (sync) HAL_Delay(1.5*CLAW_SERVOR.Time);
        break;
    case servor_object_PLATES:
        servor_rotate_2(Plate_L.Time, Plate_L.Num, Plate_L.angle[state],
                        Plate_R.Num, Plate_R.angle[state]);
        if (sync) HAL_Delay(1.5*Plate_R.Time);
        break;
    }
}