#include "n32g430.h"
#include "motor.h"

char PWM_Zero = 0;

/*�޷�����*/
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/*����ֵ����*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

extern uint16_t Channel1Pulse;

/*��ֵ����*/
/*��ڲ�����PID������ɺ������PWMֵ*/
void Load(int moto1,int moto2)//moto1=-200����ת200������
{
	//1.�о������ţ���Ӧ����ת
	if(moto1>0)	{ TIM_Compare1_Set(ADTIM, 0 ); 				TIM_Compare2_Set(ADTIM, GFP_abs(moto1) ); }		//��ת
	else 		{ TIM_Compare1_Set(ADTIM, GFP_abs(moto1) ); TIM_Compare2_Set(ADTIM, 0 ); }					//��ת
	
	if(moto2>0)	{ TIM_Compare3_Set(ADTIM, GFP_abs(moto2) ); TIM_Compare4_Set(ADTIM, 0 ); }					//��ת
	else 		{ TIM_Compare3_Set(ADTIM, 0 ); 				TIM_Compare4_Set(ADTIM, GFP_abs(moto2) ); }		//��ת	
}

void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(GFP_abs(*Jiaodu-*Med_Jiaodu)>20)	Load(PWM_Zero,PWM_Zero);
}




