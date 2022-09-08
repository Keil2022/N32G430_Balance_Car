#include "usart.h"

#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define TxBufferSize2 (countof(TxBuffer2) - 1)
#define countof(a) (sizeof(a) / sizeof(*(a)))	//数组内存大小/第一个数据内存大小 = 数据个数

uint8_t TxBuffer1[] = "USART DMA Interrupt: USARTy -> USARTz using DMA Tx and Rx Flag";
uint8_t TxBuffer2[] = "USART DMA Interrupt: USARTz -> USARTy using DMA Tx and Rx Interrupt";
uint8_t RxBuffer1[TxBufferSize2];
uint8_t RxBuffer2[TxBufferSize1];
uint8_t NbrOfDataToRead             = TxBufferSize1;
uint8_t NbrOfDataToRead2             = TxBufferSize2;
uint32_t indexFlag                  = 0;

void Usart_DMA_Init(void)
{
	USART_InitType USART_InitStructure;

    RCC_Configuration();				//配置相应时钟

    NVIC_Configuration();				//配置接收中断

    GPIO_Configuration();				//配置GPIO

    DMA_Configuration();				//配置DMA

    /* USARTy and USARTz configuration */
    USART_InitStructure.BaudRate            = 115200;			//波特率
    USART_InitStructure.WordLength          = USART_WL_8B;		//数据长度
    USART_InitStructure.StopBits            = USART_STPB_1;		//1个停止位
    USART_InitStructure.Parity              = USART_PE_NO;		//无校验码
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;	//关闭RTS和CTS
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;	//接收/发送模式
	
    USART_Initializes(USARTy, &USART_InitStructure);	//配置串口

    USART_DMA_Transfer_Enable(USARTy, USART_DMAREQ_RX | USART_DMAREQ_TX);	//启用USARTy DMA Rx和TX请求

    USART_Interrput_Enable(USARTy, USART_INT_RXDNE);	//启用USARTy接收中断

	USART_Flag_Clear(USARTy, USART_FLAG_RXDNE);

    DMA_Channel_Enable(USARTy_Tx_DMA_Channel);	/*启用USARTy TX DMA通道*/

    USART_Enable(USARTy);				/*启用USARTy */

//    /* Receive the TxBuffer2 */
//    while (indexFlag < TxBufferSize2)
//    {
//        while (USART_Flag_Status_Get(USARTy, USART_FLAG_RXDNE) == RESET)
//        {
//			
//        }
//        RxBuffer1[indexFlag++] = USART_Data_Receive(USARTy);
//    }

    /* 等到USARTy TX DMA通道传输完成 */
    while (DMA_Flag_Status_Get(DMA,USARTy_Tx_DMA_FLAG) == RESET)
    {
//*\*\          - DMA_CH2_GLBF
//*\*\          - DMA_CH2_TXCF
//*\*\          - DMA_CH2_HTXF
//*\*\          - DMA_CH2_ERRF
		DMA_Flag_Status_Clear(DMA, DMA_CH2_GLBF);
		DMA_Flag_Status_Clear(DMA, DMA_CH2_TXCF);
		DMA_Flag_Status_Clear(DMA, DMA_CH2_HTXF);
		
    }
}


/* 配置相关时钟 */
void RCC_Configuration(void)
{
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);		//开启DMA时钟
	
    RCC_AHB_Peripheral_Clock_Enable(USARTy_GPIO_CLK);			//开启GPIO时钟
	
	RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);		//开启复用时钟
	
    USARTy_APBxClkCmd(USARTy_CLK);			//开启串口时钟
}


/* 配置GPIO */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);	//配置GPIO缺省值
    
    /* Configure USARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTy_TxPin;    
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;			//复用推挽
    GPIO_InitStructure.GPIO_Alternate = USARTy_Tx_GPIO_AF;			//GPIO复用
    GPIO_Peripheral_Initialize(USARTy_GPIO, &GPIO_InitStructure);

    /* Configure USARTx Rx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTy_RxPin;
    GPIO_InitStructure.GPIO_Alternate = USARTy_Rx_GPIO_AF;			//GPIO复用
    GPIO_Peripheral_Initialize(USARTy_GPIO, &GPIO_InitStructure);      
}


/* 配置接收中断 */
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Enable the USARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                 	= USART1_IRQn;			//中断向量号
    NVIC_InitStructure.NVIC_IRQChannelSubPriority		= NVIC_SUB_PRIORITY_1;	//中断优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd              	= ENABLE;				//使能
    NVIC_Initializes(&NVIC_InitStructure);
}

/* DMA配置 */
void DMA_Configuration(void)
{
    DMA_InitType DMA_Init_Structure;
	
	/* USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config */
    DMA_Reset(USARTy_Tx_DMA_Channel);
    DMA_Init_Structure.PeriphAddr     = USARTy_DAT_Base;			//外设基地址
    DMA_Init_Structure.MemAddr        = (uint32_t)TxBuffer1;		//内存基地址
    DMA_Init_Structure.Direction      = DMA_DIR_PERIPH_DST;			//DMA数据传输方向：从内存到外设的数据传输
    DMA_Init_Structure.BufSize        = TxBufferSize1;				//数据大小
    DMA_Init_Structure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;	//指定外设地址寄存器是否递增。
    DMA_Init_Structure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;		//指定内存地址寄存器是否递增。
    DMA_Init_Structure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;	//指定外设数据宽度。
    DMA_Init_Structure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;		//指定内存数据宽度。
    DMA_Init_Structure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;	//循环模式-0：经过一轮传输后通道停止。1：通道配置为循环模式。
																	//*注意：如果在选择的通道上配置内存到内存的数据传输，则不能使用循环缓冲区模式。
    DMA_Init_Structure.Priority       = DMA_CH_PRIORITY_HIGHEST;		//优先级
    DMA_Init_Structure.Mem2Mem        = DMA_MEM2MEM_DISABLE;			//指定DMAy Channelx是否用于内存到内存传输。
    DMA_Initializes(USARTy_Tx_DMA_Channel, &DMA_Init_Structure);
    DMA_Channel_Request_Remap(USARTy_Tx_DMA_Channel,DMA_REMAP_USART1_TX);
}










