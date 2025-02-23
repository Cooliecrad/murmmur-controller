#include "Arm.h"
#include "servor_ctl.h"
#include "coordinate.h"
#include "vision.h"
#include "driver_middle.h"

#define MotorSpeed 1500
/*----------------------步进电机参数部分----------------------*/
const static float PULSE_PER_ROUND = 3200;

emm42_v5_t ARM_EMM42;

/**
 * @brief 平滑模式速度参数
 */
const static float ACC_Z_SMOOTH = 230; // 放物块时的加速度
const static float ACC_R_SMOOTH = 200; // 放物块时R的加速度
const static float ACC_X_SMOOTH = 235; // 放物块时X的加速度

arm_define_t ARM_DEFINE = {
    .emm42_handle = &ARM_EMM42,
    .center = {
        .x = -197,
        .y = 48
    },
    .claw_distance = 130,
    .PULSE_PER_ROUND = 3200,
    .deadzone = {
        .lower = {.x = -100, .y = -100},
        .upper = {.x = 20, .y = 100},
        .inner_max = 15,
        .outer_max = 100
    },
    .motor_r = {
        .ID = 1,
        .ratio = 1. / 360. * 90. / 32. * PULSE_PER_ROUND, // 25
        .acc = 225,
        .speed = 2100,
        .zero_shift = 0,
        .lower = -360,
        .upper = 360
    },

    .motor_x = {
        .ID = 3,
        .ratio = 50,
        .acc = 250,
        .speed = 2300,
        .zero_shift = 0,
        .lower = 0,
        .upper = 300,
    },

    .motor_z = {
        .ID = 2,
        .ratio = 100,
        .acc = 250,
        .speed = 2300,
        .zero_shift = 0,
        .lower = -140,
        .upper = 0,
    }
};
arm_define_t *pARM_DEFINE = &ARM_DEFINE;

// 减速比好像是1:3
// 存储
float StepMotor_R_StorePosition[] = {
    0,	   // 一般状态
    -70.2441406,	   // 红
    -82.4707031,	   // 绿
    -290.275391, // 蓝
    -180,   // 物料识别
    -180,   // 圆盘中下位置
    -193.5,   // 圆盘左上位置
    -166.5,   // 圆盘右上位置
    -360	   // 限位

};

float StepMotor_Forward_Store[] = {
    0,	   // 一般状态
    7.67285156,	   // 红
    80.3896484,	   // 绿
    7.67285156,	   // 蓝
    70, // 圆盘中下
    230,	   // 圆盘左上
    230,	   // 圆盘右上
    115	   // 完全伸出
};

float StepMotor_Z_Position[] = {
    -10, // 最高位置
    -50, // 圆盘夹取的高度
    -125, // 夹爪到地面高度
    -120,
    -50,	//码垛高度
    -40, // 放到存储机构的位置
    -16
};

/*---------------步进电机控制部分---------------------*/
// 绝对位置控制
//  0--地址
//  1--0xFD
//  2--方向
//  3--速度高八位
//  4--速度低八位
//  5--加速度
//  6--脉冲
//  7--脉冲
//  8--脉冲
//  9--脉冲
//  10--相对/绝对--1绝对，0相对
//  11--多机同步标志
//  12--校验字节

// 供通信使用
uint8_t ARM_RECV_BUFFER0[4];
uint8_t ARM_RECV_BUFFER1[4];
uint8_t ARM_RECV_BUFFER2[4];

ps_uart_t ARM_PS_UART;

