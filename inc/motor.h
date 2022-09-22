#ifndef __MOTOR_H
#define __MOTOR_H

#include "pwm.h"

extern uint16_t TimerPeriod;

#define PWM_MAX	( TimerPeriod-1 )	//PWMÏÞ·ù±äÁ¿
#define PWM_MIN ( -PWM_MAX )

extern int PWM_Zero;

void Limit(int *motoA,int *motoB);
int GFP_abs(int p);
void Load(int moto1,int moto2);
void Stop(float *Med_Jiaodu,float *Jiaodu);

#endif

