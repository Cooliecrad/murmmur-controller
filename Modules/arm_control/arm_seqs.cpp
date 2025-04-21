#include "arm_seqs.h"
#include "vision.h"
#include "driver_middle.h"
#include "servor_ctl.h"
#include "coordinate.h"

/**
 * @brief 平滑模式速度参数
 */
const static float ACC_R_SMOOTH = 235; // 放物块时R的加速度
const static float ACC_Z_SMOOTH = 240; // 放物块时Z的加速度
const static float ACC_X_SMOOTH = 245; // 放物块时X的加速度

// 就绪点位 ---------------------------------------------------------------------
const float POSITION_R_STANDBY = 0;
const float POSITION_X_STANDBY = 0;
const float POSITION_Z_STANDBY = -30;
// // 掀起盖子点位
// const float POSITION_R_LEFT_LF = 0;
// const float POSITION_R_RIGHT_LF = 0;
// const float POSITION_X_LIFT_OFF = 0;
// const float POSITION_Z_LIFT_OFF_SRC = 0;
// const float POSITION_Z_LIFT_OFF_DST = 0;

// 扫描点位
const float POSITION_R_DETECT = -180;
const float POSITION_Z_DETECT = 0;

// 扫描圆环点位
const float POSITION_X_RING_DETECT = 0;

// 翅膀点位 ---------------------------------------------------------------------
const float POSITION_R_WINGS[] = {
    -67.3127899, // 右内
    -73.9450226, // 右外
    -296.06308, // 左内
    -290.390625, // 左外
};

const float POSITION_X_WINGS[] = {
    8.43945313,
    92.359375,
    6.33789063,
    89.0488281,
};

// 物料区夹取点位 ---------------------------------------------------------------
const float POSITION_R_MATERIALS[] = {
    -187.533112,   // 圆盘中下位置
    -160.927734,   // 圆盘左上位置
    -169.105957,   // 圆盘右上位置
};

const float POSITION_X_MATERIALS[] = {
    26.4375,
    192.564453,	   // 圆盘左上
    183.085938,	   // 圆盘右上
};

const float STEPMOTOR_R_POSITIONS[] = {
    POSITION_R_STANDBY,	   // 一般状态
    -67.8638687,	   // 红
    -74.1761703,	   // 绿
    -296.44101, // 蓝
    -180,   // 物料识别
    -180,   // 圆盘中下位置
    -193.5,   // 圆盘左上位置
    -166.5,   // 圆盘右上位置
    -360	   // 限位
};

const float STEPMOTOR_X_POSITIONS[] = {
    POSITION_X_STANDBY,	   // 一般状态
    9.88867188,	   // 红
    91.6523438,	   // 绿
    9.078125,	   // 蓝
    70, // 圆盘中下
    230,	   // 圆盘左上
    230,	   // 圆盘右上
    115	   // 完全伸出
};

const float STEPMOTOR_Z_POSITIONS[] = {
    POSITION_Z_STANDBY, // 最高位置
    -103.787842, // 圆盘夹取的高度
    -200, // 夹爪到地面高度
    -120,
    -105,	//码垛高度
    -92.6025391, // 放到存储机构的位置
    -16
};


// 新加
Point2f VISION_ATTR = {.x = -197, .y = -9};
Point2f CLAW_ATTR = {.x = 147, .y = 6.5};

namespace
{
    const int STOARGE_COUNT = 4; // 储存机构容量
    /**
     * @brief 颜色 -> 位置映射
     * @note 物品位置定义：0：左内 1：左外 2：右内 3：右外
     */
    uint8_t ITEM_COLOR_POS[ITEM_COUNT+1] = {0, 0, 1, 2};
    /**
     * @brief 位置 -> 颜色映射
     */
    color_t ITEM_POS_COLOR[STOARGE_COUNT] = {color_red, color_red, color_blue};
}

void clear_item()
{
    for (int x=0; x<STOARGE_COUNT; x++) ITEM_POS_COLOR[x] = color_none;
    for (int x=0; x<ITEM_COUNT+1; x++) ITEM_COLOR_POS[x] = 0;
}

/**
 * @brief 储存物体到翅膀中，分配翅膀位置
 * @param plate_pos 圆盘上的位置
 * @return 应该储存到的翅膀位置
 */
