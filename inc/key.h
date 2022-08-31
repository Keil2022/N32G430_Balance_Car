#ifndef __KEY_H
#define __KEY_H

#include "n32g430_gpio.h"

#define KEY_Press 	0	//����
#define KEY_Lift 	1	//�ɿ�

#define	True		1
#define	Fault		0

#define KEY_ShortTime	2		//�̰��ж�ʱ��
#define KEY_LongTime	70		//�����ж�ʱ��
#define KEY_EndTime		300		//��������ʱ��

#define KEY_GPIO_PORT	GPIOB
#define KEY_GPIO_PIN	GPIO_PIN_2
#define KEY 			GPIO_Input_Pin_Data_Get(KEY_GPIO_PORT,KEY_GPIO_PIN)

extern u8 	KEY_Phase;			//�����׶�
extern u16 	KEY_TimeCount;		//����ʱ��

extern u8 KEY_ShortOK;			//�̰���Ч
extern u8 KEY_LongOK;			//������Ч
extern u8 KEY_EndOK;			//��������

void Key_GPIO_Init(void);
void Key_Scanf(void);
void KEY_Handle(void);

#endif

