#include "n32g430.h"
#include "pwm.h"

uint16_t TimerPeriod   = 0;
//uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, channel4Pulse = 0;

uint32_t ADTIMClockFrequency = 0;

void TIMx_PWM_Init(void)
{
	OCInitType TIM_OCInitStructure;
	TIM_BDTRInitType TIM_BDTRInitStructure;
	
	ADTIMClockFrequency = Common_ADTIM_RCC_Initialize(ADTIM, RCC_HCLK_DIV2);	//TIM1 
	
	Common_ADTIM_GPIO_Initialize(ADTIM);
	
	/* 计算AR寄存器中设置的值，以产生10 Khz的信号频率 */
    TimerPeriod = (ADTIMClockFrequency / 10000) - 1;
//	/* 计算CCDAT1值，为通道1生成50%的占空比 */
//    Channel1Pulse = (uint16_t)(((uint32_t)5 * (TimerPeriod - 1)) / 10);
//    /*计算CCDAT2值，为通道2生成25%的占空比 */
//    Channel2Pulse = (uint16_t)(((uint32_t)25 * (TimerPeriod - 1)) / 100);
//    /* 计算CCDAT3值，为通道3生成12.5%的占空比 */
//    Channel3Pulse = (uint16_t)(((uint32_t)125 * (TimerPeriod - 1)) / 1000);
//    /* 计算CCDAT4值，为通道4生成12.5%的占空比 */
//    channel4Pulse = (uint16_t)(((uint32_t)125 * (TimerPeriod - 1)) / 1000);
	
	/* TIM Base Init, Period = TimerPeriod, Prescaler = 0 */
    Common_TIM_Base_Initialize(ADTIM, TimerPeriod, 0);
	
	TIM_Output_Channel_Struct_Initialize(&TIM_OCInitStructure);
	
	/* 通道1、2、3 PWM模式配置 */
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = 0;
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_LOW;
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_LOW;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_SET;
    TIM_OCInitStructure.OcNIdleState = TIM_OCN_IDLE_STATE_RESET;
    TIM_Output_Channel1_Initialize(ADTIM, &TIM_OCInitStructure);
	
	//TIM_OCInitStructure.Pulse = Channel2Pulse;
    TIM_Output_Channel2_Initialize(ADTIM, &TIM_OCInitStructure);

    //TIM_OCInitStructure.Pulse = Channel3Pulse;
    TIM_Output_Channel3_Initialize(ADTIM, &TIM_OCInitStructure);

    /* 输出比较Active模式配置:Channel4 */
    /* The CH4N if only for ADTIM, not for ADTIM */
    //TIM_OCInitStructure.Pulse = channel4Pulse;
    TIM_Output_Channel4_Initialize(ADTIM, &TIM_OCInitStructure);
	
	/* 自动输出使能，断线，死区时间和锁定配置*/
    TIM_Break_And_Dead_Time_Struct_Initialize(&TIM_BDTRInitStructure);
    
    TIM_BDTRInitStructure.OssrState       = TIM_OSSR_STATE_ENABLE;
    TIM_BDTRInitStructure.OssiState       = TIM_OSSI_STATE_ENABLE;
    TIM_BDTRInitStructure.LockLevel       = TIM_LOCK_LEVEL_1;
    TIM_BDTRInitStructure.DeadTime        = 11;
    TIM_BDTRInitStructure.Break           = TIM_BREAK_IN_ENABLE;
    TIM_BDTRInitStructure.BreakPolarity   = TIM_BREAK_POLARITY_HIGH;
    TIM_BDTRInitStructure.AutomaticOutput = TIM_AUTO_OUTPUT_ENABLE;
    TIM_BDTRInitStructure.IomBreakEn      = true;

    TIM_Break_And_Dead_Time_Set(ADTIM, &TIM_BDTRInitStructure);

    TIM_On(ADTIM);
    TIM_PWM_Output_Enable(ADTIM);
}

uint32_t Common_ADTIM_RCC_Initialize(TIM_Module *TIMx, uint32_t hclk_division)
{
    uint32_t ADTIM_clock;

    RCC_ClocksType RCC_Clocks;

    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA | RCC_AHB_PERIPH_GPIOB |
                                    RCC_AHB_PERIPH_GPIOC | RCC_AHB_PERIPH_GPIOD);

    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    
    RCC_Pclk2_Config(hclk_division);	//配置高速APB时钟PCLK2。

    if(TIM1 == TIMx)
    {
        RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_TIM1);
    
    }
    else if(TIM8 == TIMx)
    {
        RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_TIM8);
    }
    else
    {
        /* none */
    }

    RCC_Clocks_Frequencies_Value_Get(&RCC_Clocks);	//返回不同芯片时钟的频率。

    
    if(RCC_Clocks.HclkFreq > RCC_Clocks.Pclk2Freq) 
    {
        ADTIM_clock = RCC_Clocks.Pclk2Freq * 2; 
    }
    else
    {
        ADTIM_clock = RCC_Clocks.Pclk2Freq;
    }
    return ADTIM_clock;
}

