#ifndef __ADC_H
#define __ADC_H

#define ADC_GPIO_PORT	GPIOB
#define ADC_GPIO_PIN	GPIO_PIN_1

extern __IO u16 ADC_Value;

void ADC_RCC_Configuration(void);
void ADC_GPIO_Configuration(void);
void ADC_Configuration(void);

#endif

