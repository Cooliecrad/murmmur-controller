#ifndef __MAINTASK_H__
#define __MAINTASK_H__

#include "common.h"

namespace points
{
    extern const Point2f ZERO;
    extern const Point2f START;
    extern const Point2f START2;
    extern const Point2f SCANQR;
    extern const Pose2f MATERIAL;
    extern const Point2f RIGHT_MID;
    extern const Pose2f PROCESS;
    extern const Pose2f TEST_STORAGE;
    extern const Point2f LEFT_UP;
    extern const Pose2f STORAGE;
    extern const Point2f RIGHT_UP;
    extern const Point2f MATERIAL2;
}

void task_init();

/**
 * @brief 取物料任务
 */
void materials_task(uint8_t round);

/**
 * @brief 处理区任务
 */
void process_place_task(uint8_t round);

/**
 * @brief 存储区任务
 */
void storage_place_task(uint8_t round);

void task(void);

#endif 