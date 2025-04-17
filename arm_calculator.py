"""机械臂与摄像头之间的参数转换计算"""
import math
from sys import argv

if __name__ == "__main__":
    if len(argv) < 7:
        print("参数数量不足，请填入 R1 S1 R2 S2 R3 S3")
        _argv = [float(input()) for x in range(6)]
    else:
        _argv = [float(x) for x in argv[1:]]

    R1, S1, R2, S2, R3, S3 = _argv
    S1 = math.radians(S1)
    S2 = math.radians(S2)
    S3 = math.radians(S3)
    sin1 = math.sin(S1)
    sin2 = math.sin(S2)
    sin3 = math.sin(S3)
    cos1 = math.cos(S1)
    cos2 = math.cos(S2)
    cos3 = math.cos(S3)
    sin21 = sin2 - sin1
    sin32 = sin3 - sin2
    cos21 = cos2 - cos1
    cos32 = 