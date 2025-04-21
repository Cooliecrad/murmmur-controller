#ifndef __PS_POINTS_H__
#define __PS_POINTS_H__

#include "common.h"

/************物流搬运初赛场地上的点位**********************/
namespace points
{
    const Point2f ZERO {0.075, 0};
    const Point2f START {0.175, 0.175};
    const Point2f START2 {0.25, 0.175};
    const Point2f SCANQR {0.65,0.175};
    const Point2f MATERIAL {1.51, 0.175};
    const Point2f RIGHT_MID {1.1, 0.175};
    const Pose2f PROCESS {{1.1, 1.95}, 180};
    const Pose2f TEST_STORAGE {{1.1, 1.95}, 0};
    const Point2f LEFT_UP {1.92, 1.95};
    const Pose2f STORAGE {{1.92, 1.13}, 90};
    const Point2f RIGHT_UP {1.92, 0.175};
    const Point2f MATERIAL2 {1.51, 0.18};
}

#endif