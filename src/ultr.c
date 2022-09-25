#include "ultr.h"

void ULTR_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB);

	GPIO_Structure_Initialize(&GPIO_InitStructure);		//初始化默认值
	/*PB6 -- KEY*/
	GPIO_InitStructure.Pin            = ULTR_GPIO_PIN;			//端口号
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST; 	//转换速率高/低
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;		//输入/输出模式
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_DOWN; 			//上/下拉
	GPIO_Peripheral_Initialize(ULTR_GPIO_PORT, &GPIO_InitStructure);		//加载配置
}


