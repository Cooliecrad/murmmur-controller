#include "servor_ctl.h"

#define ServorSpeed 300

// 爪子——4
float CLAW_ANGLE[] = {
    90.0f, // 一般状态
    90.0f, // 开
    21.0f,  // 关
    21.0f   // 完全关闭
};
servor_t CLAW_SERVOR = {.Num = 7, .Time = ServorSpeed, .angle = CLAW_ANGLE};


// 左存储——0
float PLATE_LEFT_ANGLE[] = {
    170.0f, // 一般状态——减小则外展
    80.0f, // 开
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

// 伸长舵机
// 存储
// void Forward_Store_Control(ArmTarget color)
//{
//	Servor_Packet[7] = Forward_Store.Num;
//	Servor_Packet[5] = (uint8_t)(Forward_Store.Time);
//	Servor_Packet[6] = (uint8_t)(Forward_Store.Time >> 8);
//	Servor_Packet[8] = (uint8_t)(Angle_To_Data(Forward_Store.Angle[color]));
//	Servor_Packet[9] = (uint8_t)(Angle_To_Data(Forward_Store.Angle[color]) >> 8);
//	HAL_UART_Transmit(&huart5, Servor_Packet, 10, 1000);
//}
// 摆放
// void Forward_Place_Control(ArmTarget color)
//{
////	Servor_Packet[7] = Forward_Place.Num;
////	Servor_Packet[5] = (uint8_t)(Forward_Place.Time);
////	Servor_Packet[6] = (uint8_t)(Forward_Place.Time >> 8);
////	Servor_Packet[8] = (uint8_t)(Angle_To_Data(Forward_Place.Angle[color]));
////	Servor_Packet[9] = (uint8_t)(Angle_To_Data(Forward_Place.Angle[color]) >> 8);
////	HAL_UART_Transmit(&huart5, Servor_Packet, 10, 1000);
//
//}

void servor_ctl_init(UART_HandleTypeDef *pHUART)
{
    servor_init(pHUART);
}

void servor_ctl(servor_object_t object, int state)
{
    arm_move_sync();
    HAL_Delay(500);
    switch (object)
    {
    case servor_object_CLAW:
        servor_rotate(CLAW_SERVOR.Time, CLAW_SERVOR.Num,
                      CLAW_SERVOR.angle[(int)state]);
        break;
    case servor_object_PLATES:
        servor_rotate_2(Plate_L.Time, Plate_L.Num, Plate_L.angle[state],
                        Plate_R.Num, Plate_R.angle[state]);
        break;
    }
}