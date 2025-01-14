#ifndef __PS_VISION_H__
#define __PS_VISION_H__

#include "common.h"
#include "ps_uart_v2.h"
#include "vision_protocol.h"

typedef struct
{
    bool can_adjust_yaw; // 是否已经收到两帧数据
    float last_frame_vision_angle; // 上一帧的角度
    float curr_frame_gyro_yaw; // 当前帧陀螺仪的偏航角
    float last_frame_gyro_yaw; // 上一帧陀螺仪的偏航角

    // 计数器
    ps_comm_type_t updated; // 自从上次调用vision_subscribe后，数据已更新
#   ifdef __PS_UART_INDICATOR
    uint32_t accept_sum;
#   endif
} vision_info_instance_t;

/**
 * @brief 来自视觉的信息
 */
typedef struct 
{
    color_t order[6]; // 摆放顺序
    color_t item_color; // 当前识别出物品的颜色
    Point2f ring_pos; // 当前识别的圆环的坐标
	float ring_angle; //三元环角度矫正

    vision_info_instance_t instance;
} vision_info_t;


extern vision_info_t vision_info;
extern ps_uart_handle_t vision_uart_handle;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化视觉通信
 */
void vision_init(UART_HandleTypeDef *pHUART);
 
/**
 * @brief 向视觉订阅指定主题的信息
 * @param ps_comm_type 请求类型
 */
void vision_subscribe(ps_comm_type_t type);

/**
 * @brief 向视觉订阅指定主题的信息，并等待该主题信息返回
 * @param ps_comm_type 请求类型
 * @note 这个函数可以用来保证至少有一帧新数据进入
 */
void vision_open(ps_comm_type_t type);

/**
 * @brief 查询接收到的信息并做出相应的更新，非阻塞
 * @return 是否产生了更新
 */
bool vision_update_nowait();

/**
 * @brief 查询接收到的信息并做出相应的更新，阻塞
 */
void vision_update();

#ifdef __cplusplus
}
#endif

#endif