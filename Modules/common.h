#ifndef __GXS_COMMON_H__
#define __GXS_COMMON_H__
/**
 * @brief 比赛共用定义
 */
#include <stdint.h>

const static uint8_t ROUND_COUNT = 2; // 搬运的次数

#ifdef M_PI
#   define PI M_PI
#else
#   define PI 3.1415926
#endif

/**
 * @brief 颜色定义
 */
typedef enum
{
    color_none,
    color_red,
    color_green,
    color_blue
} color_t;

/**
 * @brief 视觉校准点位定义
 */
typedef enum
{
    vision_adjust_PROCESS,
    vision_adjust_STORAGE,
    vision_adjust_PLATE
} vision_adjust_t;

/**
 * @brief int16 二维坐标点
 */
typedef struct
{
    int16_t x;
    int16_t y;
} Point2i;

/**
 * @brief float 二维坐标点
 */
typedef struct
{
    float x;
    float y;
} Point2f;

/**
 * @brief float 三维坐标点
 */
typedef struct
{
    float x;
    float y;
    float z;
} Point3f;

/**
 * @brief float 二维位姿
 */
typedef struct
{
    Point2f xy;
    float angle;
} Pose2f;

/**
 * @brief double 二维坐标点
 */
typedef struct
{
    double x;
    double y;
} Point2d;

#endif