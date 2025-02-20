#ifndef __PS_EMM42_V5_PROTOCOL_H__
#define __PS_EMM42_V5_PROTOCOL_H__

#include <stdint.h>

#define CHECKSUM_MODE_6B

#ifdef CHECKSUM_MODE_6B
#define CHECKSUM(src, len) 0x6B
#endif


typedef enum
{
    emm42_v5_SET_STATE = 0xF3, // 电机使能控制
    emm42_v5_SPEED_CTL = 0xF6, // 速度控制模式
    emm42_v5_POS_CTL = 0xFD, // 位置控制模式
    emm42_v5_HALT = 0xFE, // 急停
    emm42_v5_SYNC = 0xFF, // 多机同步运动
} emm42_v5_type_t;

#pragma pack(1)
typedef struct
{
    uint8_t addr;
    uint16_t hex_F3AB;
    uint8_t state;
    uint8_t sync;
    uint8_t checksum;
} emm42_v5_set_state_t;

typedef struct
{
    uint8_t addr;
    uint8_t hex_F6;
    uint8_t dir;
    uint16_t rpm;
    uint8_t acc;
    uint8_t sync;
    uint8_t checksum;
} emm42_v5_speed_ctl_t;

typedef struct
{
    uint8_t addr;
    uint8_t hex_FD;
    uint8_t dir;
    uint16_t rpm;
    uint8_t acc;
    uint32_t pulse;
    uint8_t abs_pos;
    uint8_t sync;
    uint8_t checksum;
} emm42_v5_pos_ctl_t;

typedef struct
{
    uint8_t addr;
    uint16_t hex_FE98;
    uint8_t sync;
    uint8_t checksum;
} emm42_v5_halt_t;

typedef struct
{
    uint8_t addr;
    uint16_t hex_FF66;
    uint8_t sync;
    uint8_t checksum;
} emm42_v5_sync_t;

typedef struct
{
    uint8_t MotType;
    uint8_t P_Pul;
    uint8_t P_Serial;
    uint8_t En;
    uint8_t Dir;
    uint8_t MStep;
    uint8_t MPlyer;
    uint8_t AutoSDD;
    uint16_t Ma;
    uint16_t Ma_Limit;
    uint16_t Op_Limit;
    uint8_t UartBaud;
    uint8_t CAN_Baud;
    uint8_t ID_Addr;
    uint8_t Checksum;
    uint8_t Response;
    uint8_t Clog_Pro;
    uint16_t Clog_Rpm;
    uint16_t Clog_Ma;
    uint16_t Clog_Ms;
    uint16_t reach_wnd; // 到位窗口角度，单位(0.1°)
} __emm42_v5_drive_cfg_t;

typedef struct
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t recv_len; // 返回命令包含字节数
    uint8_t recv_param_sum; // 返回命令包含参数数
    __emm42_v5_drive_cfg_t config;
    uint8_t checksum;
} __emm42_v5_drive_rx_cfg_t;

typedef struct
{
    uint8_t addr;
    uint8_t cmd[2];
    uint8_t _0x01;
    __emm42_v5_drive_cfg_t config;
    uint8_t checksum;
} __emm42_v5_drive_tx_cfg_t;

typedef union
{
    uint8_t dummy[33];
    __emm42_v5_drive_rx_cfg_t rx;
    __emm42_v5_drive_tx_cfg_t tx;
} emm42_v5_drive_t;

#pragma pack()

#endif