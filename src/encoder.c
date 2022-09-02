#include "n32g430.h"
#include "encoder.h"

void Encoder_TIM2_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	TIM_TimeBaseInitType TIM_TimeBaseStructure;
	TIM_ICInitType TIM_ICInitStructure;
	
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);
	
	//RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);		//��������ʱ��
	RCC_Pclk1_Config(RCC_HCLK_DIV4);							//���õ���APBʱ��(PCLK1)��
	RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM2);		//������ʱ��2ʱ��

	GPIO_InitStructure.Pin            = EN_TIM2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;
	GPIO_Peripheral_Initialize(EN_TIM2_GPIO_PORT, &GPIO_InitStructure);
	
	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);		//����ȱʡֵ
    TIM_TimeBaseStructure.Period    = 65535;
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

    TIM_Base_Initialize(TIM2, &TIM_TimeBaseStructure);  
	
	TIM_Encoder_Interface_Set(TIM2, TIM_ENCODE_MODE_TI12, TIM_IC_POLARITY_RISING, TIM_IC_POLARITY_RISING);

	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);
	TIM_ICInitStructure.IcFilter = 10;
	TIM_Input_Channel_Initialize(TIM2, &TIM_ICInitStructure);
	
	TIM_Interrupt_Status_Clear(TIM2, TIM_INT_UPDATE);	//������������жϱ�־λ
	
	TIM_Base_Count_Set(TIM2, 0);	//���㶨ʱ������ֵ
	
	TIM_On(TIM2);
}

void Encoder_TIM3_Init(void)
{
	
}

int Read_Speed(int TIMx)
{
	int value_1;
	switch(TIMx)	//��*��ΪʲôҪǿ��ת��short
	{
		case 2:value_1=(short)TIM_Base_Count_Get(TIM2); TIM_Base_Count_Set(TIM2,0); break;//IF�Ƕ�ʱ��2��1.�ɼ��������ļ���ֵ�����档2.����ʱ���ļ���ֵ���㡣
		case 4:value_1=(short)TIM_Base_Count_Get(TIM4); TIM_Base_Count_Set(TIM4,0); break;
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




