/*
 * @Author: 21-Ӧ��-��� 1928684748@qq.com
 * @Date: 2024-04-29 21:26:27
 * @LastEditors: jianming 1928684748@qq.com
 * @LastEditTime: 2024-04-29 23:30:56
 * @FilePath: \MDK-ARMe:\������\Դ��\����\���ִ���\Chassis\Chassis.c
 * @Description:
 *
 * Copyright (c) 2024 by jianming, All Rights Reserved.
 */

#include "Chassis.h"
#include "HWT101.h"
#include "emm42_v5.h"
#include "coordinate.h"
#include <math.h>

const static float CHASSIS_PITCH_A = 0.11; // 底盘的短轴长度的一半
const static float CHASSIS_PITCH_B = 0.12; // 底盘的长轴长度的一半
const static float ROTATE_TIMES = 1.45; // 旋转的空程
const static float ROTATE_THRESHOLD = 0.5; // 可接受的角度误差

#define Division 16
chassis_t CHASSIS;

/**
 * @brief 麦轮运动解算
 * -----------------------------------------------------------------------------
 */
/**
 * @description: 逆运动分解
 * @param {ChassisTypeDef} *chassis
 * @param {float} vx
 * @param {float} vy
 * @param {float} vz
 * @note 底盘运动学模型: 象限和电机地址的映射 1-2 2-1 3-3 4-4
 * @return {*}
 */
static inline void mecanum_pose_analysis(float r, Pose2f *pose, float motor_dst[4])
{
    float angle = pose->angle/180.*PI;
    float vz_ = angle * r * ROTATE_TIMES;
    motor_dst[0] = - pose->xy.x - pose->xy.y + vz_;
    motor_dst[1] = - pose->xy.x + pose->xy.y + vz_;
    motor_dst[2] = + pose->xy.x - pose->xy.y + vz_;
    motor_dst[3] = + pose->xy.x + pose->xy.y + vz_;
}

/**
 * @brief 麦轮的逆运动分解，只包含平移量
 */
static inline void mecanum_position_analysis(Point2f point, float motor_dst[4])
{
    motor_dst[0] = - point.x - point.y;
    motor_dst[1] = - point.x + point.y;
    motor_dst[2] = + point.x - point.y;
    motor_dst[3] = + point.x + point.y;
}

/** 
 * @brief 麦轮的逆运动分解，只包含角度旋转
 */
static inline void mecanum_angle_analysis(float r, float angle, float motor_dst[4])
{
    angle = angle/180.*PI;
    float vz_ = angle * r * ROTATE_TIMES;
    motor_dst[0] = + vz_;
    motor_dst[1] = + vz_;
    motor_dst[2] = + vz_;
    motor_dst[3] = + vz_;
}

void chassis_init(UART_HandleTypeDef *pHUART)
{
    // 初始化车辆姿态
    CHASSIS.pos.x = 0;
    CHASSIS.pos.y = 0;

    // 初始化底盘
    CHASSIS.define.wheel_diameter = 0.08;
    CHASSIS.define.wheel_r = sqrtf(  powf(CHASSIS_PITCH_A, 2)
                                   + powf(CHASSIS_PITCH_B, 2));
    chassic_ctl_init(pHUART, CHASSIS.define);
}

void pose_calibrate_XY(Point2f point)
{
    CHASSIS.pos = point;
}

void chassis_move(uint8_t acc, uint16_t speed, Point2f dst)
{
    // 分解
    chassis_pos_ctl_t pos_ctl;
    pos_ctl.abs = 0;
    pos_ctl.acc = acc;
    pos_ctl.speed = speed;
    mecanum_position_analysis(dst, pos_ctl.distance);
    // 发送
    chassic_pos_ctl(&pos_ctl);
    chassis_arrived();
}

void chassis_shift(uint8_t acc, uint16_t speed, Point2f dst)
{
    dst = coordinate_transform_Z_rotate(dst, HWT101Data.YawExp);
    chassis_move(acc, speed, dst);
}

void chassis_to(uint8_t acc, uint16_t speed, Point2f dst)
{
    Point2f new_dst = coordinate_transform_Z_rotate(dst, -90);
    chassis_move_abs(acc, speed, new_dst);
}

void chassis_move_abs(uint8_t acc, uint16_t speed, Point2f dst)
{
    Pose2f pose;
    pose.xy = CHASSIS.pos;
    pose.angle = HWT101_read_yaw();
    CHASSIS.pos = dst;
    dst = coordinate_transform(dst, pose); // 逆解算出HWT101_read_yaw相对移动
    chassis_move(acc, speed, dst);
}

