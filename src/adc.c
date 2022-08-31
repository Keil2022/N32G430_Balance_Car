#include "n32g430.h"
#include "adc.h"

__IO u16 ADC_Value;

void ADC_RCC_Configuration(void)
{
	/* Enable peripheral clocks		ʹ���ⲿʱ�� ------------------------------------------------*/
    /* Enable DMA GPIO ADC clocks 	ʹ��DMA��GPIO��ADCʱ��*/
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA|RCC_AHB_PERIPH_GPIOB|RCC_AHB_PERIPH_ADC);
	
    /* RCC_ADCHCLK_DIV16	����ADCHCLKԤ��Ƶ��*/
    ADC_Clock_Mode_Config(ADC_CKMOD_AHB, RCC_ADCHCLK_DIV16);
	
	/* selsect HSE as RCC ADC1M CLK Source ѡ��HSE��ΪRCC ADC1M CLKԴ */
    RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8);
}

void ADC_GPIO_Configuration(void)
{
	/* GPIO���� */
	GPIO_InitType GPIO_InitStructure;
	
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* Configure adc input as analog input ��adc��������Ϊģ������ */
//    GPIO_InitStructure.Pin       = GPIO_PIN_7 |GPIO_PIN_6 |GPIO_PIN_5 |GPIO_PIN_4 |GPIO_PIN_3 |GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
//    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin       = ADC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_Peripheral_Initialize(ADC_GPIO_PORT, &GPIO_InitStructure);
}

void ADC_Configuration(void)
{
    ADC_InitType ADC_InitStructure;
	DMA_InitType DMA_InitStructure;
	
	ADC_RCC_Configuration();	//ʱ������
	ADC_GPIO_Configuration();	//GPIO����
	
	/* DMAͨ��1���� */
	DMA_Reset(DMA_CH1);		//����ͨ��1�Ĵ���
	DMA_InitStructure.PeriphAddr     = (uint32_t)&ADC->DAT;		//��Χ����ַ
	DMA_InitStructure.MemAddr        = (uint32_t)&ADC_Value;	//�ڴ����ַ
	DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;		//ָ��������Դ����Ŀ�ꡣ
	DMA_InitStructure.BufSize        = 1;						//ָ����������С
	DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;		//ָ�������ַ�Ĵ����Ƿ������
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;			//ָ���ڴ��ַ�Ĵ����Ƿ������
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_HALFWORD;	//ָ���������ݿ�ȡ�
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_HALFWORD;		//ָ���ڴ����ݿ�ȡ�
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_ENABLE;		//DMAy Channelx�Ĳ���ģʽ��**)ע��:�����ѡ���ͨ���������ڴ浽�ڴ�����ݴ��䣬����ʹ��ѭ��������ģʽ��* /
    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;			//ָ��DMAy Channelx��������ȼ���
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;				//ָ���Ƿ����ڴ浽�ڴ洫����ʹ��DMAy Channelx��
	DMA_Initializes(DMA_CH1, &DMA_InitStructure);				//����DMA����
	DMA_Channel_Request_Remap(DMA_CH1, DMA_REMAP_ADC);			//����DMA Channelx����ӳ������
	
	DMA_Channel_Enable(DMA_CH1);	//ʹ��DMAͨ��
	
	ADC_Initializes_Structure(&ADC_InitStructure);			//��ʼ��ADC�ṹ���Ա
    ADC_InitStructure.MultiChEn      = ENABLE;				//ָ��ת������Scan(��ͨ��)ģʽ����Single(��ͨ��)ģʽ��ִ��
    ADC_InitStructure.ContinueConvEn = ENABLE;				//ָ��ת����������ģʽ���ǵ�ģʽ��ִ��
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;						//ָ��ADC���ݶ���������뻹���Ҷ���
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH;	//����������������ͨ��ģ�⵽����ת�����ⲿ������
    ADC_InitStructure.ChsNumber      = ADC_REGULAR_LEN_1;					//ָ����ʹ�ö�����Ϊ����ͨ����ת����ADCͨ����
    ADC_Initializes(&ADC_InitStructure);					//����ADC����
	
	/* ADCͨ������ʱ������ */
	ADC_Channel_Sample_Time_Config(ADC_Channel_10_PB1, ADC_SAMP_TIME_55CYCLES5);
	
	/* ADC����ͨ������ */
    ADC_Regular_Sequence_Conversion_Number_Config(ADC_Channel_10_PB1, ADC_REGULAR_NUMBER_1);
	
	/* ʹ��ADC DMA */
	ADC_DMA_Transfer_Enable();
	
	/* ʹ�� ADC */
    ADC_ON();
	
	/* ���ADC�Ƿ�׼���� */
    while(ADC_Flag_Status_Get(ADC_RD_FLAG, ADC_FLAG_JENDCA, ADC_FLAG_RDY) == RESET);
	
	/* ��ʼADC1У׼ */
	ADC_Calibration_Operation(ADC_CALIBRATION_ENABLE);
	
	/* ���ADC1У׼���� */
    while (ADC_Calibration_Operation(ADC_CALIBRATION_STS))
		
	/* ����ADC���ת�� */
    ADC_Regular_Channels_Software_Conversion_Operation(ADC_EXTRTRIG_SWSTRRCH_ENABLE);
}








