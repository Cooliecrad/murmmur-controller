# 生成字节表
RESET = "1"
SET = "2"

def to_code(number):
    """生成对应4bit的pwm码，MSB
    """
    ret = [SET if number & (0x1<<shift) else RESET for shift in range(4)]
    return ", ".join(ret)

code_table = [to_code(value) for value in range(16)]
for code in code_table:
    print(code+', ')