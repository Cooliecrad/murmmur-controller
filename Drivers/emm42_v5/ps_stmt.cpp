#include "ps_stmt.h"

using namespace ps::motor;


Stepmotor::Stepmotor(emm42::emm42_v5& handle, uint8_t ID, double ratio,
                     uint32_t PPR, uint8_t reach_wnd)
    : handle {handle}, ID {ID}, ratio {ratio}, PPR {PPR}
{
    handle.set_state(ID, true);
    handle.halt(ID);
    handle.set_response(ID, false);
    handle.set_reach_wnd(ID, reach_wnd);
}

bool Stepmotor::set_protect(float lower, float upper)
{
    if ((lower == 0 && upper == 0) || lower < upper)
    {
        this->lower = lower;
        this->upper = upper;
        return true;
    } return false;
}

void Stepmotor::move_to(float position, uint16_t RPM, uint8_t acc, bool sync)
{
    // 限制移动范围
    if (position < lower) position = lower;
    else if (position > upper) position = upper;

    this->position = position;

    // 发送到驱动
    bool dir = position >= 0 ? true : false;
    dir = reverse ? (!dir) : (dir); // 反转控制
    uint32_t step = fabs(position) * ratio;
    arrived(); // 确保到位
    handle.pos_ctl(ID, (dir ? 1 : 0), RPM, acc, step, 1, sync);
}

void Stepmotor::move_to(float position, bool sync)
{
    move_to(position, default_speed, default_acc, sync);
}

void Stepmotor::move(float position, uint16_t RPM, uint8_t acc, bool sync)
{
    // 发送到驱动
    bool dir = position >= 0 ? true : false;
    dir = reverse ? (!dir) : (dir); // 反转控制
    uint32_t step = fabs(position) * ratio;
    arrived(); // 确保到位
    handle.pos_ctl(ID, (dir ? 1 : 0), RPM, acc, step, 0, sync);
}

void Stepmotor::move(float position, bool sync)
{
    move(position, default_speed, default_acc, sync);
}

void Stepmotor::move_speed(float speed, uint8_t acc, bool sync)
{
    // v/s -> pulse/s -> RPM
    // -----------------------------------------
    // (x/t) * (pulse/x) / (pulse / round) = r/t 
    if (speed == 0)
    {
        handle.halt(ID);
        is_moving = false;
    } else
    {
        uint16_t RPM = (fabs(speed) * 60 * ratio) / PPR;
        if (RPM == 0) RPM = 1;

        bool dir = speed >= 0 ? true : false;
        dir = reverse ? (!dir) : (dir); // 反转控制
        is_moving = true;
        handle.speed_ctl(ID, (dir ? 1 : 0), RPM, acc, sync);
    }
}

float Stepmotor::update()
{
    position = ( handle.read_position(ID) / 360. * PPR ) / ratio;
    return position;
}

void Stepmotor::on_click()
{
    if (!calibrating || clicked) // 如果之前按下过，就不要设置零点
        return;

    handle.halt(ID);
    is_moving = false;
    calibrating = false; // 发送过一次halt就不需要再校准了
}

void Stepmotor::calibrate(const std::function<bool(void)>& is_clicked,
                          float speed, uint8_t acc, uint32_t deadzone)
{
    calibrating = true;
    if (is_clicked())
    {
        clicked = true;
        move_speed(-speed, acc);
        // 轮询开关状态
        while (is_clicked())  HAL_Delay(1);
        HAL_Delay(deadzone);
    }
    clicked = false;
    // 向零点移动直到触碰到开关停下来
    move_speed(speed, acc);
    arrived();
    handle.reset(ID); // 以当前位置为新的零点
}

void Stepmotor::arrived()
{
    while (is_moving) HAL_Delay(0); // 速度模式
    handle.arrived(ID); // 位置模式
}