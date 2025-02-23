#ifndef __PS_VISION_H__
#define __PS_VISION_H__

#include "common.h"
#include "ps_uart_v2.h"
#include "vision_protocol.h"

typedef struct
{
    bool can_adjust_yaw; // 是否已经收到两帧数据
    float last_frame_vision_angle; // 上一帧的误差角
    float curr_frame_gyro_yaw; // 当前帧陀螺仪的偏航角
    float last_frame_gyro_yaw; // 上一帧陀螺仪的偏航角

    // 计数器
    ps_comm_type_t updated; // 自从上次调用vision_subscribe后，数据已更新
    color_t select_color;
#   ifdef __PS_UART_INDICATOR
    uint32_t accept_sum;
#   endif
} vision_info_instance_t;

typedef enum
{
    vision_item_pos_BOTTOM = 5,
    vision_item_pos_LEFT_UP = 6,
    vision_item_pos_RIGHT_UP = 7
} vision_item_pos_define_t;

/**
 * @brief 来自视觉的信息
 */
typedef struct 
{
    color_t order[6]; // 摆放顺序
    color_t index; // 返回的值对应的位置
    Point2f point2f; // 当前识别的圆环的坐标
	float pos_detect; //三元环角度矫正
    vision_item_pos_define_t item_detect; // 物品位置定义
    Point2f storage_points[3]; // 储存位置

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
 * @brief 停止检测物品的信息，如果物品检测完毕，请调用这个函数告知视觉停止检测
 * @note 没有必要使用同步等待这个请求，因为根本不会返回值
 */
void vision_item_detect_stop();

/**
 * @brief 向视觉订阅指定物品的信息
 * @param ring 需要的色环
 */
void vision_subscribe_item(color_t ring);

/**
 * @brief 向视觉订阅指定位置的误差
 * @param pos 位置编号
 */
void vision_subscribe_pos(uint8_t pos);

/**
 * @brief 向视觉请求当前所有色环的信息
 */
void vision_subscribe_rings();

/**
 * @brief 获取指定颜色的色环位置，按照上次调用vision_get_rings的结果返回
 */
Point2f vision_get_ring(color_t color);

/**
 * @brief 等待下位机收到指定类型的数据
 * @param ps_comm_type 请求类型
 * @note 这个函数可以用来保证至少有一帧新数据进入
 */
void vision_sync(ps_comm_type_t type);

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