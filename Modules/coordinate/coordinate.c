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