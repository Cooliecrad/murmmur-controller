#include "vision.h"
#include "HWT101.h"
#include <string.h>

const static float VISION_SCALE = 0.001; // 视觉和底盘单位的缩放量
const static int VISION_POLL_INTERVAL = 10;
PsCommRawFrame VISION_COPY_BUFFER0;
PsCommRawFrame VISION_COPY_BUFFER1;
PsCommRawFrame VISION_COPY_BUFFER2;

vision_info_t vision_info;
ps_uart_t vision_uart;
ps_uart_handle_t vision_uart_handle;

/**
 * @brief 在接收中断调用，保存此时陀螺仪的YAW角
 */
void vision_angle_calibrate(void)
{
    volatile vision_info_instance_t *instance = &vision_info.instance;
    if (instance->updated != ps_comm_type_POS_DETECT)
    {
        instance->can_adjust_yaw = false;
    } else
    {
        /**
         * @note 如果上一帧不是视觉，误差值直接按照这一帧算。这是为了
         * 保证last_frame_gyro_yaw不会出现0，否则可能不调直接认为到位！ 
         */
        if (!instance->can_adjust_yaw)
        {
            instance->can_adjust_yaw = true;
            instance->last_frame_gyro_yaw = HWT101Data.YawZ;
        } else
        {
            // 获取上一帧的偏航角和视觉误差
            instance->last_frame_gyro_yaw = instance->curr_frame_gyro_yaw;
        }
        // 获取当前偏航角
        instance->curr_frame_gyro_yaw = HWT101Data.YawZ;
        instance->last_frame_vision_angle = vision_info.pos_detect;
    }
}

void vision_init(UART_HandleTypeDef *pHUART)
{
    // 初始化串口
    vision_uart.buffer[0] = &VISION_COPY_BUFFER0;
    vision_uart.buffer[1] = &VISION_COPY_BUFFER1;
    vision_uart.buffer[2] = &VISION_COPY_BUFFER2;
    vision_uart.recv_len = sizeof(PsCommRawFrame);
    vision_uart.pHUART = pHUART;
    vision_uart.recv_callback = vision_angle_calibrate;
    vision_uart_handle = &vision_uart;

    // 初始化视觉数据
    vision_info.instance.updated = ps_comm_type_error;
    vision_info.order[0] = 1;
    vision_info.order[1] = 2;
    vision_info.order[2] = 3;
    vision_info.order[3] = 3;
    vision_info.order[4] = 2;
    vision_info.order[5] = 1;
#   ifdef __PS_UART_INDICATOR
    vision_info.accept_sum = 0;
#   endif

    ps_uart_init(&vision_uart);
}

/**
 * @brief 发送订阅消息的请求
 * @param frame 消息帧，不需要填写帧头以及校验和
 */
static inline void __vision_subscribe(PsCommRawFrame *frame)
{
    frame->frame_start = __PS_COMM_FRAME_START;
    frame->check_sum = ps_uart_sigma_check_sum((uint8_t*)&frame->frame, sizeof(PsCommFrame));
    vision_info.instance.updated = ps_comm_type_error;
    ps_uart_transmit(vision_uart_handle, frame, sizeof(PsCommRawFrame));
}

void vision_subscribe(ps_comm_type_t type)
{
    static PsCommRawFrame TX_BUFFER; // 发送数据用的缓冲区
    TX_BUFFER.frame.addr = type;
    __vision_subscribe(&TX_BUFFER);
}

void vision_subscribe_pos(uint8_t pos)
{
    static PsCommRawFrame TX_BUFFER; // 发送数据用的缓冲区
    TX_BUFFER.frame.addr = ps_comm_type_POS_DETECT_REQ;
    TX_BUFFER.frame.pos_detect_req.pos = pos;
    __vision_subscribe(&TX_BUFFER);
}

void vision_subscribe_item(color_t item)
{
    static PsCommRawFrame TX_BUFFER; // 发送数据用的缓冲区
    TX_BUFFER.frame.addr = ps_comm_type_ITEM_DETECT_REQ;
    TX_BUFFER.frame.ring_pos_req.color = item;
    __vision_subscribe(&TX_BUFFER);
}

void vision_subscribe_rings()
{
    static PsCommRawFrame TX_BUFFER; // 发送数据用的缓冲区
    TX_BUFFER.frame.addr = ps_comm_type_RING_POS_REQ;
    for (int x=1; x<=3; x++)
    {
        color_t color = (color_t)x;
        TX_BUFFER.frame.ring_pos_req.color = color;
        vision_info.instance.select_color = color;
        __vision_subscribe(&TX_BUFFER);
        vision_sync(ps_comm_type_RING_POS);
        vision_info.storage_points[x-1] = vision_info.point2f;
    }
    vision_subscribe(ps_comm_type_IDLE_REQ); // 让视觉不要再扫描了
}

Point2f vision_get_ring(color_t color)
{
    return vision_info.storage_points[(int)color-1];
}

void vision_sync(ps_comm_type_t type)
{
    vision_info.instance.updated = ps_comm_type_error;
    switch (type)
    {
    case ps_comm_type_ITEM_DETECT:
    case ps_comm_type_RING_POS:
        while (   vision_info.instance.updated != type
               || vision_info.index != vision_info.instance.select_color)
            HAL_Delay(VISION_POLL_INTERVAL);
        break;
    
    default:
        while (vision_info.instance.updated != type) HAL_Delay(VISION_POLL_INTERVAL);
        break;
    }
}

bool vision_update_nowait()
{
    PsCommRawFrame *raw_frame = (PsCommRawFrame*)ps_uart_acquire(vision_uart_handle);
    bool status = false;
    if (raw_frame)
    {
        PsCommFrame *frame = &raw_frame->frame;
        // 检查校验和，失败不做任何修改
        uint8_t check_sum = ps_uart_sigma_check_sum((uint8_t*)frame,
                                                    sizeof(PsCommFrame));
        if (check_sum == raw_frame->check_sum)
        {
            status = true;
#           ifdef __PS_UART_INDICATOR
                vision_info.accept_sum++;
#           endif

            vision_info.instance.updated = frame->addr;
            // 更新对应的信息
            switch (frame->addr)
            {
            case ps_comm_type_SCAN_QR_REQ:
                memcpy(vision_info.order, frame->scan_qr.color,
                    sizeof(color_t)*ITEM_COUNT*ROUND_COUNT);
                break;
            case ps_comm_type_ITEM_DETECT:
                vision_info.item_detect = (vision_item_pos_define_t)frame->item_detect.pos;
                break;
            case ps_comm_type_RING_POS:
                vision_info.index = frame->ring_pos.color;
                vision_info.point2f.x = frame->ring_pos.x;
                vision_info.point2f.y = frame->ring_pos.y;
                break;
            case ps_comm_type_POS_DETECT:
                vision_info.pos_detect = frame->pos_detect.angle;
                vision_info.point2f.x = (float)frame->pos_detect.x * VISION_SCALE;
                vision_info.point2f.y = (float)frame->pos_detect.y * VISION_SCALE;
                break;
            }
        }
    }

    ps_uart_release(vision_uart_handle);
    return status;
}

void vision_update()
{
    bool ret;
    do {
        ret = vision_update_nowait();
        HAL_Delay(VISION_POLL_INTERVAL);
    } while (!ret);
}