#ifndef __PS_COMM_C_H__
#define __PS_COMM_C_H__

#define __PS_COMM_FRAME_START 0xA5
#define __PS_COMM_FRAME_LENGTH 11

#include <stdint.h>

typedef enum
{
    ps_comm_type_heartbeat,         // 通信心跳
    ps_comm_type_motion_ctl,         // 速度和方向控制
    ps_comm_type_motion_ctl_mode,     // 速控模式
    ps_comm_type_servo_threshold,    // 舵机阈值
    ps_comm_type_sound_effect,       // 音效
    ps_comm_type_light_effect,       // 灯效
    ps_comm_type_button_info,        // 按键
    ps_comm_type_battery_info,       // 电池信息

    ps_comm_type_SCAN_QR,  // 扫码
    ps_comm_type_ITEM_DETECT, // 位置中心
    ps_comm_type_POS_DETECT, // 位姿误差
    ps_comm_type_RING_POS, // 圆环位置

    ps_comm_type_IDLE_REQ = 0x81, // 请求视觉进入空闲状态
    
    ps_comm_type_SCAN_QR_REQ = 0x88, // 扫描二维码
    ps_comm_type_ITEM_DETECT_REQ = 0x89, // 位置中心检测
    ps_comm_type_POS_DETECT_REQ = 0x8A,   // 位置误差检测
    ps_comm_type_RING_POS_REQ = 0x8B, /// 圆环位置

    ps_comm_type_error = 0xff // 无效类型
} ps_comm_type_t;

typedef enum
{
    ps_comm_motion_ctl_mode_CLOSE_LOOP,
    ps_comm_motion_ctl_mode_OPEN_LOOP
} __ps_comm_motion_ctl_mode_t;

typedef enum
{
    ps_comm_sound_effect_OK,
    ps_comm_sound_effect_Warning,
    ps_comm_sound_effect_Finish,
    ps_comm_sound_effect_Ding,
    ps_comm_sound_effect_SystemStart,
} __ps_comm_sound_effect_t;

// 必须修改字节对齐
#pragma pack(1)
typedef struct
{
    uint8_t addr;
	uint8_t dummy[8];
} ps_comm_heartbeat_t;


typedef struct
{
    uint8_t addr;
    float speed;
    uint16_t direction;
} ps_comm_motion_ctl_t;

typedef struct
{
    uint8_t addr;
    uint8_t ctl_mode;
} ps_comm_motion_ctl_mode_t;

typedef struct
{
    uint8_t addr;
    uint8_t channel;
    uint16_t threshold;
} ps_comm_servo_threshold_t;

typedef struct
{
    uint8_t addr;
    uint8_t type;
} ps_comm_sound_effect_t;

typedef struct
{
    uint8_t addr;
    uint32_t color;
} ps_comm_light_effect_t;

typedef struct
{
    uint8_t addr;
    uint16_t pressed;
} ps_comm_button_info_t;

typedef struct
{
    uint8_t addr;
    uint8_t present;
    float voltage;
} ps_comm_battery_info_t;

typedef struct
{
    uint8_t addr;
    uint8_t color[6];
} ps_comm_scan_qr_t;

typedef struct
{
    uint8_t addr;
    uint8_t color;
} ps_comm_item_detect_req_t;

typedef struct
{
    uint8_t addr;
    uint8_t color;
    uint8_t pos;
} ps_comm_item_detect_t;

typedef struct
{
    uint8_t addr;
    uint8_t pos;
} ps_comm_pos_detect_req_t;

typedef struct
{
    uint8_t addr;
    float angle;
    int16_t x;
    int16_t y;
} ps_comm_pos_detect_t;

typedef struct
{
    uint8_t addr;
    uint8_t color;
} ps_comm_ring_pos_req_t;

typedef struct
{
    uint8_t addr;
    uint8_t color;
    int16_t x;
    int16_t y;
} ps_comm_ring_pos_t;


typedef union
{
    uint8_t addr;
    ps_comm_heartbeat_t heartbeat;
    ps_comm_motion_ctl_t motion_ctl;
    ps_comm_motion_ctl_mode_t motion_mode;
    ps_comm_servo_threshold_t servo_thr;
    ps_comm_sound_effect_t sound_eff;
    ps_comm_light_effect_t light_eff;
    ps_comm_button_info_t button;
    ps_comm_battery_info_t battery;
    ps_comm_scan_qr_t scan_qr;
    ps_comm_item_detect_t item_detect;
    ps_comm_item_detect_req_t item_detect_req;
    ps_comm_pos_detect_req_t pos_detect_req;
    ps_comm_pos_detect_t pos_detect;
    ps_comm_ring_pos_req_t ring_pos_req;
    ps_comm_ring_pos_t ring_pos;
} PsCommFrame;

typedef struct
{
    uint8_t frame_start;
    PsCommFrame frame;
    uint8_t check_sum;
} PsCommRawFrame;

#pragma pack()

#endif
