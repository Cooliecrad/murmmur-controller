#include "servor.h"
#include <stdbool.h>

/**
 * @brief 具体舵机控制
 */

typedef struct
{
    uint8_t Num; // 编号
    uint16_t Time; // 时间
    float *angle; // 可用位置数组
} servor_t;

typedef enum
{
    servor_object_CLAW = 0x0,
    servor_object_PLATES = 0x1,
} servor_object_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化舵机控制器
 */
void servor_ctl_init(UART_HandleTypeDef *pHUART);

/**
 * @brief 控制舵机
 * @param object 要控制的舵机组
 * @param state 要设置成的状态
 * @param sync 是否等待舵机运动到指定位置
 */
void servor_ctl(servor_object_t object, int state, bool sync);

#ifdef __cplusplus
}
#endif