#include "arm_seqs.h"
#include "vision.h"
#include "driver_middle.h"
#include "servor_ctl.h"
#include "coordinate.h"

/**
 * @brief 平滑模式速度参数
 */
const static float ACC_Z_SMOOTH = 230; // 放物块时的加速度
const static float ACC_R_SMOOTH = 200; // 放物块时R的加速度
const static float ACC_X_SMOOTH = 235; // 放物块时X的加速度

// 就绪点位 ---------------------------------------------------------------------
const float POSITION_R_STANDBY = 0;
const float POSITION_X_STANDBY = 0;
const float POSITION_Z_STANDBY = -5;

// 扫描点位
const float POSITION_R_DETECT = -180;

// 扫描圆环点位
const float POSITION_X_RING_DETECT = 200;
const float POSITION_Z_SAFETY_FWD = -15;

// 翅膀点位 ---------------------------------------------------------------------
const float POSITION_R_WINGS[] = {
    POSITION_R_STANDBY,
    -70.2441406,	   // 红
    -82.4707031,	   // 绿
    -290.275391, // 蓝
};

const float POSITION_X_WINGS[] = {
    POSITION_X_STANDBY,
    7.67285156,	   // 红
    80.3896484,	   // 绿
    7.67285156,	   // 蓝
};

// 物料区夹取点位 ---------------------------------------------------------------
const float POSITION_R_MATERIALS[] = {
    -180,   // 圆盘中下位置
    -193.5,   // 圆盘左上位置
    -166.5,   // 圆盘右上位置
};

const float POSITION_X_MATERIALS[] = {
    70,
    230,	   // 圆盘左上
    230,	   // 圆盘右上
};

const float STEPMOTOR_R_POSITIONS[] = {
    POSITION_R_STANDBY,	   // 一般状态
    -70.2441406,	   // 红
    -82.4707031,	   // 绿
    -290.275391, // 蓝
    -180,   // 物料识别
    -180,   // 圆盘中下位置
    -193.5,   // 圆盘左上位置
    -166.5,   // 圆盘右上位置
    -360	   // 限位

};

const float STEPMOTOR_X_POSITIONS[] = {
    POSITION_X_STANDBY,	   // 一般状态
    7.67285156,	   // 红
    80.3896484,	   // 绿
    7.67285156,	   // 蓝
    70, // 圆盘中下
    230,	   // 圆盘左上
    230,	   // 圆盘右上
    115	   // 完全伸出
};

const float STEPMOTOR_Z_POSITIONS[] = {
    POSITION_Z_STANDBY, // 最高位置
    -50, // 圆盘夹取的高度
    -125, // 夹爪到地面高度
    -120,
    -50,	//码垛高度
    -40, // 放到存储机构的位置
    -16
};

// 新加
Point2f armposition = {.x = -194, .y = 52};
Point2f ringposition;

// 物品所在的位置
uint8_t item_index[4] = {0, };

void arm_ground_place(const Point3f *point, bool smooth)
{
#	ifdef __USING_FULL_FUNCTION_ARM_MOVE
    arm_move(pos, STEPMOTOR_Z_POSITIONS[2]);
#	else
    float ringangle;
    Point2f pos = {.x = point->x, .y = point->y};
    ringposition = coordinate_transform_XY_shift(pos, armposition);
    float xdistance = sqrtf(powf(ringposition.x, 2) + powf(ringposition.y, 2));
    xdistance -= pARM_DEFINE->claw_distance;
    if (ringposition.y > 0)
    {
        ringangle = 180 + (atan(fabs(ringposition.y / ringposition.x)) / 3.1415926 * 180);
    }
    else
        ringangle = 180 - (atan(fabs(ringposition.y / ringposition.x)) / 3.1415926 * 180);
    
    ringangle = -ringangle;
    if (smooth)
    {
        uint16_t accx = pARM_DEFINE->motor_x.acc;
        pARM_DEFINE->motor_x.acc = ACC_X_SMOOTH;
        uint16_t accr = pARM_DEFINE->motor_r.acc;
        pARM_DEFINE->motor_r.acc = ACC_R_SMOOTH;
        uint16_t accz = pARM_DEFINE->motor_z.acc;
        pARM_DEFINE->motor_z.acc = ACC_Z_SMOOTH;

        stepmotor_rotate(&pARM_DEFINE->motor_r, ringangle);
        HAL_Delay(200);
        stepmotor_move(&pARM_DEFINE->motor_x, xdistance);
        stepmotor_move(&pARM_DEFINE->motor_z, point->z);

        pARM_DEFINE->motor_x.acc = accx;
        pARM_DEFINE->motor_r.acc = accr;
        pARM_DEFINE->motor_z.acc = accz;
    } else {
        stepmotor_rotate(&pARM_DEFINE->motor_r, ringangle);
        HAL_Delay(150);
        stepmotor_move(&pARM_DEFINE->motor_x, xdistance);
        stepmotor_move(&pARM_DEFINE->motor_z, point->z);
    }
#	endif
}

void arm_claw_ctl(bool open_close)
{
    arm_move_sync();
    claw_ctl(open_close, true);
}

/**
 * @brief 抬起Z轴
 */
static inline void arm_Z_raise()
{
    stepmotor_move(&pARM_DEFINE->motor_z, POSITION_Z_SAFETY_FWD);
    stepmotor_arrived(&pARM_DEFINE->motor_z);
}