uint8_t storage_item(color_t color, uint8_t plate_pos)
{
    int pos = plate_pos < 2 ? 2 : 0; // 储存到的位置：中下&左上优先到左边，其余右边
    while (ITEM_POS_COLOR[pos] != color_none) // 有东西就顺延
    {
        pos = (pos+1)%(STOARGE_COUNT); // 如果一边满了，就堆到另一边
    }
    // 维护两组映射
    ITEM_COLOR_POS[(uint8_t)color] = pos;
    ITEM_POS_COLOR[pos] = color;
    return pos;
}

/**
 * @brief 获取物品位置
 */
uint8_t get_storage_item(color_t color)
{
    return ITEM_COLOR_POS[(uint8_t)color];
}

void arm_ground_place(const Point3f *point)
{
#	ifdef __USING_FULL_FUNCTION_ARM_MOVE
    arm_move(pos, STEPMOTOR_Z_POSITIONS[2]);
#	else
    float deg;
    Point2f pos = {.x = point->x, .y = point->y};
    pos = coordinate_transform_XY_shift(pos, VISION_ATTR); // 变换到夹爪坐标系

    // R -> 前伸距离
    float R = sqrtf(powf(pos.x, 2) + powf(pos.y, 2));
    // deg -> 旋转角度
    if (pos.y > 0)
    {
        deg = 180 + (atan(fabs(pos.y / pos.x)) / 3.1415926 * 180);
    }
    else
        deg = 180 - (atan(fabs(pos.y / pos.x)) / 3.1415926 * 180);
    deg = -deg;

    // 夹爪补偿
    deg -= (asinf(CLAW_ATTR.y/R) / PI * 180.);
    R = sqrt(powf(R, 2) - powf(CLAW_ATTR.y, 2)) - CLAW_ATTR.x;
    // 移动
    motor_r->move_to(deg);
    motor_r->arrived();
    motor_x->move_to(R);
    motor_z->move_to(point->z);
#	endif
}

/**
 * @brief 安全和机械臂配合的机械爪控制
 * @param close_open false: 开, true: 闭
 * @note 只有机械臂稳定下来，机械爪才会接受控制。如果要在机械臂不稳定时控制机械爪，请
 * 使用servor_ctl.h中的函数
 */
static inline void arm_claw_ctl(bool close_open)
{
    arm_move_sync();
    claw_ctl(close_open, true);
}

/**
 * @brief 抬起Z轴
 */
void arm_Z_standby()
{
    motor_z->move_to(POSITION_Z_STANDBY);
    motor_z->arrived();
}

void arm_standby(void)
{
    wing_ctl(true, false);
    motor_z->move_to(POSITION_Z_STANDBY);
    motor_z->arrived();

    motor_x->move_to(POSITION_X_STANDBY);
    motor_r->move_to(POSITION_R_STANDBY);
    arm_move_sync();
}

void arm_item_detect(void)
{
    arm_claw_ctl(false);
    wing_ctl(false, false);

    motor_x->move_to(POSITION_X_STANDBY);
    motor_r->move_to(POSITION_R_DETECT);
    arm_move_sync();
    motor_z->move_to(POSITION_Z_DETECT);
}

void arm_cat_materials(uint8_t pos)
{
    motor_z->move_to(POSITION_Z_STANDBY);
    motor_z->arrived();

    motor_r->move_to(POSITION_R_MATERIALS[pos]);
    motor_r->arrived();

    motor_x->move_to(POSITION_X_MATERIALS[pos]);
    motor_z->move_to(STEPMOTOR_Z_POSITIONS[1]);

    arm_claw_ctl(true);
}

void arm_store_item(uint8_t index)
{
    motor_x->move_to(POSITION_X_WINGS[index]);
    motor_r->move_to(POSITION_R_WINGS[index]);
    arm_move_sync();

    // HAL_Delay(100);
    
    motor_z->move_to(STEPMOTOR_Z_POSITIONS[5]);
    motor_z->arrived();
}

void arm_ring_detect(void)
{
    motor_r->move_to(POSITION_R_DETECT);
    motor_r->arrived();
    motor_x->move_to(POSITION_X_RING_DETECT);
    motor_z->move_to(POSITION_Z_DETECT);
    arm_move_sync();
}

