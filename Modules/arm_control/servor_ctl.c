#include "servor_ctl.h"

/**
 * @brief 具体舵机控制
 */
typedef struct
{
    uint8_t Num; // 编号
    uint8_t status; // 当前处在的位置
    uint16_t Time; // 单次舵机操作的时间
    float *angle; // 可用位置数组
} servor_t;

// 爪子——4
float CLAW_ANGLE[] = {
    90.0f, // 一般状态
    17.0f,  // 关
};
servor_t CLAW_SERVOR = {.Num = 0, .Time = 300, .angle = CLAW_ANGLE};

// 左存储——0
float PLATE_LEFT_ANGLE[] = {
    175.0f, // 一般状态——减小则外展
    90.0f, // 开
};
servor_t Plate_L = {.Num = 2, .Time = 700, .angle = PLATE_LEFT_ANGLE};

// 右存储——1
float PLATE_RIGHT_ANGLE[] = {
    5.0f, // 关
    90.f, // 开
};
servor_t Plate_R = {.Num = 1, .Time = 700, .angle = PLATE_RIGHT_ANGLE};


void wing_ctl(bool open_close, bool sync)
{
    uint8_t new_status = open_close ? 0 : 1;
    if (Plate_L.status != new_status)
    {
        Plate_L.status = new_status;
        Plate_R.status = new_status;
        servor_rotate_2(Plate_L.Time, sync,
                        Plate_L.Num, Plate_L.angle[new_status],
                        Plate_R.Num, Plate_R.angle[new_status]);
    }
}

void claw_ctl(bool close_open, bool sync)
{
    uint8_t new_status = close_open ? 1 : 0;
    if (CLAW_SERVOR.status != new_status)
    {
        CLAW_SERVOR.status = new_status;
        servor_rotate(CLAW_SERVOR.Time, sync, CLAW_SERVOR.Num,
                      CLAW_SERVOR.angle[new_status]);
        if (sync) HAL_Delay(0.5*CLAW_SERVOR.Time); // 多等会爪子
    }
}