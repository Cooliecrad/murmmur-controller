#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "stm32h7xx_hal.h"
#include "common.h"
#include "chassis_ctl.h"

typedef struct
{
    chassis_def_t define;
    Point2f pos;
    volatile float *angle_src;
} chassis_t;

extern chassis_t CHASSIS;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化底盘
 * @param angle 指向原始的角度数据的指针
 */
void chassis_init(UART_HandleTypeDef *pHUART);

/**
 * @brief 校准底盘坐标：将底盘坐标设置为当前坐标
 */
void pose_calibrate_XY(Point2f point);

/**
 * @brief 平移到指定目标点，采用车身坐标系
 * @param dst 移动的三维坐标
 */
void chassis_move(uint8_t acc, uint16_t speed, Point2f dst);

/**
 * @brief 移动车身位置，采用旋转至世界坐标系相同角度的车身坐标系
 * @note 本质上车身是在世界坐标系下平动
 */
void chassis_shift(uint8_t acc, uint16_t speed, Point2f dst);

/**
 * @brief 平移到采用世界坐标系
 * @param dst 移动的二维坐标
 */
void chassis_to(uint8_t acc, uint16_t speed, Point2f dst);

inline void _chassis_to(uint8_t acc, uint16_t speed, float x, float y)
{
    Point2f point = {x, y};
    chassis_to(acc, speed, point);
}

/**
 * @brief 平移到指定目标点，采用交换了XY的世界坐标系
 * @param dst 移动的二维坐标
 */
void chassis_move_abs(uint8_t acc, uint16_t speed, Point2f dst);

/**
 * @brief 底盘移动
 * @param speed 底盘的运动方向和旋转方向
 */
void chassis_move_speed(uint8_t acc, Pose2f *speed);

/**
 * @brief 旋转指定角度
 * @param angle 旋转的角度
 */
void chassis_rotate(uint8_t acc, uint16_t speed, float angle);

/**
 * @brief 旋转到指定角度
 * @param angle 指定的方位角
 */
void chassis_rotate_abs(uint8_t acc, uint16_t speed, float angle);

/**
 * @brief 边转边平移
 * @param position 目标世界坐标系点位
 * @param angle 指定的方位角
 */
void chassis_rotate_move_tall(uint8_t acc, uint16_t speed, float angle, Point2f position);


#ifdef __cplusplus
}
#endif

#endif
