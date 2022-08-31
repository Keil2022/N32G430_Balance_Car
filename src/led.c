#include "n32g430.h"
#include "led.h"

//�Ӳ���ϵĿ�ȱ��Ӳ���޸ĺ��ɾ��
void LED_GPIO_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOC);

	GPIO_Structure_Initialize(&GPIO_InitStructure);
	/* PA5_ */
	GPIO_InitStructure.Pin            = LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;	//��ƽ��ת�ٶ�
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_OUT_PP;		//����/���ģʽ
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP; 			//����/����
	GPIO_Peripheral_Initialize(LED_GPIO_PORT, &GPIO_InitStructure);		//��ʼ������
	
	LED_Off;
}


