#include "remote_control.h"

#include "vision.h"
#include "Arm.h"

#define __PCALL_SERVER
#include "mmcar_table.h"

void arm_r_rotate(float position)
{
    motor_r->move_to(position);
}

static ps::pcall::Server server {pcall_init()};

static uint8_t RCBuffer[128];
void remote_control_nowait()
{
    auto data = vision_get_RC();
    if (data.data)
    {
        memcpy(RCBuffer, data.data, data.length);
        server.parse(RCBuffer, data.length);
    }
}