#ifndef __CHASSIS_PID_H__
#define __CHASSIS_PID_H__

#include <stdint.h>

typedef enum
{
    PID_mode_P,
    PID_mode_PI,
    PID_mode_PD,
    PID_mode_PID,
} PID_mode_t;

typedef struct
{
    PID_mode_t Mode;        //使用哪个模式调节
    
    float EXP;     //用户设定值

    float CV;        //当前值，实际值
 
    float Kp;        //比例系数
    float T;      //PID计算周期--采样周期
    uint16_t   Tdata;    //判断PID周期到没到
    float Ti;        //积分时间常数
    float Td;       //微分系数
    
    
    
    float Ek;          //本次偏差
    float Ek_1;        //上次偏差
    float SEk;         //历史偏差之和
	float DelEk;
	
	float SEK_death;	//当Ek大于此值时，sek=0；
    
    float Iout;        //积分输出
    float Pout;        //比例输出    
    float Dout;        //微分输出
    
    float OUT0;        //一个维持的输出，防止失控
	float MAX_OUT;
	float MIN_OUT;
    float OUT;        //计数最终得到的值
         
} PIDTypeDef;

#ifdef __cplusplus
extern "C" {
#endif

void PID_Init(PIDTypeDef *PID, PID_mode_t PID_Mode, float PID_T, float PID_Kp,
              float PID_Ti, float PID_Td, float PID_MAX_OUT,float PID_MIN_OUT, float sek_death);

void PID_Caclulate(PIDTypeDef *PID, float PID_EXP, float PID_CV);

#ifdef __cplusplus
}
#endif


#endif