void chassis_move_speed(uint8_t acc, Pose2f *speed)
{
    chassis_spd_ctl_t spd_ctl;
    spd_ctl.acc = acc;
    mecanum_pose_analysis(CHASSIS.define.wheel_r, speed, spd_ctl.speed);
    // 发送
    chassis_speed_ctl(&spd_ctl);
}

void chassis_rotate(uint8_t acc, uint16_t speed, float angle)
{
    // 分解
    chassis_pos_ctl_t pos_ctl;
    pos_ctl.abs = 0;
    pos_ctl.acc = acc;
    pos_ctl.speed = speed;
    mecanum_angle_analysis(CHASSIS.define.wheel_r, angle, pos_ctl.distance);
    // 发送
    chassic_pos_ctl(&pos_ctl);
    chassis_arrived();
}

float ch_angle;
void chassis_rotate_abs(uint8_t acc, uint16_t speed, float angle)
{
    // 读取出角度
    ch_angle = angle_normal(angle - HWT101_read_yaw());
    while (fabs(ch_angle) > ROTATE_THRESHOLD)
    {
        chassis_rotate(200, 800, ch_angle);
        ch_angle = angle_normal(angle - HWT101_read_yaw());
    }
//    HWT101_calibrate(angle);
}

// void Chassis_Movedistance(ChassisTypeDef *chassis, uint8_t acc,uint16_t speed ,float x,float y)
// {
// //	chassis->coord.Mileage.Distancefinishflag=0;
    
//     kinematic_analysis(chassis,x,y,0);	
    
//     chassic_move_to(chassis, acc, speed);
    
//     chassis->Location.XDistancecurrent+=x;
//     chassis->Location.YDistancecurrent+=y;
//     HAL_Delay(100);
// }

// void chassic_change_angle(ChassisTypeDef *chassis)
// {
//     Speed_kinematic_analysis(chassis,0,0,chassis->coord.Angle.speed);

//     chassic_ctl_end();
//     for (int x=0; x<4; x++)
//     {
//         emm42_speed_ctl(x+1, chassis->Motor[x].polarity, chassis->Motor[x].TargetSpeed,
//                         chassis->coord.Angle.acc, 1);  
//     }
//     emm42_sync();
// }

// void Chassis_ChangeAngle(ChassisTypeDef *chassis,float angle)
// {
//     float TempX;
//     float TempY;
//     chassis->coord.Angle.AngleTargetLast=chassis->coord.Angle.AngleTarget;
//     chassis->coord.Angle.AngleTarget=angle;
    
//     TempX = chassis->Location.XDistancecurrent;
//     TempY = chassis->Location.YDistancecurrent;
//     chassis->Location.XDistancecurrent = CoordinatesTansformation(TempX, TempY, (chassis->coord.Angle.AngleTarget - chassis->coord.Angle.AngleTargetLast), 0);
//     chassis->Location.YDistancecurrent = CoordinatesTansformation(TempX, TempY, (chassis->coord.Angle.AngleTarget - chassis->coord.Angle.AngleTargetLast), 1);

//     while(fabs(HWT101Data.YawZ-chassis->coord.Angle.AngleTarget)>0.1)
//     {
//         PID_AngleCaclulate(&chassis->coord.Angle.AnglePID,(chassis->coord.Angle.AngleTarget - chassis->coord.Angle.AngleOffset), HWT101Data.YawZ);
//         chassis->coord.Angle.speed = chassis->coord.Angle.AnglePID.OUT;

//         chassic_change_angle(chassis);
//     }


//     emm42_halt(0, 0);
//     HAL_Delay(200);
    
//     Chassis_ChangeAngle2(chassis,angle);
    
// }

// void Chassis_ChangeAngle2(ChassisTypeDef *chassis,float angle)
// {
//     while(fabs(HWT101Data.YawZ-angle)>0.1)
//     {
//         PID_AngleCaclulate(&chassis->coord.Angle.AnglePID2,(angle - chassis->coord.Angle.AngleOffset), HWT101Data.YawZ);
//         chassis->coord.Angle.speed = chassis->coord.Angle.AnglePID2.OUT;

//         chassic_change_angle(chassis);
//     }

//     emm42_halt(0, 0);
//     HAL_Delay(200);	
        
// }

// void Screwrod_Blockage_protection_Disable(ChassisTypeDef *chassis)
// {
//     //��ַ
//     chassis->communicationpacket.M_Tx_Data[0]=MOTOR1;	
//     //�����ֽ�
//     chassis->communicationpacket.M_Tx_Data[1]=0x0E;
//     //�����ֽ�
//     chassis->communicationpacket.M_Tx_Data[2]=0x52;
//     //У���ֽ�
//     chassis->communicationpacket.M_Tx_Data[3]=0X6B;
//     chassis->communicationpacket.send_num=4;
//     chassis->communicationpacket.send_flag=1;
//     HAL_Delay(2);	
// }
