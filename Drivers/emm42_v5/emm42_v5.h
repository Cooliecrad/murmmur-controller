#ifndef __PS_EMM42_V5_H__
#define __PS_EMM42_V5_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "ps_uart_v3.h"
#include "emm42_v5_protocol.hpp"

/**
 * @brief emm42_v5 电机驱动库，提供了高效的电机到位输出支持 
 * @ref https://blog.csdn.net/zhangdatou666?type=blog
 * @authors Ptil0psis & Cooliecrad
 * @note 如何使用？
 * 1. 将emm42_v5类实例化
 * 2. 将该对象的recieve_DMA_IT放在DMA接收中断中
 * 3. 将该对象的receive_IDLE_IT放在串口中断中
 */


/**
 * @brief DEBUG选项，记录成功接收的次数在accept_sum
 */
#define __EMM42_V5_INDICATOR

/**
 * @brief 控制是否需要检查电机的收到响应（Receive）
 * @note 根据测试，收到响应实际上还是相当稳定的，所以也可以不检查，只检查到位响应
 */
#define __EMM42_V5_CHECK_RESPONSE


namespace ps
{

namespace emm42
{

/**
 * @brief 电机状态标志位
 */
enum emm42_v5_state : uint8_t
{
    DISABLE = 0x00, // 失能
    ENABLE = 0x01, // 使能
    REACHED = 0x02, // 到达
    CLOG = 0x04, // 堵转
    CLOG_PROTECT = 0x08, // 堵转保护
};

/**
 * @brief 基于上下文的舵机控制
 */
class emm42_v5 : private ps::UART
{
public:
    emm42_v5(UART_HandleTypeDef *pHUART, uint8_t *( bf[3] ));

    ~emm42_v5() {stop();}
    /**
     * @brief 设置电机状态
     * @param addr 要设置的地址
     * @param enable 电机状态
     * @param sync 同步
     */
    HAL_StatusTypeDef set_state(uint8_t addr, bool enable, bool sync = false);

    /**
     * @brief 读取电机状态
     * @param addr 要读取的地址
     * @return 电机状态，标志位
     */
    emm42_v5_state read_state(uint8_t addr);

    /**
     * @brief 设置速度控制模式
     */
    HAL_StatusTypeDef speed_ctl(uint8_t addr, uint8_t dir, uint16_t RPM,
                                uint8_t ACC,  bool sync = false);

    /**
     * @brief 设置位置控制模式
     */
    HAL_StatusTypeDef pos_ctl(uint8_t addr, uint8_t dir, uint16_t RPM, uint8_t ACC,
                              uint32_t pulse, bool relate_abs, bool sync = false);

    /**
     * @brief 急停
     */
    HAL_StatusTypeDef halt(uint8_t addr = 0, uint8_t sync = false);

    /**
     * @brief 多机同步
     */
    HAL_StatusTypeDef sync();

    /**
     * @brief 归零
     */
    HAL_StatusTypeDef reset();

    /**
     * @brief 等待最后一次运动到达（标志量：handle->reached）
     * @note 这个函数只有用户需要到位等待的时候才需要调用，是可选功能。有时候，
     * 不是必须等待底盘控制指令结束，如果平移的话，完全可以尝试叠加的效果
     */
    void arrived(uint8_t addr);

    /**
     * @paragraph 广播设置参数
     * @note 关于广播：
     * @note 1. 如果没有1号地址的电机，将无法采用广播设置
     * @note 2. 如果采用广播地址，所有的电机将会和1号地址的电机参数相同
     */

    /**
     * @brief 设置响应模式
     * @param addr 要使用广播地址，请阅读"广播设置参数""
     * @note 这个指令相当耗时间，应该和后面一个指令间隔几十毫秒的延时
     */
    void set_response(uint8_t addr, bool reached);

    /**
     * @brief 设置到位窗口大小
     * @param addr 要使用广播地址，请阅读"广播设置参数""
     * @param deg 单位 0.1°
     */
    void set_reach_wnd(uint8_t addr, uint16_t deg);

    /**
     * @brief 读取电机实时位置，单位：角度
     */
    float read_position(uint8_t addr);

    /**
     * @brief 均见父类ps::UART
     */
    void receive_DMA_IT() {UART::receive_DMA_IT();}
    void receive_IDLE_IT() {UART::receive_IDLE_IT();}
    private:

    static void receive_cb(ps::UART& self, const uint8_t *data, uint8_t length);

    /**
     * @brief 检测最后一次收发完成。每次发送请求之前都会检查上次是否完成接收
     */
    void until_received();

    /**
     * @brief 从步进电机中读取驱动参数
     * @note 和__emm42_write_drive_config()配合使用
     */
    void __read_drive_config(uint8_t addr, emm42::pkg::drive_config *rx);

    /**
     * @brief 从步进电机中写入参数
     * @note 和__emm42_read_drive_config()配合使用
     */
    void __write_drive_config(uint8_t addr, emm42::pkg::drive_config *tx);

    uint8_t tx_buffer[40];

    /**
     * @brief 最后一次运动到位的标志量
     * @note 只有调用emm42_pos_ctl才会修改这个标志量
     */
    volatile bool reached = true;

#   ifdef __EMM42_V5_CHECK_RESPONSE
        bool received = true; // 收到数据
#   endif

#   ifdef __EMM42_V5_INDICATOR
        uint32_t accept_sum = 0;
#   endif
};

}

}

#endif