void Common_ADTIM_GPIO_Initialize(TIM_Module* TIMx)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);

    if(TIM1 == TIMx)
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Current = GPIO_DS_4MA;

//        /* TIM1 CHx pins init */
//        GPIO_InitStructure.Pin        = TIM1_REMAP0_CH1_PIN;
//        GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH1_AF;
//        GPIO_Peripheral_Initialize(TIM1_REMAP0_CH1_PORT, &GPIO_InitStructure);

//        GPIO_InitStructure.Pin        = TIM1_REMAP0_CH2_PIN;
//        GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH2_AF;
//        GPIO_Peripheral_Initialize(TIM1_REMAP0_CH2_PORT, &GPIO_InitStructure);

//        GPIO_InitStructure.Pin        = TIM1_REMAP0_CH3_PIN;
//        GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH3_AF;
//        GPIO_Peripheral_Initialize(TIM1_REMAP0_CH3_PORT, &GPIO_InitStructure);

//        GPIO_InitStructure.Pin        = TIM1_REMAP0_CH4_PIN;
//        GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH4_AF;
//        GPIO_Peripheral_Initialize(TIM1_REMAP0_CH4_PORT, &GPIO_InitStructure);

//        /* TIM1 CHxN pins ini. CH4N is only for TIM1 */
//        GPIO_InitStructure.Pin        = TIM1_REMAP0_CH1N_PIN;
//        GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH1N_AF;
//        GPIO_Peripheral_Initialize(TIM1_REMAP0_CH1N_PORT, &GPIO_InitStructure);

//        GPIO_InitStructure.Pin        = TIM1_REMAP0_CH2N_PIN;
//        GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH2N_AF;
//        GPIO_Peripheral_Initialize(TIM1_REMAP0_CH2N_PORT, &GPIO_InitStructure);

//        GPIO_InitStructure.Pin        = TIM1_REMAP0_CH3N_PIN;
//        GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH3N_AF;
//        GPIO_Peripheral_Initialize(TIM1_REMAP0_CH3N_PORT, &GPIO_InitStructure);

//        GPIO_InitStructure.Pin        = TIM1_REMAP0_CH4N_PIN;
//        GPIO_InitStructure.GPIO_Alternate = TIM1_REMAP0_CH4N_AF;
//        GPIO_Peripheral_Initialize(TIM1_REMAP0_CH4N_PORT, &GPIO_InitStructure);
    }
    else if(TIM8 == TIMx)
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Current = GPIO_DS_4MA;

        /* TIM1 CHx pins init */
        GPIO_InitStructure.Pin        = TIM8_REMAP0_CH1_PIN;
        GPIO_InitStructure.GPIO_Alternate = TIM8_REMAP0_CH1_AF;
        GPIO_Peripheral_Initialize(TIM8_REMAP0_CH1_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.Pin        = TIM8_REMAP0_CH2_PIN;
        GPIO_InitStructure.GPIO_Alternate = TIM8_REMAP0_CH2_AF;
        GPIO_Peripheral_Initialize(TIM8_REMAP0_CH2_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.Pin        = TIM8_REMAP0_CH3_PIN;
        GPIO_InitStructure.GPIO_Alternate = TIM8_REMAP0_CH3_AF;
        GPIO_Peripheral_Initialize(TIM8_REMAP0_CH3_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.Pin        = TIM8_REMAP0_CH4_PIN;
        GPIO_InitStructure.GPIO_Alternate = TIM8_REMAP0_CH4_AF;
        GPIO_Peripheral_Initialize(TIM8_REMAP0_CH4_PORT, &GPIO_InitStructure);
    }
}

void Common_TIM_Base_Initialize(TIM_Module *TIMx, uint16_t period, uint16_t prescaler)
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
	
	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = period;
    TIM_TimeBaseStructure.Prescaler = prescaler;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

    TIM_Base_Initialize(TIMx, &TIM_TimeBaseStructure);   
}



