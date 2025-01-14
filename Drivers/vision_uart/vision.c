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
    vision_info_instance_t *instance = &vision_info.instance;
    if (instance->updated != ps_comm_type_ring_angle_detect)
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
            instance->last_frame_gyro_yaw = vision_info.ring_angle;
        } else
        {
            // 获取上一帧的偏航角和视觉误差
            instance->last_frame_gyro_yaw = instance->curr_frame_gyro_yaw;
        }
        // 获取当前偏航角
        instance->curr_frame_gyro_yaw = HWT101Data.YawZ;
        instance->last_frame_vision_angle = vision_info.ring_angle;
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

void vision_subscribe_ring(color_t ring)
{
    static PsCommRawFrame TX_BUFFER; // 发送数据用的缓冲区
    TX_BUFFER.frame.addr = ps_comm_type_select_ring;
    TX_BUFFER.frame.select_ring.color = ring;
    __vision_subscribe(&TX_BUFFER);
}

void vision_sync(ps_comm_type_t type)
{
    vision_info.instance.updated = ps_comm_type_error;
    while (vision_info.instance.updated != type) HAL_Delay(VISION_POLL_INTERVAL);
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

            // 更新对应的信息
            switch (frame->addr)
            {
            case ps_comm_type_scan_qr:
                vision_info.instance.updated = ps_comm_type_scan_qr;
                memcpy(vision_info.order, frame->scan_qr.color,
                    sizeof(color_t)*ITEM_COUNT*ROUND_COUNT);
                break;
            case ps_comm_type_ring_pos_detect:
                vision_info.instance.updated = ps_comm_type_ring_pos_detect;
                vision_info.ring_pos.x = (float)frame->ring_center.x * VISION_SCALE;
                vision_info.ring_pos.y = (float)frame->ring_center.y * VISION_SCALE;
                break;
            case ps_comm_type_ring_angle_detect:
                vision_info.instance.updated = ps_comm_type_ring_angle_detect;
                vision_info.ring_angle = frame->ring_angle.angle;
                vision_info.ring_pos.x = (float)frame->ring_angle.x * VISION_SCALE;
                vision_info.ring_pos.y = (float)frame->ring_angle.y * VISION_SCALE;
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