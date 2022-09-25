#include "ultr.h"

void ULTR_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB);

	GPIO_Structure_Initialize(&GPIO_InitStructure);		//��ʼ��Ĭ��ֵ
	/*PB6 -- KEY*/
	GPIO_InitStructure.Pin            = ULTR_GPIO_PIN;			//�˿ں�
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST; 	//ת�����ʸ�/��
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;		//����/���ģʽ
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_DOWN; 			//��/����
	GPIO_Peripheral_Initialize(ULTR_GPIO_PORT, &GPIO_InitStructure);		//��������
}