void arm_ctl_init(UART_HandleTypeDef *pHUART)
{
    // 初始化串口
    ARM_PS_UART.buffer[0] = ARM_RECV_BUFFER0;
    ARM_PS_UART.buffer[1] = ARM_RECV_BUFFER1;
    ARM_PS_UART.buffer[2] = ARM_RECV_BUFFER2;
    ARM_PS_UART.recv_len = sizeof(ARM_RECV_BUFFER0);
    ARM_PS_UART.pHUART = pHUART;
    ARM_PS_UART.recv_callback = NULL;
    ARM_EMM42.ps_uart_handle = &ARM_PS_UART;

    // 初始化步进电机结构体
    arm_define_t *arm = pARM_DEFINE;
    arm->deadzone.inner = true;
    arm->motor_x.instance.range = arm->motor_x.upper - arm->motor_x.lower;
    arm->motor_z.instance.range = arm->motor_z.upper - arm->motor_z.lower;
    arm->motor_r.instance.range = arm->motor_r.upper - arm->motor_r.lower;

    // 初始化电机
    emm42_v5_init(&ARM_EMM42);
    arm_set_state(true);
//    emm42_set_reach_wnd(&ARM_EMM42, 1, 1);
//    emm42_set_reach_wnd(&ARM_EMM42, 2, 1);
//    emm42_set_reach_wnd(&ARM_EMM42, 3, 1);
//    arm_position_update();
}

void arm_set_state(bool state)
{
    emm42_set_state(&ARM_EMM42, ARM_DEFINE.motor_r.ID, state, 0);
    emm42_set_state(&ARM_EMM42, ARM_DEFINE.motor_x.ID, state, 0);
    emm42_set_state(&ARM_EMM42, ARM_DEFINE.motor_z.ID, state, 0);
}

void arm_position_update(void)
{
    stepmotor_position_update(&ARM_EMM42, PULSE_PER_ROUND, &ARM_DEFINE.motor_r);
    stepmotor_position_update(&ARM_EMM42, PULSE_PER_ROUND, &ARM_DEFINE.motor_x);
    stepmotor_position_update(&ARM_EMM42, PULSE_PER_ROUND, &ARM_DEFINE.motor_z);
}

void stepmotor_halt(stepmotor_t *motor)
{
    emm42_halt(ARM_DEFINE.emm42_handle, motor->ID, 0);
}

static inline void stepmotor_arrived(stepmotor_t *motor)
{   
    emm42_arrived(ARM_DEFINE.emm42_handle, motor->ID);
}

/**
 * @brief 驱动特定电机移动到指定位置
 */
static inline void __stepmotor_move(stepmotor_t *motor, float position)
{
    // 计算实际移动距离
    float distance = position - motor->position;
    motor->position = position;
    // 发送到驱动
    uint8_t dir = position >= 0 ? MOTOR_FORWARD : MOTOR_BACKWARD;
    uint32_t step = fabs(position) * motor->ratio;
    stepmotor_arrived(motor); // 确保到位
    emm42_pos_ctl(ARM_DEFINE.emm42_handle, motor->ID, dir, motor->speed,
                  motor->acc, step, 1, 0);
}

void stepmotor_move(stepmotor_t *motor, float position)
{
    // 限制移动范围
    if (position < motor->lower)
    {
        position = motor->lower;
    }
    else if (position > motor->upper)
    {
        position = motor->upper;
    }

    // 移动补偿：由于步进电机零点会偏移，所以有一个抽象零点
    // 实现这个抽象零点依靠的是零点偏移补偿
    position -= motor->zero_shift;

    __stepmotor_move(motor, position);
}

void stepmotor_rotate(stepmotor_t *motor, float position)
{
    // 取值 (0, upper-lower)
    position = fmodf(position, 360.);
    // 越界要退一圈
    if (position > motor->upper)
        position -= 360;
    else if (position < motor->lower)
        position += 360;

    // 移动补偿：由于步进电机零点会偏移，所以有一个抽象零点
    // 实现这个抽象零点依靠的是零点偏移补偿
    position -= motor->zero_shift;

    __stepmotor_move(motor, position);
}

