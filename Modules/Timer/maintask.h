#ifndef __MAINTASK_H__
#define __MAINTASK_H__

#include <stdint.h>

/**
 * @brief 安全移动模式。启动之后每次点位间移动会调整车子的姿态
 */
// #define __SAFE_MOVE_MODE

#ifdef __cplusplus
extern "C" {
#endif

void my_debug(uint8_t flag);
void Taskpoint(void);

#ifdef __cplusplus
}
#endif

#endif 