#ifndef __ESP12F_USART_H
#define __ESP12F_USART_H

#include "n32g430_gpio.h"

//ESP12F Config
#define ESP12F_IO0_PORT			GPIOA
#define ESP12F_IO0_PIN			GPIO_PIN_12

//IO²Ù×÷º¯Êý
#define ESP12F_IO0_SET()		do{GPIO_Pins_Set(ESP12F_IO0_PORT,ESP12F_IO0_PIN);}while(0);
#define ESP12F_IO0_RESET() 		do{GPIO_Pins_Reset(ESP12F_IO0_PORT,ESP12F_IO0_PIN);}while(0);

void ESP12F_IO0_Init(void);

#endif



