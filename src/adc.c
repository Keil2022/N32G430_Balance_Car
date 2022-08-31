#include "n32g430.h"
#include "adc.h"

__IO u16 ADC_Value;

void ADC_RCC_Configuration(void)
{
	/* Enable peripheral clocks		使能外部时钟 ------------------------------------------------*/
    /* Enable DMA GPIO ADC clocks 	使能DMA、GPIO、ADC时钟*/
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA|RCC_AHB_PERIPH_GPIOB|RCC_AHB_PERIPH_ADC);
	
    /* RCC_ADCHCLK_DIV16	配置ADCHCLK预分频器*/
    ADC_Clock_Mode_Config(ADC_CKMOD_AHB, RCC_ADCHCLK_DIV16);
	
	/* selsect HSE as RCC ADC1M CLK Source 选择HSE作为RCC ADC1M CLK源 */
    RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8);
}

void ADC_GPIO_Configuration(void)
{
	/* GPIO配置 */
	GPIO_InitType GPIO_InitStructure;
	
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    /* Configure adc input as analog input 将adc输入配置为模拟输入 */
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
	
	ADC_RCC_Configuration();	//时钟配置
	ADC_GPIO_Configuration();	//GPIO配置
	
	/* DMA通道1配置 */
	DMA_Reset(DMA_CH1);		//重置通道1寄存器
	DMA_InitStructure.PeriphAddr     = (uint32_t)&ADC->DAT;		//外围基地址
	DMA_InitStructure.MemAddr        = (uint32_t)&ADC_Value;	//内存基地址
	DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;		//指定外设是源还是目标。
	DMA_InitStructure.BufSize        = 1;						//指定缓存区大小
	DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;		//指定外设地址寄存器是否递增。
    DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;			//指定内存地址寄存器是否递增。
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_HALFWORD;	//指定外设数据宽度。
    DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_HALFWORD;		//指定内存数据宽度。
    DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_ENABLE;		//DMAy Channelx的操作模式。**)注意:如果在选择的通道上配置内存到内存的数据传输，则不能使用循环缓冲区模式。* /
    DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGH;			//指定DMAy Channelx的软件优先级。
    DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;				//指定是否在内存到内存传输中使用DMAy Channelx。
	DMA_Initializes(DMA_CH1, &DMA_InitStructure);				//加载DMA配置
	DMA_Channel_Request_Remap(DMA_CH1, DMA_REMAP_ADC);			//设置DMA Channelx的重映射请求。
	
	DMA_Channel_Enable(DMA_CH1);	//使能DMA通道
	
	ADC_Initializes_Structure(&ADC_InitStructure);			//初始化ADC结构体成员
    ADC_InitStructure.MultiChEn      = ENABLE;				//指定转换是在Scan(多通道)模式还是Single(单通道)模式下执行
    ADC_InitStructure.ContinueConvEn = ENABLE;				//指定转换是在连续模式还是单模式中执行
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;						//指定ADC数据对齐是左对齐还是右对齐
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH;	//定义用于启动常规通道模拟到数字转换的外部触发器
    ADC_InitStructure.ChsNumber      = ADC_REGULAR_LEN_1;					//指定将使用定序器为常规通道组转换的ADC通道数
    ADC_Initializes(&ADC_InitStructure);					//加载ADC配置
	
	/* ADC通道采样时间配置 */
	ADC_Channel_Sample_Time_Config(ADC_Channel_10_PB1, ADC_SAMP_TIME_55CYCLES5);
	
	/* ADC常规通道配置 */
    ADC_Regular_Sequence_Conversion_Number_Config(ADC_Channel_10_PB1, ADC_REGULAR_NUMBER_1);
	
	/* 使能ADC DMA */
	ADC_DMA_Transfer_Enable();
	
	/* 使能 ADC */
    ADC_ON();
	
	/* 检查ADC是否准备好 */
    while(ADC_Flag_Status_Get(ADC_RD_FLAG, ADC_FLAG_JENDCA, ADC_FLAG_RDY) == RESET);
	
	/* 开始ADC1校准 */
	ADC_Calibration_Operation(ADC_CALIBRATION_ENABLE);
	
	/* 检查ADC1校准结束 */
    while (ADC_Calibration_Operation(ADC_CALIBRATION_STS))
		
	/* 启动ADC软件转换 */
    ADC_Regular_Channels_Software_Conversion_Operation(ADC_EXTRTRIG_SWSTRRCH_ENABLE);
}








