#include "n32g430.h"
#include "key.h"

u8 	KEY_Phase = 0;			//按键阶段
u16 KEY_TimeCount = 0;		//按键时间

u8 KEY_ShortOK;			//短按有效
u8 KEY_LongOK;			//长按有效
u8 KEY_EndOK;			//结束按键

void Key_GPIO_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB);

	GPIO_Structure_Initialize(&GPIO_InitStructure);		//初始化默认值
	/*PB6 -- KEY*/
	GPIO_InitStructure.Pin            = KEY_GPIO_PIN;				//端口号
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST; 	//转换速率高/低
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;		//输入/输出模式
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP; 			//上/下拉
	GPIO_Peripheral_Initialize(KEY_GPIO_PORT, &GPIO_InitStructure);		//加载配置
}

void Key_Scanf(void)
{
	switch(KEY_Phase)
    {
		case 0:
					//按键1
					if(KEY == KEY_Press)
					{
						KEY_TimeCount++;
						if(KEY_TimeCount >=  KEY_LongTime)
						{
							KEY_Phase = 1;			//注释掉为 不跳转长按功能
							//KEY_LongOK = True;	//超时即生效
						}
					}
					else
					{
						if(KEY_TimeCount >= KEY_ShortTime)		KEY_ShortOK = True;
						KEY_TimeCount = 0;
					}
		break;
        
        
        case 1:
					if(KEY == KEY_Press)
					{
						KEY_TimeCount++;
						if(KEY_TimeCount >= KEY_EndTime)
						{
							KEY_Phase = 2;
						}
					}
					else
					{
						KEY_Phase = 0;
						KEY_TimeCount = 0;
                        KEY_LongOK = True;		//松开后生效
					}
		break;
        
        
        case 2:
					if(KEY == KEY_Lift)		//等待松手
					{
						KEY_Phase = 0;
						KEY_TimeCount = 0;
                        KEY_LongOK = True;
					}
		break;
        

        default:
					KEY_Phase = 0;
					KEY_TimeCount = 0;
		break;
	}
}

void KEY_Handle(void)
{
	if(KEY_ShortOK == True)
	{
		KEY_ShortOK = Fault;
		
		
	}
	
	if(KEY_LongOK == True)
	{
		KEY_LongOK = Fault;
		
		
	}
}	



