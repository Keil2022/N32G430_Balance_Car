#include "n32g430.h"
#include "exti.h"

void EXTI_Init(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;



    /* Enable the GPIO Clock */
    if (GPIOx == GPIOA)
    {
        RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);
		RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    }
    else if (GPIOx == GPIOB)
    {
		RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB);
		RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    }
    else if (GPIOx == GPIOC)
    {
		RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOC);
		RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    }
    else if (GPIOx == GPIOD)
    {
		RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOD);
		RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);		
    }
    else
    {
        return;
    }

    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_Structure_Initialize(&GPIO_InitStructure);
        GPIO_InitStructure.Pin          = Pin;
        GPIO_InitStructure.GPIO_Pull    = GPIO_PULL_UP;
        GPIO_Peripheral_Initialize(GPIOx, &GPIO_InitStructure);
    }

    /* Configure key EXTI Line to key input Pin 配置按键EXTI Line为按键输入Pin */
    GPIO_EXTI_Line_Set(KEY_INPUT_PORT_SOURCE, KEY_INPUT_PIN_SOURCE);

    /* Configure key EXTI line */
    EXTI_InitStructure.EXTI_Line    = KEY_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);

    /* Set key input interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel                   = KEY_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_SUB_PRIORITY_0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}