void arm_move(Point2f XY, float Z)
{
    // 变换XY坐标，求出前伸量和旋转角度
    XY = coordinate_transform_XY_shift(XY, ARM_DEFINE.center);
    float xdistance = sqrtf(powf(XY.x, 2) + powf(XY.y, 2)) - ARM_DEFINE.claw_distance;
    float angle = (point_to_angle(XY) / PI * 180) + 180; // 机械臂的零点和X轴间隔180度

    // 检测目标是否在死区，并限制Z轴
    bool inner = false; // 本次是否移动到死区内
    if (point2f_in_range(XY, ARM_DEFINE.deadzone.lower, ARM_DEFINE.deadzone.upper))
    {
        ARM_DEFINE.motor_z.upper = ARM_DEFINE.deadzone.inner_max;
        inner = true;
    } else
        ARM_DEFINE.motor_z.upper = ARM_DEFINE.deadzone.outer_max;
    
    // 如果从死区外移动到死区内，先升起机械臂
    bool first_in = false;
    if (inner == true && ARM_DEFINE.deadzone.inner == false)
    {
        first_in = true;
        stepmotor_move(&ARM_DEFINE.motor_z, Z);
        stepmotor_arrived(&ARM_DEFINE.motor_z); // 必须确保升起来
    }
    // 定位
    stepmotor_move(&ARM_DEFINE.motor_x, xdistance);
    stepmotor_rotate(&ARM_DEFINE.motor_r, angle);
    // 除开上面那种情况，其他的都是最后移动机械臂
    if (!first_in)
    {
        stepmotor_arrived(&ARM_DEFINE.motor_x);
        stepmotor_arrived(&ARM_DEFINE.motor_r);
        stepmotor_move(&ARM_DEFINE.motor_z, Z);
    }
    ARM_DEFINE.deadzone.inner = inner;
}

// 新加
Point2f armposition = {.x = -198.75, .y = 48};
Point2f ringposition;

void arm_ground_place(const Point3f *point, bool smooth)
{
#	ifdef __USING_FULL_FUNCTION_ARM_MOVE
    arm_move(pos, StepMotor_Z_Position[2]);
#	else
    float ringangle;
    Point2f pos = {.x = point->x, .y = point->y};
    ringposition = coordinate_transform_XY_shift(pos, armposition);
    float xdistance = sqrtf(powf(ringposition.x, 2) + powf(ringposition.y, 2));
    xdistance -= ARM_DEFINE.claw_distance;
    if (ringposition.y > 0)
    {
        ringangle = 180 + (atan(fabs(ringposition.y / ringposition.x)) / 3.1415926 * 180);
    }
    else
        ringangle = 180 - (atan(fabs(ringposition.y / ringposition.x)) / 3.1415926 * 180);
	
    ringangle = -ringangle;
    if (smooth)
    {
        uint16_t accx = ARM_DEFINE.motor_x.acc;
        ARM_DEFINE.motor_x.acc = ACC_X_SMOOTH;
        uint16_t accr = ARM_DEFINE.motor_r.acc;
        ARM_DEFINE.motor_r.acc = ACC_R_SMOOTH;
        uint16_t accz = ARM_DEFINE.motor_z.acc;
        ARM_DEFINE.motor_z.acc = ACC_Z_SMOOTH;

        stepmotor_rotate(&ARM_DEFINE.motor_r, ringangle);
        HAL_Delay(200);
        stepmotor_move(&ARM_DEFINE.motor_x, xdistance);
        stepmotor_move(&ARM_DEFINE.motor_z, point->z);

        ARM_DEFINE.motor_x.acc = accx;
        ARM_DEFINE.motor_r.acc = accr;
        ARM_DEFINE.motor_z.acc = accz;
    } else {
        stepmotor_rotate(&ARM_DEFINE.motor_r, ringangle);
        HAL_Delay(150);
        stepmotor_move(&ARM_DEFINE.motor_x, xdistance);
        stepmotor_move(&ARM_DEFINE.motor_z, point->z);
    }
#	endif
}

void arm_move_sync(void)
{
    stepmotor_arrived(&ARM_DEFINE.motor_r);
    stepmotor_arrived(&ARM_DEFINE.motor_x);
    stepmotor_arrived(&ARM_DEFINE.motor_z);
}

void arm_claw_ctl(bool open_close)
{
    arm_move_sync();
    servor_ctl(servor_object_CLAW, open_close ? 1 : 0, true);
    arm_move_sync();
}

