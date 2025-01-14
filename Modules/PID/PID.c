#include "PID.h"
#include <math.h>

void PID_Init(PIDTypeDef *PID, PID_mode_t PID_Mode, float PID_T, float PID_Kp,
              float PID_Ti, float PID_Td, float PID_MAX_OUT,float PID_MIN_OUT, float sek_death)
{
    PID->Mode = PID_Mode;
    PID->T = PID_T;
    PID->Kp = PID_Kp;
    PID->Ti = PID_Ti;
    PID->Td = PID_Td;
    PID->MAX_OUT = PID_MAX_OUT;
    PID->SEK_death = sek_death;
    PID->MIN_OUT= PID_MIN_OUT;
}

void PID_Caclulate(PIDTypeDef *PID, float PID_EXP, float PID_CV)
{
    if (PID->Tdata < (PID->T)) // ��С��������δ��
    {
        PID->Tdata++;
        return;
    }
    PID->EXP = PID_EXP;
    PID->CV = PID_CV;
    PID->Tdata = 0;
    PID->Ek = PID->EXP - PID->CV; // �õ���ǰ��ƫ��ֵ
    if (PID->Ek > 180)
        PID->Ek = PID->Ek - 360;
    else if (PID->Ek < -180)
        PID->Ek = PID->Ek + 360;
    PID->Pout = PID->Kp * PID->Ek; // �������

    PID->SEk += PID->Ek; // ��ʷƫ���ܺ�

    PID->DelEk = PID->Ek - PID->Ek_1; // �������ƫ��֮��
    PID->Ek_1 = PID->Ek;

    PID->Iout = PID->Ti * PID->SEk; // �������
    PID->Dout = PID->Td * PID->DelEk;

    if (fabs(PID->Ek) > PID->SEK_death)
        PID->SEk = 0;

    switch (PID->Mode)
    {
    case PID_mode_P:
        PID->OUT = PID->Pout;
        break;

    case PID_mode_PI:
        PID->OUT = PID->Pout + PID->Iout;
        break;

    case PID_mode_PID:
        PID->OUT = PID->Pout + PID->Iout + PID->Dout;
        break;

    case PID_mode_PD:
        PID->OUT = PID->Pout + PID->Dout;
        break;
    }
    if (fabs(PID->OUT) >= PID->MAX_OUT)
    {
        if (PID->OUT >= 0)
            PID->OUT = PID->MAX_OUT;
        else
            PID->OUT = -PID->MAX_OUT;
    }
    
    if (fabs(PID->OUT) <= PID->MIN_OUT)
    {
        if (PID->OUT >= 0)
            PID->OUT = PID->MIN_OUT;
        else
            PID->OUT = -PID->MIN_OUT;
    }
}