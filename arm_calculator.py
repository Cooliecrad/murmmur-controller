"""机械臂与摄像头之间的参数转换计算"""
import math
from sys import argv

if __name__ == "__main__":
    if len(argv) < 7:
        print("参数数量不足，请填入 S1 S2 X1 X2 Y1 Y2 SHIFT_R")
        _argv = [float(input()) for x in range(6)]
    else:
        _argv = [float(x) for x in argv[1:]]

    s1, s2, x1, x2, y1, y2, sr = _argv
    # 计算减速比
    s1 = ( s1 + sr )
    s2 = ( s2 + sr )

    s1 = math.radians(s1)
    s2 = math.radians(s2)
    ts1 = math.tan(s1)
    ts2 = math.tan(s2)
    Cx = ( ts1 * x1 - ts2 * x2 - ( y1 - y2 ) ) / ( ts1 - ts2 )
    Cy = y1 - (x1 - Cx) * ts1
    print(f"X = {Cx}, Y = {Cy}")