#ifndef __PS_STEPMOTOR_H__
#define __PS_STEPMOTOR_H__

/**
 * @brief Ptil0psis' S STepMoTor controllerr 步进电机控制。提供了相当程度的和驱动解耦
 */

#include "emm42_v5.h"
#include <functional>

namespace ps
{

namespace motor
{

/**
 * @brief 步进电机控制类
 */
class Stepmotor
{
public:
    Stepmotor(emm42::emm42_v5& handle, uint8_t ID, double ratio,
              uint32_t PPR = 3200, uint8_t reach_wnd = 1);

    /**
     * @brief 设置保护范围，电机不会超过保护范围
     */
    bool set_protect(float lower, float upper);

    /**
     * @brief 位置控制模式移动，绝对模式
     * @param RPM 转速
     * @param acc 电机转速加速度。和RPM有关系，具体见电机文档
     */
    void move_to(float position, uint16_t RPM, uint8_t acc, bool sync = false);

    /**
     * @brief 位置控制模式移动，绝对模式
     */
    void move_to(float position, bool sync = false);

    /**
     * @brief 相对模式移动
     * @param RPM 转速
     * @param acc 电机转速加速度。和RPM有关系，具体见电机文档
     * @note 这个模式不被保护范围影响
     */
    void move(float position, uint16_t RPM, uint8_t acc, bool sync = false);

    /**
     * @brief 相对模式移动
     * @note 这个模式不被保护范围影响
     */
    void move(float position, bool sync = false);

    /**
     * @brief 速度模式运动。如果speed=0，停止运动
     * @param speed 单位距离每秒
     * @param acc 电机转速加速度，和acc没有明确关联，具体见电机文档
     * @note 这个模式不被保护范围影响
     */
    void move_speed(float speed, uint8_t acc, bool sync = false);

    /**
     * @brief 设置当前状态
     */
    void set_state(bool enable, bool sync = false) {handle.set_state(ID, enable, sync);}

    /**
     * @brief 读取电机，更新当前位置并返回
     */
    float update();
    
    /**
     * @brief 以当前位置设置为零点
     */
    void zero_set();

    /**
     * @brief 设置原点的操作序列
     * @note 要使用该功能前，必须按照下面的操作序列，检查否存在无法检测/卡死的情况。
     * @note 操作序列：1. 查询端口状态is_clicked()。若为true，跳3，否则跳2。
     *               2. 发送speed相反方向运动指令，运动到is_clicked()为false的
     *                   deadzone毫秒后
     *               2. 发送运动指令，速度为speed。
     * @param is_clicked 零点的检测函数
     * @param speed 调整速度，具体含义见上方操作序列的描述
     * @param deadzone 等待时长，具体含义见上方操作序列的描述
     */
    void calibrate(const std::function<bool(void)>& is_clicked,
                   float speed, uint8_t acc, uint32_t deadzone = 500);
    

    /**
     * @brief 检测到按键时触发
     * @note 对于需要使用动态零点的电机，需要配置相应的检测硬件，例如：微动开关、光电开关。
     * 同时，在对应硬件开关触发时调用的函数中，添加本函数
     */
    void on_click();

    /**
     * @brief 等待电机到位
     */
    void arrived();

    double ratio; // 减速比：脉冲数/一基础距离单位
    const uint32_t PPR; // Pulse per round, 每圈脉冲数
    bool reverse = false; // 反转电机方向

    // 位置控制模式的缺省速度
    uint8_t default_acc;
    uint16_t default_speed;

private:
    emm42::emm42_v5& handle;
    uint8_t ID = 0; // 硬件地址

    float position = 0; // 电机位置，这个值应该与电机角度一致
    bool is_moving = false; // 速度模式下，电机是否在运动状态
    bool clicked = false; // 校准用：是否从clicked状态触发中断
    bool calibrating = false; // 指示正在校准

    // 保护用，如果两个值都是0，说明不保护
    float lower = 0;
    float upper = 0;
};

}

}

#endif