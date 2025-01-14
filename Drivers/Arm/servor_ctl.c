#include "servor_ctl.h"

#define ServorSpeed 300

// 爪子——4
float CLAW_ANGLE[] = {
    110.0f, // 一般状态
    110.0f, // 开
    25.0f,  // 关
    23.0f   // 完全关闭
};
servor_t CLAW_SERVOR = {.Num = 4, .Time = ServorSpeed, .angle = CLAW_ANGLE};

//// 前伸舵机——1
// servor_t Forward_Store = {
//	.Num = 1,
//	.Time = ServorSpeed,
//	//数值增大是往回缩
//	.Angle = {
//		144,	// 一般状态——d=0mm
//		90.0f, // 红
//		143.5,	// 绿
//		143.5f, // 蓝
//		100.0f, // 取圆盘上物块的位置
//		75.0f	// 完全伸出——d=115mm
//	}};
//// 摆放
// servor_t Forward_Place = {
//	.Num = 1,
//	.Time = ServorSpeed,
//	.Angle = {
//		144,		// 一般状态
//		85.0f, // 红
//		125.0f,	// 绿
//		95.0f, // 蓝
//	}};

// 左存储——0
float PLATE_LEFT_ANGLE[] = {
    170.0f, // 一般状态——减小则外展
    120.0f, // 开
    170.0f  // 关
};
servor_t Plate_L = {.Num = 0, .Time = ServorSpeed, .angle = PLATE_LEFT_ANGLE};

// 右存储——1
float PLATE_RIGHT_ANGLE[] = {
    0.0f,  // 一般状态
    51.0f, // 开
    0.0f   // 关
};
servor_t Plate_R = {.Num = 1, .Time = ServorSpeed, .angle = PLATE_RIGHT_ANGLE};

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