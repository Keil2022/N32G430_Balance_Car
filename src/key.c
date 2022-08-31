#include "n32g430.h"
#include "key.h"

u8 	KEY_Phase = 0;			//�����׶�
u16 KEY_TimeCount = 0;		//����ʱ��

u8 KEY_ShortOK;			//�̰���Ч
u8 KEY_LongOK;			//������Ч
u8 KEY_EndOK;			//��������

void Key_GPIO_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB);

	GPIO_Structure_Initialize(&GPIO_InitStructure);		//��ʼ��Ĭ��ֵ
	/*PB6 -- KEY*/
	GPIO_InitStructure.Pin            = KEY_GPIO_PIN;				//�˿ں�
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST; 	//ת�����ʸ�/��
	GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_INPUT;		//����/���ģʽ
	GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP; 			//��/����
	GPIO_Peripheral_Initialize(KEY_GPIO_PORT, &GPIO_InitStructure);		//��������
}

void Key_Scanf(void)
{
	switch(KEY_Phase)
    {
		case 0:
					//����1
					if(KEY == KEY_Press)
					{
						KEY_TimeCount++;
						if(KEY_TimeCount >=  KEY_LongTime)
						{
							KEY_Phase = 1;			//ע�͵�Ϊ ����ת��������
							//KEY_LongOK = True;	//��ʱ����Ч
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
                        KEY_LongOK = True;		//�ɿ�����Ч
					}
		break;
        
        
        case 2:
					if(KEY == KEY_Lift)		//�ȴ�����
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



