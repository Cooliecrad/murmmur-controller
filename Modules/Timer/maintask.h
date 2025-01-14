#ifndef __MAINTASK_H__
#define __MAINTASK_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void my_debug(uint8_t flag);
void Task(void);
void Taskpoint(void);

#ifdef __cplusplus
}
#endif

#endif 