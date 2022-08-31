#include "n32g430.h"
#include "esp12f_io0.h"

//填补硬件上的空缺，硬件修改后可删除
void ESP12F_IO0_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);

	GPIO_Structure_Initialize(&GPIO_InitStructure);
	/* PA12 -- IO0控制脚：0——下载模式、1——运行模式 */
	GPIO_InitStructure.Pin            = GPIO_PIN_12;
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;	//电平翻转速度
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_OUT_PP;		//输入/输出模式
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP; 			//开上/下拉
	GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);		//初始化配置
	
	ESP12F_IO0_SET();
}

