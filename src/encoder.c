#include "n32g430.h"
#include "encoder.h"
#include "GPTIM_common.h"

uint32_t GPTIMClockFrequency = 0;

void Encoder_TIM2_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	TIM_TimeBaseInitType TIM_TimeBaseStructure;
	TIM_ICInitType TIM_ICInitStructure;
	
	GPTIMClockFrequency = Common_GPTIM_RCC_Initialize(TIM2, RCC_HCLK_DIV4);

	GPIO_InitStructure.Pin            = EN_TIM2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
	GPIO_Peripheral_Initialize(EN_TIM2_GPIO_PORT, &GPIO_InitStructure);
	
	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);		//填入缺省值
    TIM_TimeBaseStructure.Period    = 65535;
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_Base_Initialize(TIM2, &TIM_TimeBaseStructure);
	
	//TIM_Base_Auto_Reload_Set(TIM2, 65535);
	
	TIM_Hall_Sensor_Disable(TIM2);
	TIM_Encoder_Interface_Set(TIM2, TIM_ENCODE_MODE_TI12, TIM_IC_POLARITY_RISING, TIM_IC_POLARITY_RISING);

	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
	TIM_ICInitStructure.IcFilter = 10;
	TIM_Input_Channel_Initialize(TIM2, &TIM_ICInitStructure);
	
	TIM_Interrupt_Status_Clear(TIM2, TIM_INT_UPDATE);	//配置溢出更新中断标志位
	//Common_TIM_NVIC_Initialize(TIM2_IRQn, ENABLE);
	TIM_Interrupt_Enable(TIM2, TIM_INT_UPDATE);
	
	TIM_Base_Count_Set(TIM2, 0);	//清零定时器计数值
	
	TIM_On(TIM2);
}

void Encoder_TIM3_Init(void)
{
	
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
	if(TIM_Interrupt_Status_Get(TIM2,TIM_INT_UPDATE)!=0)
	{
		TIM_Interrupt_Status_Clear(TIM2,TIM_INT_UPDATE);
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM_Interrupt_Status_Get(TIM3,TIM_INT_UPDATE)!=0)
	{
		TIM_Interrupt_Status_Clear(TIM3,TIM_INT_UPDATE);
	}
}




