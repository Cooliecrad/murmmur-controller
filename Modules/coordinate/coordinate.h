#ifndef __PS_COORDINATE_H__
#define __PS_COORDINATE_H__
/**
 * @note 考虑到坐标变换存在大量的类型转换和潜在的计算错误，坐标变换建议使用这种含义
 * 明确的方法。而且通过这样，所有的代码都是自注释的，不需要额外的解释
 */

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 获得point在新坐标系下的坐标。新坐标系 绕着 旧坐标系绕Z轴 旋转 angle角度
 * @param point 旧坐标系下的点
 * @param angle 新坐标系旋转的角度，采用旋转角
 * @return 新坐标系下的点
 */
Point2f coordinate_transform_Z_rotate(Point2f point, float angle);

/**
 * @brief 获得point在新坐标系下的坐标。新坐标系原点 在 旧坐标系 的坐标为 ref
 * @param point 旧坐标系下的点
 * @param ref 新坐标系原点在旧坐标系的坐标为
 */
Point2f coordinate_transform_XY_shift(Point2f point, Point2f ref);

/**
 * @brief 获得point在新坐标系下的坐标。
 * @param point 旧坐标系下的点
 * @param ref 新坐标系在旧坐标系下的二维位姿
 * @return 新坐标系下的点
 */
Point2f coordinate_transform(Point2f point, Pose2f ref);

/**
 * @brief 通过新坐标系的位姿和新坐标系下的点，获得point在旧坐标系下的坐标。
 * 是coordinate_transform的逆过程。
 * @param point 新坐标系下的点
 * @param ref 新坐标系在旧坐标系下的二维位姿
 * @return 旧坐标系下的点
 */
Point2f coordinate_transform_reverse(Point2f point, Pose2f ref);

#ifdef __cplusplus
}
#endif

#endif