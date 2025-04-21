#include "ps_2_lea.h"
#include "stm32h750xx.h"

#define DI()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)      //数据输入引脚

#define CMD_H()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)   //命令位高
#define CMD_L()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)   //命令位低

#define CS_H()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)    //CS拉高(别名ATT)
#define CS_L()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)    //CS拉低(别名ATT)

#define CLK_H()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)    //时钟拉高
#define CLK_L()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)    //时钟拉低

uint8_t CMD[3] = { 0x01,0x42,0x00 };  // 请求接受数据
uint8_t PS2OriginalValue[9] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };   //存储手柄返回数据
uint8_t RockerValue[4] = { 0x00,0x00,0x00,0x00 };  //摇杆模拟值
uint8_t ButtonValue[16] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };  //所有按键状态值
uint8_t i,mode;

/**
  * @简  述  PS2数据读写函数
  * @参  数  cmd:要写入的命令
  * @返回值  读出数据
  */
static uint8_t PS2_ReadWriteData(uint8_t cmd)
{
	volatile uint8_t res = 0;
	volatile uint8_t ref;
	
	//写入命令，并读取一个1字节数据
	for(ref = 0x01; ref > 0x00; ref <<= 1)
	{
		////输出一位数据
		if(ref&cmd) CMD_H();	
		else CMD_L();
		CLK_L();
		Delay_us(16);
		
		//读取一位数据
		if(DI()) res |= ref;
		CLK_H();
		Delay_us(16);		
	}
	
    return res;						//返回读出数据
	
}

//获取ps2手柄返回的初始值
void PS2OriginalValueGet(void)
{
	short i = 0;
	CS_L();																																	//拉低CS，开始通信
	Delay_us(20);
	PS2OriginalValue[0] = PS2_ReadWriteData(CMD[0]);												//发送开始命令0x01
	PS2OriginalValue[1] = PS2_ReadWriteData(CMD[1]);												//发送0x42请求数据，手柄返回ID，0x41=数字, 0x23=NegCon, 0x73=模拟红灯, 0x53=模拟绿灯
	PS2OriginalValue[2] = PS2_ReadWriteData(CMD[2]);												//手柄返回0x5A，准备传送数据
	for (i = 3; i < 9; i++)
	{
		PS2OriginalValue[i] = PS2_ReadWriteData(PS2OriginalValue[1]);					//一次返回数据，并存入PS2OriginalValue
	}
	CS_H();																																	//拉高CS，结束通信
}

//判断是否有按键被触发
uint8_t Deter_Press(void)
{
	for (i = 0 ; i < 4 ; i ++)
	{
		if (RockerValue[i] == 0xFF || RockerValue[i] == 0x00) return 1;
	}
	for (i = 0 ; i < 16 ; i ++)
	{
		if (ButtonValue[i] == 0x01) return 1;
	}
	return 0;
}	

//解码按键值
void ButtonValueGet(void)
{
	uint8_t bit = 1;
	uint8_t button = 0;
	
	//解码PS2OriginalValue[3]中的数据，存入ButtonValue[0__7]
	for (bit = 8; bit > 0; bit--)
	{
		bit -= 1;
		ButtonValue[button] = (PS2OriginalValue[3] & (1 << bit)) >> bit;
		bit += 1;
		button++;
	}
	
	//解码PS2OriginalValue[4]中的数据，存入ButtonValue[8__15]
	for (bit = 8; bit > 0; bit--)
	{
		bit -= 1;
		ButtonValue[button] = (PS2OriginalValue[4] & (1 << bit)) >> bit;
		bit += 1;
		button++;
	}
	
	//对ButtonValue中的数据依次取反
	for (button = 0; button < 16; button++)
	{
		if (ButtonValue[button] == 1)  ButtonValue[button] = 0;
		else  ButtonValue[button] = 1;
	}
}

//解码摇杆值
void RockerValueGet(void)
{
	int i;
	
	//依次解码PS2OriginalValue[5__8]，并存入RockerValue
	for (i = 5; i < 9; i++)
	{
		PS2OriginalValue[i] = (int)PS2OriginalValue[i];
		RockerValue[i - 5] = PS2OriginalValue[i];
	}
}

//清除PS2OriginalValueClear中的数据
void PS2OriginalValueClear(void)
{
	for (i = 0; i < 9; i++)
	{
		PS2OriginalValue[i] = 0x00; 
	}
	for (i = 5; i < 9; i++)
	{
		PS2OriginalValue[i] = 0x7F; 
	}
}

