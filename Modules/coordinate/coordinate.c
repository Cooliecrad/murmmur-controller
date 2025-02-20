#include "coordinate.h"
#include <math.h>

Point2f coordinate_transform_Z_rotate(Point2f point, float angle)
{
    Point2f ret;
    float radian, sinr, cosr;
    radian = PI * (angle) / 180;
    sinr = sin(radian), cosr = cos(radian);    
    ret.x = (point.x * cosr + point.y * sinr);
    ret.y = (-point.x * sinr + point.y * cosr);
    
    return ret;
}

Point2f coordinate_transform_XY_shift(Point2f point, Point2f ref)
{
    point.x -= ref.x;
    point.y -= ref.y;
    return point;
}

Point2f coordinate_transform(Point2f point, Pose2f ref)
{
    point = coordinate_transform_XY_shift(point, ref.xy);
    return coordinate_transform_Z_rotate(point, ref.angle);
}

Point2f coordinate_transform_reverse(Point2f point, Pose2f ref)
{
    point = coordinate_transform_Z_rotate(point, -ref.angle);
    ref.xy.x = -ref.xy.x;
    ref.xy.y = -ref.xy.y;
    return coordinate_transform_XY_shift(point, ref.xy);
}

bool point2f_in_range(Point2f point, Point2f lower, Point2f upper)
{
    // 检测范围有效性
    if (lower.x > upper.x && lower.y > upper.y) return false;
    return    (point.x >= lower.x && point.y >= lower.y)
           && (point.x <= upper.x && point.y <= upper.y);
}

float angle_normal_large(float angle)
{
    return angle_normal(fmodf(angle, 360.));
}

// 定义最大迭代次数
#define MAX_ITER 100
// 定义误差阈值
#define EPSILON 1e-6

double atan_taylor(double x) {
    if (x > 1 || x < -1) {
        // 如果 |x| > 1，则使用 atan(x) = pi/2 - atan(1/x)
        double sign = (x > 0) ? 1.0 : -1.0;
        return sign * (3.1415926 / 2 - atan_taylor(1.0 / fabs(x)));
    }

    double term = x;  // 第一项
    double result = term;
    double x_squared = x * x;

    for (int n = 1; n < MAX_ITER; ++n) {
        term *= -x_squared;  // 计算当前项
        double next_term = term / (2 * n + 1);

        result += next_term;

        // 如果收敛到误差阈值，则停止
        if (fabs(next_term) < EPSILON) {
            break;
        }
    }
    return result;
}

float point_to_angle(Point2f point)
{
    if (point.x > 0)
    {
        if (point.y > 0)
            return atan_taylor(point.y/point.x);
        else
            return 2*PI + atan_taylor(point.y/point.x);
    } else
        return PI + atan_taylor(point.y/point.x);
}