/*
机械臂扫码位置
Claw——张开
Forward——0
R——0
Z——最高处
*/
void Arm_Scan(void)
{
	stepmotor_move(&ARM_DEFINE.motor_z, StepMotor_Z_Position[0]);
    stepmotor_arrived(&ARM_DEFINE.motor_z);
	
    stepmotor_move(&ARM_DEFINE.motor_x, StepMotor_Forward_Store[0]);
    stepmotor_arrived(&ARM_DEFINE.motor_x);

    stepmotor_move(&ARM_DEFINE.motor_r, StepMotor_R_StorePosition[0]);
    stepmotor_arrived(&ARM_DEFINE.motor_r);
}

/**
 * @brief 机械臂到达识别物料位置
 */
void Arm_Dtct(void)
{
    stepmotor_move(&ARM_DEFINE.motor_z, StepMotor_Z_Position[0]);
    stepmotor_arrived(&ARM_DEFINE.motor_z);

    stepmotor_move(&ARM_DEFINE.motor_x, StepMotor_Forward_Store[0]);
    stepmotor_move(&ARM_DEFINE.motor_r, StepMotor_R_StorePosition[4]);

    arm_move_sync();
}

/*
机械臂从圆盘抓取物料位置
R——180
Z——从圆盘上取
*/

// vision_captureposition:5中下，6左上，7右上
void Arm_PlateCapture(uint16_t vision_captureposition)
{
    stepmotor_rotate(&ARM_DEFINE.motor_r, StepMotor_R_StorePosition[vision_captureposition]);
    stepmotor_arrived(&ARM_DEFINE.motor_r);

    stepmotor_move(&ARM_DEFINE.motor_x, StepMotor_Forward_Store[vision_captureposition - 1]);
    stepmotor_arrived(&ARM_DEFINE.motor_x);

    stepmotor_move(&ARM_DEFINE.motor_z, StepMotor_Z_Position[1]);
    stepmotor_arrived(&ARM_DEFINE.motor_z);
}

/*
机械臂将物料存放到对应存储机构
*/
void Arm_Store(ArmTarget color)
{
    stepmotor_move(&ARM_DEFINE.motor_z, StepMotor_Z_Position[0]);
    stepmotor_arrived(&ARM_DEFINE.motor_z);
    
    stepmotor_move(&ARM_DEFINE.motor_x, StepMotor_Forward_Store[color]);

    stepmotor_move(&ARM_DEFINE.motor_r, StepMotor_R_StorePosition[color]);
    arm_move_sync();

   HAL_Delay(100);
    
    stepmotor_move(&ARM_DEFINE.motor_z, StepMotor_Z_Position[5]);
    stepmotor_arrived(&ARM_DEFINE.motor_z);
}

/*
Z轴归中
*/
void Arm_Z_Middle(void)
{
    stepmotor_move(&ARM_DEFINE.motor_z, StepMotor_Z_Position[0]);
    //	StepMotor_ToStepData(&ARM_DEFINE.motor_z, StepMotor_Z_Position[0], MotorSpeed, 1);
    //	StepMotor_ToStepControl(&ARM_DEFINE.motor_z);
    //	stepmotor_arrived(&ARM_DEFINE.motor_z,WaitFlag);
}

void Arm_X_Forward(void)
{
    stepmotor_move(&pARM_DEFINE->motor_x, 200);
    stepmotor_move(&pARM_DEFINE->motor_z, -15);
    stepmotor_rotate(&pARM_DEFINE->motor_r, -180);
    arm_move_sync();
}

void Arm_X_Zip(void)
{
    stepmotor_move(&pARM_DEFINE->motor_x, 10);
}

/*--------------------------运行控制--------------------------------------*/

/**
 * @brief 检测位置 -> 圆盘抓取物料 -> 存放 -> 检测位置
 */
