#ifndef __PS_EMM42_V5_PROTOCOL_H__
#define __PS_EMM42_V5_PROTOCOL_H__

#include <cstdint>

#define CHECKSUM_MODE_6B

#ifdef CHECKSUM_MODE_6B
#define CHECKSUM(src, len) 0x6B
#endif

namespace ps
{

namespace emm42
{
    
enum package_type
{
    SET_STATE = 0xF3, // 电机使能控制
    SPEED_CTL = 0xF6, // 速度控制模式
    POS_CTL = 0xFD, // 位置控制模式
    HALT = 0xFE, // 急停
    SYNC = 0xFF, // 多机同步运动
};

namespace pkg
{

#pragma pack(1)
struct set_state
{
    uint8_t addr;
    uint16_t hex_F3AB;
    uint8_t state;
    uint8_t sync;
    uint8_t checksum;
};

struct speed_ctl
{
    uint8_t addr;
    uint8_t hex_F6;
    uint8_t dir;
    uint16_t rpm;
    uint8_t acc;
    uint8_t sync;
    uint8_t checksum;
};

struct pos_ctl
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
};

struct halt
{
    uint8_t addr;
    uint16_t hex_FE98;
    uint8_t sync;
    uint8_t checksum;
};

struct sync
{
    uint8_t addr;
    uint16_t hex_FF66;
    uint8_t sync;
    uint8_t checksum;
};

struct drive_cfg
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
};

struct drive_rx_cfg
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t recv_len; // 返回命令包含字节数
    uint8_t recv_param_sum; // 返回命令包含参数数
    drive_cfg config;
    uint8_t checksum;
};

struct drive_tx_cfg
{
    uint8_t addr;
    uint8_t cmd[2];
    uint8_t _0x01;
    drive_cfg config;
    uint8_t checksum;
};

union drive_config
{
    uint8_t dummy[33];
    drive_rx_cfg rx;
    drive_tx_cfg tx;
};

/**
 * @brief 读取位置数据返回的数据包
 */
struct read_position
{
    uint8_t addr;
    uint8_t _0x36;
    uint8_t symbol;
    uint32_t pulse; // 注意大小端！
    uint8_t checksum;
};

#pragma pack()
}

}

}

#endif