#ifndef  _CONTROL_H
#define  _CONTROL_H

#include "motor.h"

extern float Med_Angle;		//机械中值。---在这里修改你的机械中值即可。
extern float Target_Speed;	//期望速度。---二次开发接口，用于控制小车前进后退及其速度。

extern float 
	Vertical_Kp,	//直立环KP、KD
	Vertical_Kd;
extern float 
	Velocity_Kp,	//速度环KP、KI
	Velocity_Ki;

extern int Vertical_out,Velocity_out,Turn_out;	//直立环&速度环&转向环 的输出变量

extern float Pitch,Roll,Yaw;			//角度
extern short gyrox,gyroy,gyroz;			//陀螺仪--角速度
extern short aacx,aacy,aacz;			//加速度
extern int Encoder_Left,Encoder_Right;	//编码器数据（速度）
extern int MOTO1,MOTO2;					//电机装载变量

int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z);
void EXTI9_5_IRQHandler(void);

#endif