void Arm_Action_GetPlate(ArmTarget color, uint16_t vision_captureposition)
{
    Arm_PlateCapture(vision_captureposition);

    arm_claw_ctl(true); // 闭爪
    Arm_Z_Middle();					   // Z抬高
    stepmotor_arrived(&pARM_DEFINE->motor_z);

    Arm_Store(color);				   // 摆放到存储机构

    arm_claw_ctl(false); // 张爪
    Arm_Dtct();						   // 归位为检测位置
}

/**
 * @brief 就绪位置 -> 存储机构取出物料 -> 放置在圆环 -> 就绪位置
 */
void Arm_Action_Store_To_Ground(ArmTarget color)
{
    Arm_Store(color);							  // 移到存储处对应位置
    arm_claw_ctl(true);            // 闭
    Arm_Z_Middle();
    stepmotor_arrived(&pARM_DEFINE->motor_z);

    // 移动到放置位置
    Point2f tmp = vision_get_ring(ARM_TARGET_COLOR_MAP[color]);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = StepMotor_Z_Position[2]};
    arm_ground_place(&target, true); // 移到对应圆环位置

    arm_claw_ctl(false);			  // 张爪
    Arm_Z_Middle();
    stepmotor_arrived(&pARM_DEFINE->motor_z);
}

/**
 * @brief 就绪位置 -> 取出物品 -> 码垛 -> 就绪位置
 */
void Arm_Action_Maduo(ArmTarget color)
{
    Arm_Store(color);				   // 移到对应存储机构位置
    arm_claw_ctl(true); // 闭
    Arm_Z_Middle();					   // Z抬高
    stepmotor_arrived(&pARM_DEFINE->motor_z);

    // 移动到码垛位置
    Point2f tmp = vision_get_ring(ARM_TARGET_COLOR_MAP[color]);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = StepMotor_Z_Position[4]};
    arm_ground_place(&target, false);

    HAL_Delay(500); // 稍微等一下，不然容易放不稳
    arm_claw_ctl(false); // 张爪
    Arm_Z_Middle();
    stepmotor_arrived(&pARM_DEFINE->motor_z);
}

/**
 * @brief 就绪位置 -> 地面拾取物料 -> 存放到存储机构 -> 就绪位置
 */
void Arm_Action_Ground_To_Store(ArmTarget color)
{
    Point2f tmp = vision_get_ring(ARM_TARGET_COLOR_MAP[color]);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = StepMotor_Z_Position[2]};
    arm_ground_place(&target, false); // 移到对应圆环位置

    arm_claw_ctl(true);			  // 闭

    Arm_Z_Middle();
    Arm_X_Zip();
    arm_move_sync();

    Arm_Store(color);							  // 移到对应存储机构位置
    arm_claw_ctl(false);			  // 张爪

    Arm_Z_Middle();
    arm_move_sync();
}

// 机械臂总控
void Arm_Action(ArmAction action, ArmTarget color)
{
    Point2f point;
    switch (action)
    {
    case Ordinary3: // 一般位置
        Arm_Dtct(); 
        break;
    case Scan: // 扫码位置
        Arm_Scan();
        break;
    case Dtct: // 在圆盘上检测位置
        Arm_Dtct();
        break;
    case Plate_To_Store:  // 圆盘-->存储
        Arm_Action_GetPlate(color,vision_info.item_detect);// 移到对应存储机构位置
                          //		Arm_Action_GetPlate(color);
        break;
    case Store_Scan:      // 存储扫描圆环
        Arm_X_Forward();
        break;
    case Store_To_Ground: // 存储-->地面
        Arm_Action_Store_To_Ground(color);
        break;
    case Ground_To_Store: // 地面-->存储
        Arm_Action_Ground_To_Store(color); // Z抬高
        break;
    case Maduo: // 码垛
        Arm_Action_Maduo(color);
        break;
    case Store_Open: // 存储机构开关
        servor_ctl(servor_object_PLATES, 1, false);
        ;
        break;
    case Store_Close:
        servor_ctl(servor_object_PLATES, 2, false);
        ;
        break;
    }
}
