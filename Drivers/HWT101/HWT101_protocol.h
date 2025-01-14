#ifndef __HWT101_PROTOCOL_H__
#define __HWT101_PROTOCOL_H__

#include <stdint.h>

typedef enum
{
    HWT101_cmd_W = 0x52,
    HWT101_cmd_Yaw = 0x53,
} HWT101_cmd_t;

#pragma pack(1)
typedef struct
{
    uint16_t dummy1;
    int16_t WY;
    int16_t WZ;
    uint16_t dummy2;
} HWT101_angle_vec_t;

typedef struct
{
    uint32_t dummy;
    int16_t yaw;
    uint16_t version;
} HWT101_yaw_t;

typedef struct
{
    uint8_t header;
    uint8_t type;
    union
    {
        uint8_t dummy[8];
        HWT101_angle_vec_t W;
        HWT101_yaw_t yaw;
    };
    uint8_t checksum;
} HWT101_protocol_t;
#pragma pack()

#endif