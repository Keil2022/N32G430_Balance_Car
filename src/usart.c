#include "usart.h"

uint8_t TxBuffer1[] = {0xaa,0x55};
uint8_t RxBuffer1[TxBufferSize2];

uint8_t NbrOfDataToRead             = TxBufferSize1;
uint32_t indexFlag                  = 0;

u8 send_buf[32];

void Usart_DMA_Init(void)
{
	USART_InitType USART_InitStructure;

    RCC_Configuration();				//配置相应时钟

    NVIC_Configuration();				//配置接收中断

    GPIO_Configuration();				//配置GPIO

    DMA_Configuration();				//配置DMA

    /* USARTy and USARTz configuration */
    USART_InitStructure.BaudRate            = 500000;			//波特率
    USART_InitStructure.WordLength          = USART_WL_8B;		//数据长度
    USART_InitStructure.StopBits            = USART_STPB_1;		//1个停止位
    USART_InitStructure.Parity              = USART_PE_NO;		//无校验码
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;	//关闭RTS和CTS
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;	//接收/发送模式
	
    USART_Initializes(USARTy, &USART_InitStructure);	//配置串口

    USART_DMA_Transfer_Enable(USARTy, USART_DMAREQ_RX | USART_DMAREQ_TX);	//启用USARTy DMA Rx和TX请求

    //USART_Interrput_Enable(USARTy, USART_INT_RXDNE);	/* 启用USARTy接收中断 */
	USART_Flag_Clear(USARTy, USART_FLAG_RXDNE);			/* 清除中断标志位 */

    DMA_Channel_Enable(USARTy_Tx_DMA_Channel);	/* 启用USARTy TX DMA通道 */
	DMA_Channel_Enable(USARTy_Rx_DMA_Channel);	/* 启用USARTy RX DMA通道 */

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
//    while (DMA_Flag_Status_Get(DMA,USARTy_Tx_DMA_FLAG) == RESET)
//    {		
//    }
		DMA_Flag_Status_Clear(DMA,USARTy_Tx_DMA_FLAG);
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

DMA_InitType DMA_Init_Structure;

/* DMA配置 */
void DMA_Configuration(void)
{	
	/* USARTy_Tx_DMA_Channel(由USARTy Tx事件触发)配置 */
    DMA_Reset(USARTy_Tx_DMA_Channel);
    DMA_Init_Structure.PeriphAddr     = USARTy_DAT_Base;			//外设基地址
    DMA_Init_Structure.MemAddr        = (uint32_t)send_buf;			//内存基地址
    DMA_Init_Structure.Direction      = DMA_DIR_PERIPH_DST;			//DMA数据传输方向：从内存到外设的数据传输
    DMA_Init_Structure.BufSize        = TxBufferSize1;				//数据大小
    DMA_Init_Structure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;	//指定外设地址寄存器是否递增。
    DMA_Init_Structure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;		//指定内存地址寄存器是否递增。
    DMA_Init_Structure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;		//指定外设数据宽度。
    DMA_Init_Structure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;		//指定内存数据宽度。
    DMA_Init_Structure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;		//循环模式-0：经过一轮传输后通道停止。1：通道配置为循环模式。
																		//*注意：如果在选择的通道上配置内存到内存的数据传输，则不能使用循环缓冲区模式。
    DMA_Init_Structure.Priority       = DMA_CH_PRIORITY_HIGHEST;		//优先级
    DMA_Init_Structure.Mem2Mem        = DMA_MEM2MEM_DISABLE;			//指定DMAy Channelx是否用于内存到内存传输。
    DMA_Initializes(USARTy_Tx_DMA_Channel, &DMA_Init_Structure);
    DMA_Channel_Request_Remap(USARTy_Tx_DMA_Channel,USARTy_Tx_DMA_REMAP);
	
	/* USARTy RX DMA1通道(由USARTy RX事件触发)配置 */
    DMA_Reset(USARTy_Rx_DMA_Channel);
    DMA_Init_Structure.PeriphAddr = USARTy_DAT_Base;
    DMA_Init_Structure.MemAddr    = (uint32_t)RxBuffer1;
    DMA_Init_Structure.Direction  = DMA_DIR_PERIPH_SRC;		//DMA数据传输方向：从外设到内存的数据传输
    DMA_Init_Structure.BufSize    = TxBufferSize2;
	DMA_Init_Structure.CircularMode   = DMA_CIRCULAR_MODE_ENABLE;		//循环模式-0：经过一轮传输后通道停止。1：通道配置为循环模式。
    DMA_Initializes(USARTy_Rx_DMA_Channel, &DMA_Init_Structure);
    DMA_Channel_Request_Remap(USARTy_Rx_DMA_Channel, USARTy_Rx_DMA_REMAP);
}

void DMA_Restart(u8 len)
{
	DMA_Flag_Status_Clear(DMA,USARTy_Tx_DMA_FLAG);	/* 清除DMA标志位 */
	
	DMA_Channel_Disable(USARTy_Tx_DMA_Channel);		/* 关闭USARTy TX DMA通道 */
	
	DMA_Current_Data_Transfer_Number_Set(USARTy_Tx_DMA_Channel, (u16)len);	/* 重新加载数据大小 */
	
	DMA_Channel_Enable(USARTy_Tx_DMA_Channel);		/* 启用USARTy TX DMA通道 */
}

void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 i;
	if(len>28)return;	//最多28字节数据
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];					//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];			//计算校验和
	//for(i=0;i<len+4;i++)Usart_SendByte(USART1,send_buf[i]);		//发送数据到串口1
	DMA_Restart(len+4);		//启动DMA传输
}

void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}

void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
}






