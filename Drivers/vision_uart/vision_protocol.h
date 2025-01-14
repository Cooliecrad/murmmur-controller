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
    ps_comm_type_scan_qr,            // 扫描二维码
    ps_comm_type_color_detect,   // 物体颜色检测
    ps_comm_type_ring_pos_detect,  // 色环中心位置检测
    ps_comm_type_ring_angle_detect,  // 色环角度检测
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
    uint32_t color;
} ps_comm_color_detect_t;

typedef struct
{
    uint8_t addr;
    int16_t x;
    int16_t y;
} ps_comm_ring_pos_detect_t;

typedef struct
{
    uint8_t addr;
    float angle;
    int16_t x;
    int16_t y;
} ps_comm_ring_angle_detect_t;

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
    ps_comm_color_detect_t item_color;
    ps_comm_ring_pos_detect_t ring_center;
    ps_comm_ring_angle_detect_t ring_angle;
} PsCommFrame;

typedef struct
{
    uint8_t frame_start;
    PsCommFrame frame;
    uint8_t check_sum;
} PsCommRawFrame;

#pragma pack()

#endif