void arm_standby(void)
{
    wing_ctl(true, false);
    stepmotor_move(&pARM_DEFINE->motor_z, POSITION_Z_STANDBY);
    stepmotor_arrived(&pARM_DEFINE->motor_z);

    stepmotor_move(&pARM_DEFINE->motor_x, POSITION_X_STANDBY);
    stepmotor_move(&pARM_DEFINE->motor_r, POSITION_R_STANDBY);
    arm_move_sync();
}

void arm_item_detect(void)
{
    arm_claw_ctl(false);
    wing_ctl(false, false);

    stepmotor_move(&pARM_DEFINE->motor_x, POSITION_X_STANDBY);
    stepmotor_rotate(&pARM_DEFINE->motor_r, POSITION_R_DETECT);
    arm_move_sync();
}

void arm_cat_materials(uint8_t pos)
{
    stepmotor_move(&pARM_DEFINE->motor_x, POSITION_X_MATERIALS[pos]);
    stepmotor_move(&pARM_DEFINE->motor_r, POSITION_R_MATERIALS[pos]);
    stepmotor_move(&pARM_DEFINE->motor_z, STEPMOTOR_Z_POSITIONS[1]);

    arm_claw_ctl(true);
}

void arm_store_item(uint8_t index)
{
    stepmotor_move(&pARM_DEFINE->motor_x, POSITION_X_WINGS[index]);
    stepmotor_move(&pARM_DEFINE->motor_r, POSITION_R_WINGS[index]);
    arm_move_sync();

    HAL_Delay(100);
    
    stepmotor_move(&pARM_DEFINE->motor_z, STEPMOTOR_Z_POSITIONS[5]);
    stepmotor_arrived(&pARM_DEFINE->motor_z);
}

void arm_ring_detect(void)
{
    stepmotor_rotate(&pARM_DEFINE->motor_r, POSITION_R_DETECT);
    stepmotor_arrived(&pARM_DEFINE->motor_r);
    stepmotor_move(&pARM_DEFINE->motor_x, POSITION_X_RING_DETECT);
    stepmotor_move(&pARM_DEFINE->motor_z, POSITION_Z_SAFETY_FWD);
    arm_move_sync();
}

/*--------------------------运行控制--------------------------------------*/

void arm_action_get_materials(uint8_t pos, color_t color)
{
    // 圆盘抓取物料
    arm_claw_ctl(false);
    arm_cat_materials(pos);
    arm_claw_ctl(true); 

    // 更新物品所在位置
    item_index[color] = color;
    pos = item_index[color];

    // 存放
    arm_Z_raise();
    arm_store_item(pos); // 摆放到存储机构
    arm_claw_ctl(false); // 张爪
    
    arm_Z_raise();
    arm_item_detect(); // 归位为检测位置
}

void arm_action_store_to_ground(color_t color)
{
    uint8_t pos = item_index[color];
    arm_store_item(pos); // 移到存储处对应位置
    arm_claw_ctl(true); // 闭
    arm_Z_raise();

    // 移动到放置位置
    Point2f tmp = vision_get_ring(color);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = STEPMOTOR_Z_POSITIONS[2]};
    arm_ground_place(&target, true); // 移到对应圆环位置

    arm_claw_ctl(false); // 张爪
    arm_Z_raise();
}

void arm_action_palletising(color_t color)
{
    uint8_t pos = item_index[color];
    arm_store_item(pos); // 移到对应存储机构位置
    arm_claw_ctl(true); // 闭
    arm_Z_raise();

    // 移动到码垛位置
    Point2f tmp = vision_get_ring(ARM_TARGET_COLOR_MAP[color]);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = STEPMOTOR_Z_POSITIONS[4]};
    arm_ground_place(&target, true);

    HAL_Delay(500); // 稍微等一下，不然容易放不稳
    arm_claw_ctl(false); // 张爪
    arm_Z_raise();
}

void arm_action_ground_to_store(color_t color)
{
    Point2f tmp = vision_get_ring(color);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = STEPMOTOR_Z_POSITIONS[2]};
    arm_ground_place(&target, false); // 移到对应圆环位置

    arm_claw_ctl(true); // 闭
    stepmotor_move(&pARM_DEFINE->motor_x, POSITION_X_STANDBY);
    arm_Z_raise();

    uint8_t pos = item_index[color];
    arm_store_item(pos); // 移到对应存储机构位置
    arm_claw_ctl(false); // 张爪

    arm_Z_raise();
}

void arm_action_wing_test()
{
    arm_standby();
    wing_ctl(false, true);

    for (int x=1; x<=3; x++)
    {
        arm_claw_ctl(false); // 打开机械爪
        arm_store_item((color_t)x); // 移动到物品上
        arm_claw_ctl(true); // 夹住物品
        HAL_Delay(1000);
        arm_claw_ctl(true);
        arm_Z_raise(); // 抬起机械臂
    }
    wing_ctl(true, false);
}

// 机械臂总控
void Arm_Action(ArmAction action, ArmTarget color)
{
    Point2f point;
    color_t tmp = ARM_TARGET_COLOR_MAP[color];
    switch (action)
    {
    case Ordinary3: // 一般位置
        arm_item_detect(); 
        break;
    case Scan: // 扫码位置
        arm_standby();
        break;
    case Store_Scan:      // 存储扫描圆环
        arm_ring_detect();
        break;
    case Store_To_Ground: // 存储-->地面
        arm_action_store_to_ground(tmp);
        break;
    case Ground_To_Store: // 地面-->存储
        arm_action_ground_to_store(tmp); // Z抬高
        break;
    case Maduo: // 码垛
        arm_action_palletising(tmp);
        break;
    case Store_Open: // 存储机构开关
        wing_ctl(false, false);
        break;
    case Store_Close:
        wing_ctl(true, false);
        break;
    }
}