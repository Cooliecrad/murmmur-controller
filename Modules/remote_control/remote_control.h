#ifndef __REMOTE_CONTROL_H__
#define __REMOTE_CONTROL_H__

/**
 * @brief 非阻塞的读取一条函数调用并执行，如果没有函数调用，就不执行
 */
void remote_control_nowait();

#endif