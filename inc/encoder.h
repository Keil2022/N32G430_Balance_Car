#ifndef __ENCODER_H
#define __ENCODER_H

#include "timer_common.h"

#define EN_TIM2_GPIO_PORT	GPIOA
#define EN_TIM2_GPIO_PIN	GPIO_PIN_0 | GPIO_PIN_1

void Encoder_TIM2_Init(void);
void Encoder_TIM2_Init(void);
int Read_Speed(int TIMx);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
	
#endif


