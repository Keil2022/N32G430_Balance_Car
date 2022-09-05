#include "n32g430.h"
#include "encoder.h"
#include "GPTIM_common.h"

uint32_t GPTIMClockFrequency = 0;

void Encoder_TIM2_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	TIM_TimeBaseInitType TIM_TimeBaseStructure;
	TIM_ICInitType TIM_ICInitStructure;
	
	GPTIMClockFrequency = Common_GPTIM_RCC_Initialize(TIM2, RCC_HCLK_DIV4);		//配置相关时钟

	GPIO_InitStructure.Pin            = EN_TIM2_GPIO_PIN;	//IO口
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;	//输入模式
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM2;		//TIM2复用PA0 PA1
	GPIO_Peripheral_Initialize(EN_TIM2_GPIO_PORT, &GPIO_InitStructure);	//配置GPIO
	
	//Common_TIM_NVIC_Initialize(TIM2_IRQn, ENABLE);	//需要中断再配置
	
	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);		//填入缺省值
    TIM_TimeBaseStructure.Period    = 65535;			//重装载值
    TIM_TimeBaseStructure.Prescaler = 0;				//不分频
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;		//CK_INT（定时器时钟）和 DTS（用于死区时间发生器和数字滤波器（ETR、TIx）的时钟）之间的分频比	
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;	//向上计数
    TIM_Base_Initialize(TIM2, &TIM_TimeBaseStructure);	
	
	TIM_Encoder_Interface_Set(TIM2, TIM_ENCODE_MODE_TI12, TIM_IC_POLARITY_RISING, TIM_IC_POLARITY_RISING);	//编码器模式

	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);			//输入捕获配置
    TIM_ICInitStructure.Channel     = TIM_CH_1;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0;
    TIM_Input_Channel_Initialize(TIM2, &TIM_ICInitStructure);
	
	TIM_Interrupt_Status_Clear(TIM2, TIM_INT_UPDATE);		//配置溢出中断标志位
	TIM_Interrupt_Enable(TIM2, TIM_INT_UPDATE);				//使能定时器2中断	
	
	TIM_Base_Count_Set(TIM2, 0);	//清零定时器计数值
	
	TIM_On(TIM2);	//开始定时器2
}

void Encoder_TIM3_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	TIM_TimeBaseInitType TIM_TimeBaseStructure;
	TIM_ICInitType TIM_ICInitStructure;
	
	GPTIMClockFrequency = Common_GPTIM_RCC_Initialize(TIM3, RCC_HCLK_DIV4);		//配置相关时钟

	GPIO_InitStructure.Pin            = EN_TIM3_GPIO_PIN;	//IO口
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;	//输入模式
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM3;		//TIM3复用PA6 PA7
	GPIO_Peripheral_Initialize(EN_TIM3_GPIO_PORT, &GPIO_InitStructure);	//配置GPIO
	
	//Common_TIM_NVIC_Initialize(TIM3_IRQn, ENABLE);	//需要中断再配置
	
	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);		//填入缺省值
    TIM_TimeBaseStructure.Period    = 65535;			//重装载值
    TIM_TimeBaseStructure.Prescaler = 0;				//不分频
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;		//CK_INT（定时器时钟）和 DTS（用于死区时间发生器和数字滤波器（ETR、TIx）的时钟）之间的分频比	
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;	//向上计数
    TIM_Base_Initialize(TIM3, &TIM_TimeBaseStructure);	
	
	TIM_Encoder_Interface_Set(TIM3, TIM_ENCODE_MODE_TI12, TIM_IC_POLARITY_RISING, TIM_IC_POLARITY_RISING);	//编码器模式

	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);			//输入捕获配置
    TIM_ICInitStructure.Channel     = TIM_CH_1;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0;
    TIM_Input_Channel_Initialize(TIM3, &TIM_ICInitStructure);
	
	TIM_Interrupt_Status_Clear(TIM3, TIM_INT_UPDATE);		//配置溢出中断标志位
	TIM_Interrupt_Enable(TIM3, TIM_INT_UPDATE);				//使能定时器2中断	
	
	TIM_Base_Count_Set(TIM3, 0);	//清零定时器计数值
	
	TIM_On(TIM3);	//开始定时器2	
}

int Read_Speed(int TIMx)
{
	int value_1;
	switch(TIMx)	//【*】为什么要强制转换short
	{
		case 2:value_1=(short)TIM_Base_Count_Get(TIM2); TIM_Base_Count_Set(TIM2,0); break;//IF是定时器2，1.采集编码器的计数值并保存。2.将定时器的计数值清零。
		case 3:value_1=(short)TIM_Base_Count_Get(TIM3); TIM_Base_Count_Set(TIM3,0); break;
		default:value_1=0;
	}
	return value_1;
}

void TIM2_IRQHandler(void)
{
	if(TIM_Interrupt_Status_Get(TIM2,TIM_INT_UPDATE) == SET)
	{
		TIM_Interrupt_Status_Clear(TIM2,TIM_INT_UPDATE);
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM_Interrupt_Status_Get(TIM3,TIM_INT_UPDATE) == SET)
	{
		TIM_Interrupt_Status_Clear(TIM3,TIM_INT_UPDATE);
	}
}




