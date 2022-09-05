#include "n32g430.h"
#include "encoder.h"
#include "GPTIM_common.h"

uint32_t GPTIMClockFrequency = 0;

void Encoder_TIM2_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	TIM_TimeBaseInitType TIM_TimeBaseStructure;
	TIM_ICInitType TIM_ICInitStructure;
	
	GPTIMClockFrequency = Common_GPTIM_RCC_Initialize(TIM2, RCC_HCLK_DIV4);		//�������ʱ��

	GPIO_InitStructure.Pin            = EN_TIM2_GPIO_PIN;	//IO��
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;	//����ģʽ
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM2;		//TIM2����PA0 PA1
	GPIO_Peripheral_Initialize(EN_TIM2_GPIO_PORT, &GPIO_InitStructure);	//����GPIO
	
	//Common_TIM_NVIC_Initialize(TIM2_IRQn, ENABLE);	//��Ҫ�ж�������
	
	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);		//����ȱʡֵ
    TIM_TimeBaseStructure.Period    = 65535;			//��װ��ֵ
    TIM_TimeBaseStructure.Prescaler = 0;				//����Ƶ
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;		//CK_INT����ʱ��ʱ�ӣ��� DTS����������ʱ�䷢�����������˲�����ETR��TIx����ʱ�ӣ�֮��ķ�Ƶ��	
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;	//���ϼ���
    TIM_Base_Initialize(TIM2, &TIM_TimeBaseStructure);	
	
	TIM_Encoder_Interface_Set(TIM2, TIM_ENCODE_MODE_TI12, TIM_IC_POLARITY_RISING, TIM_IC_POLARITY_RISING);	//������ģʽ

	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);			//���벶������
    TIM_ICInitStructure.Channel     = TIM_CH_1;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0;
    TIM_Input_Channel_Initialize(TIM2, &TIM_ICInitStructure);
	
	TIM_Interrupt_Status_Clear(TIM2, TIM_INT_UPDATE);		//��������жϱ�־λ
	TIM_Interrupt_Enable(TIM2, TIM_INT_UPDATE);				//ʹ�ܶ�ʱ��2�ж�	
	
	TIM_Base_Count_Set(TIM2, 0);	//���㶨ʱ������ֵ
	
	TIM_On(TIM2);	//��ʼ��ʱ��2
}

void Encoder_TIM3_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	TIM_TimeBaseInitType TIM_TimeBaseStructure;
	TIM_ICInitType TIM_ICInitStructure;
	
	GPTIMClockFrequency = Common_GPTIM_RCC_Initialize(TIM3, RCC_HCLK_DIV4);		//�������ʱ��

	GPIO_InitStructure.Pin            = EN_TIM3_GPIO_PIN;	//IO��
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;	//����ģʽ
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF3_TIM3;		//TIM3����PA6 PA7
	GPIO_Peripheral_Initialize(EN_TIM3_GPIO_PORT, &GPIO_InitStructure);	//����GPIO
	
	//Common_TIM_NVIC_Initialize(TIM3_IRQn, ENABLE);	//��Ҫ�ж�������
	
	TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);		//����ȱʡֵ
    TIM_TimeBaseStructure.Period    = 65535;			//��װ��ֵ
    TIM_TimeBaseStructure.Prescaler = 0;				//����Ƶ
    TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;		//CK_INT����ʱ��ʱ�ӣ��� DTS����������ʱ�䷢�����������˲�����ETR��TIx����ʱ�ӣ�֮��ķ�Ƶ��	
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;	//���ϼ���
    TIM_Base_Initialize(TIM3, &TIM_TimeBaseStructure);	
	
	TIM_Encoder_Interface_Set(TIM3, TIM_ENCODE_MODE_TI12, TIM_IC_POLARITY_RISING, TIM_IC_POLARITY_RISING);	//������ģʽ

	TIM_Input_Struct_Initialize(&TIM_ICInitStructure);			//���벶������
    TIM_ICInitStructure.Channel     = TIM_CH_1;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0;
    TIM_Input_Channel_Initialize(TIM3, &TIM_ICInitStructure);
	
	TIM_Interrupt_Status_Clear(TIM3, TIM_INT_UPDATE);		//��������жϱ�־λ
	TIM_Interrupt_Enable(TIM3, TIM_INT_UPDATE);				//ʹ�ܶ�ʱ��2�ж�	
	
	TIM_Base_Count_Set(TIM3, 0);	//���㶨ʱ������ֵ
	
	TIM_On(TIM3);	//��ʼ��ʱ��2	
}

int Read_Speed(int TIMx)
{
	int value_1;
	switch(TIMx)	//��*��ΪʲôҪǿ��ת��short
	{
		case 2:value_1=(short)TIM_Base_Count_Get(TIM2); TIM_Base_Count_Set(TIM2,0); break;//IF�Ƕ�ʱ��2��1.�ɼ��������ļ���ֵ�����档2.����ʱ���ļ���ֵ���㡣
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