/*--------------------------运行控制--------------------------------------*/

void arm_action_get_materials(uint8_t pos, color_t color)
{
    // 圆盘抓取物料
    arm_claw_ctl(false);
    arm_cat_materials(pos);
    arm_claw_ctl(true); 

    // 存放
    arm_Z_standby();
    uint8_t wing_pos = storage_item(color, pos);
    arm_store_item(wing_pos); // 摆放到存储机构
    arm_claw_ctl(false); // 张爪
    
    arm_Z_standby();
#   ifdef __MATERIALS_TASK_ONLY_SCAN_ONCE
#   else
        arm_item_detect(); // 归位为检测位置
#   endif
}

void arm_action_store_to_ground(color_t color)
{
    // 采用慢速加速
    uint8_t acc_xzr[3];
    for (int x=0; x<3; x++) acc_xzr[x] = motor_xzr[x]->default_acc;
    motor_x->default_acc = ACC_X_SMOOTH;
    motor_z->default_acc = ACC_Z_SMOOTH;
    motor_r->default_acc = ACC_R_SMOOTH;

    uint8_t wing_pos = get_storage_item(color);
    arm_store_item(wing_pos); // 移到存储处对应位置
    arm_claw_ctl(true); // 闭
    // X&Z移动到就绪位置
    arm_Z_standby();
    motor_x->move_to(POSITION_X_STANDBY);
    motor_x->arrived();

    // 移动到放置位置
    Point2f tmp = vision_get_ring(color);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = STEPMOTOR_Z_POSITIONS[2]};
    arm_ground_place(&target); // 移到对应圆环位置

    arm_claw_ctl(false); // 张爪
    arm_Z_standby();

    // 恢复原始加速度
    for (int x=0; x<3; x++) motor_xzr[x]->default_acc = acc_xzr[x];
}

void arm_action_palletising(color_t color)
{
    uint8_t wing_pos = get_storage_item(color);
    arm_store_item(wing_pos); // 移到对应存储机构位置
    arm_claw_ctl(true); // 闭
    arm_Z_standby();
    motor_x->move_to(POSITION_X_STANDBY);
    motor_x->arrived();

    // 移动到码垛位置
    Point2f tmp = vision_get_ring(ARM_TARGET_COLOR_MAP[color]);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = STEPMOTOR_Z_POSITIONS[4]};
    arm_ground_place(&target);
    HAL_Delay(500); // 稍微等一下，不然容易放不稳
    arm_claw_ctl(false); // 张爪
    arm_Z_standby();
}

void arm_action_ground_to_store(color_t color)
{
    Point2f tmp = vision_get_ring(color);
    Point3f target = {.x = tmp.x, .y = tmp.y, .z = STEPMOTOR_Z_POSITIONS[2]};
    arm_ground_place(&target); // 移到对应圆环位置

    arm_claw_ctl(true); // 闭
    motor_x->move_to(POSITION_X_STANDBY);
    arm_Z_standby();

    uint8_t wing_pos = get_storage_item(color);
    arm_store_item(wing_pos); // 移到对应存储机构位置
    arm_claw_ctl(false); // 张爪

    arm_Z_standby();
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
        arm_claw_ctl(true);
        arm_Z_standby(); // 抬起机械臂
    }
    wing_ctl(true, false);
}  

//void arm_action_lift_off(bool left_right)
//{
//    arm_Z_standby();
//    motor_r->move_to(POSITION_R_DETECT);
//    motor_x->move_to(POSITION_X_STANDBY);
//    arm_move_sync();
//    
//    // 左边
//    if (left_right == true)
//    {
//        motor_r->move_to(POSITION_R_LEFT_LF);
//    } else // 右边
//    {
//        motor_r->move_to(POSITION_R_RIGHT_LF);
//    }
//    motor_x->move_to(POSITION_X_LIFT_OFF);
//    motor_z->move_to(POSITION_Z_LIFT_OFF_SRC);
//    arm_move_sync();
//    motor_x->move_to(POSITION_Z_LIFT_OFF_DST);
//    arm_move_sync();

//    arm_Z_standby();
//    motor_r->move_to(POSITION_R_DETECT);
//    motor_x->move_to(POSITION_X_STANDBY);
//    arm_move_sync();
//}

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
    default:
        break;
    }
}