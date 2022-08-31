#include "n32g430.h"
#include "led.h"

//填补硬件上的空缺，硬件修改后可删除
void LED_GPIO_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOC);

	GPIO_Structure_Initialize(&GPIO_InitStructure);
	/* PA5_ */
	GPIO_InitStructure.Pin            = LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;	//电平翻转速度
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_OUT_PP;		//输入/输出模式
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP; 			//开上/下拉
	GPIO_Peripheral_Initialize(LED_GPIO_PORT, &GPIO_InitStructure);		//初始化配置
	
	LED_Off;
}