//更新ps2手柄的数据
void PS2AllValueUpdate(void)
{
	PS2OriginalValueGet(); 
	RockerValueGet();
	ButtonValueGet();
	mode = PS2OriginalValue[1];
	PS2OriginalValueClear();
//	HAL_Delay(1);
}

//判断手柄是否处于绿灯工作模式
uint8_t Work_Green(void)
{
	if (mode == 0X73) return 1;
	else return 0;	
}

//判断手柄是否处于红灯工作模式
uint8_t Work_Red(void)
{
	if (mode == 0X41) return 1;
	else return 0;	
}

//开始键是否被按下
uint8_t Start(void)
{
	if (ButtonValue[4] == 0x01) return 1;
	else return 0;
}

//选择键是否被按下
uint8_t Select(void)
{
	if (ButtonValue[7] == 0x01) return 1;
	else return 0;
}

//左摇杆是否向左拨
uint8_t lJoy_Left(void)
{
	if (RockerValue[2] == 0x00) return 1;
	else return 0;
}

//左摇杆是否向右拨
uint8_t lJoy_Right(void)
{
	if (RockerValue[2] == 0xFF) return 1;
	else return 0;
}

//左摇杆是否向上拨
uint8_t lJoy_Up(void)
{
	if (RockerValue[3] == 0x00) return 1;
	else return 0;
}

//左摇杆是否向下拨
uint8_t lJoy_Down(void)
{
	if (RockerValue[3] == 0xFF) return 1;
	else return 0;
}

//左摇杆是否被按下
uint8_t lJoy_Press(void)
{
	if (ButtonValue[6] == 0x01) return 1;
	else return 0;
}

//右摇杆是否向左拨
uint8_t rJoy_Left(void)
{
	if (RockerValue[0] == 0x00) return 1;
	else return 0;
}

//右摇杆是否向右拨
uint8_t rJoy_Right(void)
{
	if (RockerValue[0] == 0xFF) return 1;
	else return 0;
}

//右摇杆是否向上拨
uint8_t rJoy_Up(void)
{
	if (RockerValue[1] == 0x00) return 1;
	else return 0;
}

//右摇杆是否向下拨
uint8_t rJoy_Down(void)
{
	if (RockerValue[1] == 0xFF) return 1;
	else return 0;
}

//右摇杆是否被按下
uint8_t rJoy_Press(void)
{
	if (ButtonValue[5] == 0x01) return 1;
	else return 0;
}

//方向左键是否被按下
uint8_t Dir_Left(void)
{
	if (ButtonValue[0] == 0x01) return 1;
	else return 0;
}

//方向右键是否被按下
uint8_t Dir_Right(void)
{
	if (ButtonValue[2] == 0x01) return 1;
	else return 0;
}

//方向上键是否被按下
uint8_t Dir_Up(void)
{
	if (ButtonValue[3] == 0x01) return 1;
	else return 0;
}

//方向下键是否被按下
uint8_t Dir_Down(void)
{
	if (ButtonValue[1] == 0x01) return 1;
	else return 0;
}

//功能左键是否被按下
uint8_t Fun_Left(void)
{
	if (ButtonValue[8] == 0x01) return 1;
	else return 0;
}

//功能右键是否被按下
uint8_t Fun_Right(void)
{
	if (ButtonValue[10] == 0x01) return 1;
	else return 0;
}

//功能上键是否被按下
uint8_t Fun_Up(void)
{
	if (ButtonValue[11] == 0x01) return 1;
	else return 0;
}

//功能下键是否被按下
uint8_t Fun_Down(void)
{
	if (ButtonValue[9] == 0x01) return 1;
	else return 0;
}

//左上肩键是否被按下
uint8_t lSho_Up(void)
{
	if (ButtonValue[13] == 0x01) return 1;
	else return 0;
}

//左下肩键是否被按下
uint8_t lSho_Down(void)
{
	if (ButtonValue[15] == 0x01) return 1;
	else return 0;
}

//右上肩键是否被按下
uint8_t rSho_Up(void)
{
	if (ButtonValue[12] == 0x01) return 1;
	else return 0;
}

//右下肩键是否被按下
uint8_t rSho_Down(void)
{
	if (ButtonValue[14] == 0x01) return 1;
	else return 0;
}
