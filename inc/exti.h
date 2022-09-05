#ifndef __EXTI_H
#define __EXTI_H

#include "n32g430.h"

#define KEY_INPUT_PORT        GPIOB
#define KEY_INPUT_PIN         GPIO_PIN_6
#define KEY_INPUT_EXTI_LINE   EXTI_LINE5
#define KEY_INPUT_PORT_SOURCE EXTI_LINE_SOURCE5
#define KEY_INPUT_PIN_SOURCE  AFIO_EXTI_PB6
#define KEY_INPUT_IRQn        EXTI9_5_IRQn

#define Exti	 GPIO_Input_Pin_Data_Get(KEY_INPUT_PORT,KEY_INPUT_PIN)

void EXTI_Init(GPIO_Module* GPIOx, uint16_t Pin);

#endif

