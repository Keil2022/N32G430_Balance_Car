#ifndef  _CONTROL_H
#define  _CONTROL_H

#include "motor.h"

extern float Med_Angle;		//��е��ֵ��---�������޸���Ļ�е��ֵ���ɡ�
extern float Target_Speed;	//�����ٶȡ�---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�

extern float 
	Vertical_Kp,	//ֱ����KP��KD
	Vertical_Kd;
extern float 
	Velocity_Kp,	//�ٶȻ�KP��KI
	Velocity_Ki;

extern int Vertical_out,Velocity_out,Turn_out;	//ֱ����&�ٶȻ�&ת�� ���������

extern float Pitch,Roll,Yaw;			//�Ƕ�
extern short gyrox,gyroy,gyroz;			//������--���ٶ�
extern short aacx,aacy,aacz;			//���ٶ�
extern int Encoder_Left,Encoder_Right;	//���������ݣ��ٶȣ�
extern int MOTO1,MOTO2;					//���װ�ر���

int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z);
void EXTI9_5_IRQHandler(void);

#endif


