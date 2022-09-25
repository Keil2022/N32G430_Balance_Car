#ifndef __ULTR_H
#define __ULTR_H

#include "main.h"

#define ULTR_GPIO_PORT	GPIOB
#define ULTR_GPIO_PIN	GPIO_PIN_3
#define ULTR_Status 	GPIO_Input_Pin_Data_Get(ULTR_GPIO_PORT,ULTR_GPIO_PIN)

void ULTR_Init(void);


#endif

