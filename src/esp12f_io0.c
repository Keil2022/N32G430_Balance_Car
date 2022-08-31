#include "n32g430.h"
#include "esp12f_io0.h"

//�Ӳ���ϵĿ�ȱ��Ӳ���޸ĺ��ɾ��
void ESP12F_IO0_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);

	GPIO_Structure_Initialize(&GPIO_InitStructure);
	/* PA12 -- IO0���ƽţ�0��������ģʽ��1��������ģʽ */
	GPIO_InitStructure.Pin            = GPIO_PIN_12;
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;	//��ƽ��ת�ٶ�
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_OUT_PP;		//����/���ģʽ
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP; 			//����/����
	GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);		//��ʼ������
	
	ESP12F_IO0_SET();
}

