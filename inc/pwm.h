#ifndef	__PWM_H
#define	__PWM_H

#include "n32g430.h"
#include "n32g430_rcc.h"
#include "n32g430_gpio.h"
#include "n32g430_tim.h"
#include "misc.h"
#include "TIM1_remap.h"
#include "TIM8_remap.h"

#ifndef ADTIM_NUM 
#define ADTIM_NUM     8
#endif

#if ADTIM_NUM == 1

    #define ADTIM   TIM1

#elif ADTIM_NUM == 8

    #define ADTIM   TIM8

#endif

void TIMx_PWM_Init(void);
uint32_t Common_ADTIM_RCC_Initialize(TIM_Module *TIMx, uint32_t hclk_division);
void Common_ADTIM_GPIO_Initialize(TIM_Module* TIMx);
void Common_TIM_Base_Initialize(TIM_Module *TIMx, uint16_t period, uint16_t prescaler);

#endif

