#include "vision.h"
#include "vision_protocol.h"
#include "ps_uart_v3.h"

#include <string.h>
#include <queue>

const static float VISION_SCALE = 0.001; // 视觉和底盘单位的缩放量
const static int VISION_POLL_INTERVAL = 10;
static uint8_t TX_BUFFER[__PS_COMM_FRAME_LENGTH]; // 发送数据用的缓冲区
static uint8_t VISION_BF0[__PS_COMM_FRAME_LENGTH];
static uint8_t VISION_BF1[__PS_COMM_FRAME_LENGTH];
static uint8_t VISION_BF2[__PS_COMM_FRAME_LENGTH];
static uint8_t *VISION_BUFFER[] = {
    (uint8_t*)&VISION_BF0, (uint8_t*)&VISION_BF1, (uint8_t*)&VISION_BF2
};

typedef struct
{
    bool can_adjust_yaw; // 是否已经收到两帧数据
    float last_frame_vision_angle; // 上一帧的误差角
    float curr_frame_gyro_yaw; // 当前帧陀螺仪的偏航角
    float last_frame_gyro_yaw; // 上一帧陀螺仪的偏航角

    // 计数器
    bool updated; // 自从上次调用vision_subscribe后，数据已更新
    ps_comm_type_t last_subscribed; // 最后一次请求的类型
    color_t select_color; // 最后一次请求的颜色
} vision_info_instance_t;

enum vision_item_pos_define_t : uint8_t
{
    vision_item_pos_BOTTOM = 0,
    vision_item_pos_LEFT_UP = 1,
    vision_item_pos_RIGHT_UP = 2
};

/**
 * @brief 来自视觉的信息
 */
typedef struct 
{
    color_t order[6]; // 摆放顺序
    Point2f point2f; // 当前识别的圆环的坐标
	float pos_detect; //三元环角度矫正
    vision_item_pos_define_t item_detect; // 物品位置定义
    Point2f storage_points[3]; // 储存位置

    vision_info_instance_t instance;
} vision_info_t;

vision_info_t vision_info;
ps::UART *vision_uart_handle = NULL;
static std::queue<RC_data_t> call_queue; // 指令序列

static bool validate(ps::UART& self, const uint8_t *data, uint8_t length)
{
    // 如果长度小于3，则 { 包头 地址 [数据] 校验和 } 的格式不匹配
    if (length < 3) return false;
    // 检验包头
    if (data[0] != __PS_COMM_FRAME_START) return false;
    // 检查校验和，失败不做任何修改
    uint8_t check_sum = ps::sigma_check_sum(data, length-1);
    if (check_sum != data[length-1]) return false;// 包尾最后一个字节是校验和
    // 是否和等待的包匹配
    auto type = (ps_comm_type_t)data[1];
    if (type != (vision_info.instance.last_subscribed & 0x7F)) return false;
    // 如果是某些类型，还需要是否和请求的颜色一致。颜色都在data[2]中储存
    switch (type)
    {
    case ps_comm_type_ITEM_DETECT:
        if (data[2] != vision_info.instance.select_color) return false;
        break;
    default:
        break;
    }
    return true;
}

static void receive_cb(ps::UART& self, const uint8_t *data, uint8_t length)
{
    auto type = (ps_comm_type_t)data[1];
    switch (type)
    {
    case ps_comm_type_SCAN_QR:
        memcpy(vision_info.order, ((ps_comm_scan_qr_t*)data)->color,
               sizeof(color_t)*ITEM_COUNT*ROUND_COUNT);
        break;
    case ps_comm_type_ITEM_DETECT:
        vision_info.item_detect = (vision_item_pos_define_t)((ps_comm_item_detect_t*)data)->pos;
        break;
    case ps_comm_type_RING_POS:
        for (int x=0; x<ITEM_COUNT; x++)
        {
            vision_info.storage_points[x].x = (float)((ps_comm_ring_pos_t*)data)->points[x][0];
            vision_info.storage_points[x].y = (float)((ps_comm_ring_pos_t*)data)->points[x][1];
        }
        break;
    case ps_comm_type_POS_DETECT:
        vision_info.point2f.x = (float)(((ps_comm_pos_detect_t*)data)->x * VISION_SCALE);
        vision_info.point2f.y = (float)(((ps_comm_pos_detect_t*)data)->y * VISION_SCALE);
        break;
    case ps_comm_type_RC:
        // 拷贝内存 & 添加到队列
        {
            length -= 3; // 去掉包头、地址、校验和
            uint8_t *call = (uint8_t*)malloc(length); 
            if (call != NULL) // 如果是空指针，就不要接着了
            {
                memcpy(call, data+2, length);
                call_queue.push({call, length});
            }
        }
        break;
    default:
        break;
    }
    vision_info.instance.updated = true; // 收到了新的数据包
}

