#ifndef __MOTOR_H
#define __MOTOR_H

#define PWM_MAX	12800	//PWMÏÞ·ù±äÁ¿
#define PWM_MIN -12800	

extern char PWM_Zero,stop;

void Limit(int *motoA,int *motoB);
int GFP_abs(int p);
void Load(int moto1,int moto2);

#endif

