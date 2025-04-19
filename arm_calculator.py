"""机械臂与摄像头之间的参数转换计算"""
import math
from sys import argv

if __name__ == "__main__":
    if len(argv) < 7:
        print("参数数量不足，请填入 R1 S1 R2 S2 R3 S3 CLAW_ATTR_X")
        _argv = [float(input()) for x in range(7)]
    else:
        _argv = [float(x) for x in argv[1:]]

    R1, S1, R2, S2, R3, S3, CAX = _argv
    R1 += CAX
    R2 += CAX
    R3 += CAX
    S1 = math.radians(S1-180)
    S2 = math.radians(S2-180)
    S3 = math.radians(S3-180)
    sin1 = math.sin(S1)
    sin2 = math.sin(S2)
    sin3 = math.sin(S3)
    cos1 = math.cos(S1)
    cos2 = math.cos(S2)
    cos3 = math.cos(S3)
    sin21 = sin2 - sin1
    sin32 = sin3 - sin2
    cos21 = cos2 - cos1
    cos32 = cos3 - cos1
    RS21 = R2 * sin2 - R1 * sin1
    RS32 = R3 * sin3 - R2 * sin2
    RC21 = R2 * cos2 - R1 * cos1
    RC32 = R3 * cos3 - R2 * cos2
    cc2132 = cos21 - cos32
    ss2132 = sin21 - sin32
    cs2132 = cc2132 / ss2132
    Cy = (( (RS21 - RS32) / ss2132 ) - ( (RC21 - RC32) / cc2132 )) / (cs2132 - 1/cs2132)
    Cx = cs2132 * Cy - (RS21 - RS32) / ss2132
    
    print(f"Cx{Cx}, Cy{Cy}")