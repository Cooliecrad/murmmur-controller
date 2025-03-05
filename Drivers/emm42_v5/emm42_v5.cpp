#include "emm42_v5.h"
#include <cstring>

const static int CHASSIS_POLL_INTERVAL = 0;
const static int EMM42_V5_QUERY_INTERVAL = 0;
const static int EMM42_V5_READ_POSITION_INTERVAL = 20;

using namespace ps::emm42;


void emm42_v5::receive_cb(ps::UART& self, const uint8_t *data, uint8_t length)
{
    ((emm42_v5&)self).received = true;
#   ifdef __EMM42_V5_INDICATOR
        ((emm42_v5&)self).accept_sum++;
#   endif
}

emm42_v5::emm42_v5(UART_HandleTypeDef *pHUART, uint8_t *( bf[3] ))
    : ps::UART {pHUART, bf, 64}
{
    ps::UART::receive_cb = receive_cb;
    start();
}

void emm42_v5::until_received()
{
#ifdef __EMM42_V5_CHECK_RESPONSE
    while (!received) HAL_Delay(0);
    received = false;
#endif
}

HAL_StatusTypeDef emm42_v5::set_state(uint8_t addr, bool enable, bool sync)
{
    until_received();

    const static int LEN = 6;
    static_assert(sizeof(tx_buffer) >= LEN, "tx_buffer too small");
    tx_buffer[0] = addr;
    tx_buffer[1] = 0xF3;
    tx_buffer[2] = 0xAB;
    tx_buffer[3] = enable ? 1 : 0;
    tx_buffer[4] = sync ? 1 : 0;
    tx_buffer[5] = CHECKSUM(tx_buffer, LEN);
    
    return transmit_nowait(tx_buffer, LEN);
}

emm42_v5_state emm42_v5::read_state(uint8_t addr)
{
    until_received();

    const static int LEN = 3;
    static_assert(sizeof(tx_buffer) >= LEN, "tx_buffer too small");
    tx_buffer[0] = addr;
    tx_buffer[1] = 0x3A;
    tx_buffer[2] = CHECKSUM(tx_buffer, LEN);

    // 反复发送请求，并且反复查询请求。收发没有明确的先后次序
    emm42_v5_state ret;
    bool received = false;
    while (!received)
    {
        transmit_nowait(tx_buffer, LEN);
        const uint8_t *recv = (const uint8_t*)get_nowait();
        if (recv != NULL)
        {
            ret = (emm42_v5_state)recv[2];
            received = true;
        }
        HAL_Delay(EMM42_V5_QUERY_INTERVAL);
    }
    return ret;
}

HAL_StatusTypeDef emm42_v5::speed_ctl(uint8_t addr, uint8_t dir, uint16_t RPM,
                                      uint8_t ACC,  bool sync)
{
    until_received();

    const static int LEN = 8;
    static_assert(sizeof(tx_buffer) >= LEN, "tx_buffer too small");
    tx_buffer[0] = addr;
    tx_buffer[1] = 0xF6;
    tx_buffer[2] = dir;
    tx_buffer[3] = RPM >> 8;
    tx_buffer[4] = RPM & 0xFF;
    tx_buffer[5] = ACC;
    tx_buffer[6] = sync;
    tx_buffer[7] = CHECKSUM(tx_buffer, LEN);

    return transmit_nowait(tx_buffer, LEN);
}

HAL_StatusTypeDef emm42_v5::pos_ctl(uint8_t addr, uint8_t dir, uint16_t RPM, uint8_t ACC,
                                    uint32_t pulse, bool relate_abs, bool sync)
{
    until_received();

    const static int LEN = 13;
    static_assert(sizeof(tx_buffer) >= LEN, "tx_buffer too small");
    tx_buffer[0] = addr;
    tx_buffer[1] = 0xFD;
    tx_buffer[2] = dir;
    tx_buffer[3] = RPM >> 8;
    tx_buffer[4] = RPM & 0xFF;
    tx_buffer[5] = ACC;
    tx_buffer[10] = relate_abs;
    tx_buffer[11] = sync;
    tx_buffer[12] = CHECKSUM(tx_buffer, LEN);
    // 小端转大端
    tx_buffer[9] = pulse & 0xFF;
    pulse >>= 8;
    tx_buffer[8] = pulse & 0xFF;
    pulse >>= 8;
    tx_buffer[7] = pulse & 0xFF;
    pulse >>= 8;
    tx_buffer[6] = pulse & 0xFF;

    reached = false;
    return transmit_nowait(tx_buffer, LEN);
}

