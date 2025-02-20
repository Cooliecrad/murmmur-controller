#ifndef __ARMTASK_H_
#define __ARMTASK_H_

#include "main.h" // Device header
#include "StepmotorGPIO.h"
#include "usart.h"
#include "common.h"
#include "emm42_v5.h"

extern float StepMotor_R_StorePosition[];
extern float StepMotor_Forward_Store[];
extern float StepMotor_Z_Position[];
extern emm42_handle_t arm_emm42_handle;

/**
 * @brief 采用全功能机械臂解算函数来控制机械臂位置
 */
// #define __USING_FULL_FUNCTION_ARM_MOVE

typedef enum
{
	Ordinary2 = 0,
    Red2 = 1,
    Green2 = 2,
    Blue2 = 3,
    Dtct2 = 4,
    Limit = 5
} ArmTarget;

typedef enum
{
    Ordinary = 0,
    Open = 1,
    Close = 2
} ArmState;

typedef enum
{
    Ordinary3 = 0,
    Scan = 1,			 // 扫码
    Dtct = 2,			 // 在圆盘上识别
    Plate_To_Store = 3,	 // 从圆盘拾取放到存储机构
    Store_To_Ground = 4, // 从存储机构摆放到地面
    Ground_To_Store = 5, // 从地面拾取放到存储机构
    Maduo = 6,			 // 码垛
    Store_Open = 7,		 // 存储机构开
    Store_Close = 8,     // 存储机构关
    Store_Scan,
} ArmAction;

typedef struct
{
    bool inner; // 标志着当前在死区内
    Point2f upper;
    Point2f lower; // 死区范围
    float inner_max; // 死区内最大移动量
    float outer_max; // 死区外最大移动量
} arm_deadzone_t;

typedef struct
{
    Point2f center; // 旋转中心（Z轴）在视觉坐标系中的位置
    float claw_distance; // 爪子零点到旋转中心的距离
    arm_deadzone_t deadzone;
    stepmotor_t motor_r; // 旋转电机
    stepmotor_t motor_x; // 前伸电机
    stepmotor_t motor_z; // 上升电机
} arm_define_t;

extern arm_define_t *pARM_DEFINE;

typedef struct
{
	uint16_t angle;
	uint16_t forward;
}Color_Position;

typedef struct
{
	Color_Position red;
	Color_Position green;
	Color_Position blue;
}vision_position;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 初始化机械臂控制
 */
void arm_ctl_init(UART_HandleTypeDef *pHUART);

/**
 * @brief 电机绝对移动
 */
void stepmotor_move(stepmotor_t *motor, float position);

/**
 * @brief 电机绝对旋转
 */
void stepmotor_rotate(stepmotor_t *motor, float position);

/**
 * @brief 控制机械臂到达指定位置
 */
void arm_move(Point2f XY, float Z);

/**
 * @brief 机械臂抓取指定点位
 */
Point2f arm_ground_place(Point2f pos, float z);

/**
 * @brief 等待机械臂的所有电机运动完毕
 */
void arm_move_sync(void);

void stepmotor_halt(stepmotor_t *motor);	
void StepMotor_GetState(stepmotor_t *motor);
void Arm_Scan(void);
void Arm_X_Zip(void);
uint16_t angle_to_position(float angle);
void Arm_Action(ArmAction action, ArmTarget color);
void Arm_Action_Store_To_Ground2(ArmTarget color);
	
	
#ifdef __cplusplus
}
#endif

#endif
