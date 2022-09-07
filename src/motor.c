#include "n32g430.h"
#include "motor.h"

char PWM_Zero = 0;

/*限幅函数*/
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/*绝对值函数*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

extern uint16_t Channel1Pulse;

/*赋值函数*/
/*入口参数：PID运算完成后的最终PWM值*/
void Load(int moto1,int moto2)//moto1=-200：反转200个脉冲
{
	//1.研究正负号，对应正反转
	if(moto1>0)	{ TIM_Compare1_Set(ADTIM, 0 ); 				TIM_Compare2_Set(ADTIM, GFP_abs(moto1) ); }		//正转
	else 		{ TIM_Compare1_Set(ADTIM, GFP_abs(moto1) ); TIM_Compare2_Set(ADTIM, 0 ); }					//反转
	
	if(moto2>0)	{ TIM_Compare3_Set(ADTIM, GFP_abs(moto2) ); TIM_Compare4_Set(ADTIM, 0 ); }					//正转
	else 		{ TIM_Compare3_Set(ADTIM, 0 ); 				TIM_Compare4_Set(ADTIM, GFP_abs(moto2) ); }		//反转	
}

void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(GFP_abs(*Jiaodu-*Med_Jiaodu)>20)	Load(PWM_Zero,PWM_Zero);
}