HAL_StatusTypeDef emm42_v5::halt(uint8_t addr, uint8_t sync)
{
    until_received();

    const static int LEN = 5;
    static_assert(sizeof(tx_buffer) >= LEN, "tx_buffer too small");
    tx_buffer[0] = addr;
    tx_buffer[1] = 0xFE;
    tx_buffer[2] = 0x98;
    tx_buffer[3] = sync;
    tx_buffer[4] = CHECKSUM(tx_buffer, LEN);

    return transmit_nowait(tx_buffer, LEN);
}

HAL_StatusTypeDef emm42_v5::sync()
{
    until_received();

    const static int LEN = 4;
    static_assert(sizeof(tx_buffer) >= LEN, "tx_buffer too small");
    tx_buffer[0] = 0x00;
    tx_buffer[1] = 0xFF;
    tx_buffer[2] = 0x66;
    tx_buffer[3] = CHECKSUM(tx_buffer, LEN);
    
    return transmit_nowait(tx_buffer, LEN);
}

HAL_StatusTypeDef emm42_v5::reset()
{
    until_received();

    const static int LEN = 4;
    static_assert(sizeof(tx_buffer) >= LEN, "tx_buffer too small");
    tx_buffer[0] = 0x00;
    tx_buffer[1] = 0x0A;
    tx_buffer[2] = 0x6D;
    tx_buffer[3] = CHECKSUM(tx_buffer, LEN);

    return transmit_nowait(tx_buffer, LEN);
}

void emm42_v5::__read_drive_config(uint8_t addr, emm42::pkg::drive_config *rx)
{
    /**
     * @note 有意思的地方：收发的实际数据包一样大，所以可以通用
     */
    rx->dummy[0] = addr;
    rx->dummy[1] = 0x42;
    rx->dummy[2] = 0x6C;
    rx->dummy[3] = CHECKSUM(rx, 4);
    transmit((uint8_t*)rx, 4);
    auto ret = get();
    memcpy(rx, ret, sizeof(emm42::pkg::drive_config));
}

void emm42_v5::__write_drive_config(uint8_t addr, emm42::pkg::drive_config *tx)
{
    // 修改参数
    tx->tx.addr = addr;
    tx->tx.cmd[0] = 0x48;
    tx->tx.cmd[1] = 0xD1;
    tx->tx._0x01 = 0x01;
    transmit((uint8_t*)tx, sizeof(emm42::pkg::drive_config));
    HAL_Delay(10); // 驱动设置相关，需要一个长延时
}

void emm42_v5::set_response(uint8_t addr, bool reached)
{
    until_received();

    auto tx_drive = (emm42::pkg::drive_config*)tx_buffer;
    static_assert(sizeof(tx_buffer) >= sizeof(emm42::pkg::drive_config), "tx_buffer too small");
    __read_drive_config(addr, tx_drive);
    tx_drive->tx.config.Response = reached ? 3 : 1;
    __write_drive_config(addr, tx_drive);
}

void emm42_v5::set_reach_wnd(uint8_t addr, uint16_t deg)
{
    until_received();

    auto tx_drive = (emm42::pkg::drive_config*)tx_buffer;
    static_assert(sizeof(tx_buffer) >= sizeof(emm42::pkg::drive_config), "tx_buffer too small");
    __read_drive_config(addr, tx_drive);
    tx_drive->tx.config.reach_wnd = deg;
    __write_drive_config(addr, tx_drive);
}

void emm42_v5::arrived(uint8_t addr)
{
    // 轮询该地址是否完成即可
    emm42_v5_state state;
    do {
        state = read_state(addr);
    } while (   ((state & emm42_v5_state::ENABLE) == 0)
             || ((state & emm42_v5_state::REACHED) == 0));
}

float emm42_v5::read_position(uint8_t addr)
{
    until_received();

    const static int LEN = 3;
    static_assert(sizeof(tx_buffer) >= LEN, "tx_buffer too small");
    tx_buffer[0] = addr;
    tx_buffer[1] = 0x36;
    tx_buffer[2] = CHECKSUM(tx_buffer, 3);

    const emm42::pkg::read_position *ret = NULL;
    do
    {
        transmit_nowait(tx_buffer, LEN);
        HAL_Delay(poll_interval);
        int length = 0;
        ret = (const emm42::pkg::read_position*)get_nowait(&length);
    } while (ret == NULL);
    const uint8_t *data = (const uint8_t*)&ret->pulse;
    float pulse = (float)( data[3] + (data[2]<<8)+ (data[1]<<16) + (data[0]<<24) );

    return ret->symbol == 0 ? ( pulse * 360. ) / 65536. : -( pulse * 360. ) / 65536.; 
}