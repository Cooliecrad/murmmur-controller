#include "servor.h"
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 翅膀控制
 * @param open_close 翅膀状态，true关闭/ false打开
 * @param sync 在函数中等待操作完毕
 */
void wing_ctl(bool open_close, bool sync);

/**
 * @brief 机械爪控制
 * @param open_close 机械爪状态，true关闭/ false打开
 * @param sync 在函数中等待操作完毕
 */
void claw_ctl(bool close_open, bool sync);

#ifdef __cplusplus
}
#endif