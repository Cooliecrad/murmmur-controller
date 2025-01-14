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
#pragma pack()

#endif