#ifndef __KEY_H
#define __KEY_H

#include "n32g430_gpio.h"

#define KEY_Press 	0	//按下
#define KEY_Lift 	1	//松开

#define	True		1
#define	Fault		0

#define KEY_ShortTime	2		//短按判断时间
#define KEY_LongTime	70		//长按判断时间
#define KEY_EndTime		300		//结束按键时间

#define KEY_GPIO_PORT	GPIOB
#define KEY_GPIO_PIN	GPIO_PIN_2
#define KEY 			GPIO_Input_Pin_Data_Get(KEY_GPIO_PORT,KEY_GPIO_PIN)

extern u8 	KEY_Phase;			//按键阶段
extern u16 	KEY_TimeCount;		//按键时间

extern u8 KEY_ShortOK;			//短按有效
extern u8 KEY_LongOK;			//长按有效
extern u8 KEY_EndOK;			//结束按键

void Key_GPIO_Init(void);
void Key_Scanf(void);
void KEY_Handle(void);

#endif