void vision_init(UART_HandleTypeDef *pHUART)
{
    // 初始化串口
    vision_uart_handle = new ps::UART {pHUART, VISION_BUFFER,
                                       __PS_COMM_FRAME_LENGTH};
    vision_uart_handle->receive_cb = receive_cb;
    vision_uart_handle->validate = validate;
    vision_uart_handle->start();

    // 初始化视觉数据
    vision_info.instance.last_subscribed = ps_comm_type_error;
    vision_info.order[0] = color_red;
    vision_info.order[1] = color_green;
    vision_info.order[2] = color_blue;
    vision_info.order[3] = color_blue;
    vision_info.order[4] = color_green;
    vision_info.order[5] = color_red;
}

/**
 * @brief 发送订阅消息的请求
 * @param frame 消息帧，不需要填写帧头以及校验和
 */
static inline void __vision_subscribe(uint8_t *frame, int length)
{
    // 记录最后一次订阅的类型
    vision_info.instance.last_subscribed = (ps_comm_type_t)frame[1];
    // 如果是特定类型，还需要记录最后一次发送的颜色
    auto type = (ps_comm_type_t)(frame[1]);
    switch (type)
    {
    case ps_comm_type_ITEM_DETECT_REQ:
        vision_info.instance.select_color = (color_t)frame[2];
    default:
        break;
    }
    // 填入包头&检验和：校验和是不带包头的
    frame[0] = __PS_COMM_FRAME_START;
    frame[length-1] = ps::sigma_check_sum(frame+1, length-2);
    // 发送新的数据包 & 已接收的数据作废
    vision_info.instance.updated = false;
    vision_uart_handle->transmit(frame, length);
}

void vision_idle()
{
    auto bf = (ps_comm_idle_req_t*)TX_BUFFER;
    bf->addr = ps_comm_type_IDLE_REQ;
    __vision_subscribe(TX_BUFFER, sizeof(ps_comm_idle_req_t));
}

void vision_subscribe_order(void)
{
    TX_BUFFER[1] = (uint8_t)ps_comm_type_SCAN_QR_REQ;
    __vision_subscribe(TX_BUFFER, sizeof(ps_comm_scan_qr_req_t));
}

color_t vision_order(uint8_t round, uint8_t index)
{
    return vision_info.order[round * ITEM_COUNT + index];
}

void vision_subscribe_item(color_t color)
{
    auto bf = (ps_comm_item_detect_req_t*)TX_BUFFER;
    bf->addr = ps_comm_type_ITEM_DETECT_REQ;
    bf->color = color;
    __vision_subscribe(TX_BUFFER, sizeof(ps_comm_item_detect_req_t));
}

uint8_t vision_get_item(color_t color)
{
    vision_subscribe_item(color);
    vision_sync();
    vision_idle();
    return (uint8_t)vision_info.item_detect;
}

void vision_item_detect_stop()
{
    vision_subscribe_item(color_none);
}

Point2f vision_get_pos(uint8_t pos)
{
    // 发送请求
    auto bf = (ps_comm_pos_detect_req_t*)TX_BUFFER;
    bf->addr = ps_comm_type_POS_DETECT_REQ;
    bf->pos = pos;
    __vision_subscribe(TX_BUFFER, sizeof(ps_comm_pos_detect_req_t));
    vision_sync();
    return vision_info.point2f;
}

void vision_subscribe_rings()
{
    auto bf = (ps_comm_ring_pos_req_t*)TX_BUFFER;
    bf->addr = ps_comm_type_RING_POS_REQ;
    __vision_subscribe(TX_BUFFER, sizeof(ps_comm_ring_pos_req_t));
    vision_sync();
    vision_idle();
}

Point2f vision_get_ring(color_t color)
{
    return vision_info.storage_points[(int)color-1];
}

void vision_remote_control_mode()
{
    vision_info.instance.last_subscribed = ps_comm_type_RC;
}

RC_data_t vision_get_RC()
{
    if (!call_queue.empty())
    {
        RC_data_t ret {call_queue.front()};
        call_queue.pop();
        return ret;
    } else
        return RC_data_t {NULL, 0};
}

void vision_sync()
{
    while (!vision_info.instance.updated) HAL_Delay(VISION_POLL_INTERVAL);
    vision_info.instance.updated = false; // 当调用过这个函数后，认为没有新数据了
}