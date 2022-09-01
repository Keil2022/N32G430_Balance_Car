#ifndef __LED_H
#define __LED_H

#include "n32g430_gpio.h"

//ESP12F Config
#define LED_GPIO_PORT			GPIOC
#define LED_GPIO_PIN			GPIO_PIN_13

//IO²Ù×÷º¯Êý
#define LED_GPIO_SET()			{ do{GPIO_Pins_Set(LED_GPIO_PORT,LED_GPIO_PIN);}while(0); }
#define LED_GPIO_RESET() 		{ do{GPIO_Pins_Reset(LED_GPIO_PORT,LED_GPIO_PIN);}while(0); }

#define LED_On		LED_GPIO_RESET()
#define	LED_Off		LED_GPIO_SET()
#define LED_Blink	GPIO_Pin_Toggle(LED_GPIO_PORT,LED_GPIO_PIN)

void LED_GPIO_Init(void);

#endif



