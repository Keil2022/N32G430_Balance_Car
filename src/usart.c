#include "usart.h"

uint8_t TxBuffer1[] = {0xaa,0x55};
uint8_t RxBuffer1[TxBufferSize2];

uint8_t NbrOfDataToRead             = TxBufferSize1;
uint32_t indexFlag                  = 0;

u8 send_buf[32];

void Usart_DMA_Init(void)
{
	USART_InitType USART_InitStructure;

    RCC_Configuration();				//������Ӧʱ��

    NVIC_Configuration();				//���ý����ж�

    GPIO_Configuration();				//����GPIO

    DMA_Configuration();				//����DMA

    /* USARTy and USARTz configuration */
    USART_InitStructure.BaudRate            = 500000;			//������
    USART_InitStructure.WordLength          = USART_WL_8B;		//���ݳ���
    USART_InitStructure.StopBits            = USART_STPB_1;		//1��ֹͣλ
    USART_InitStructure.Parity              = USART_PE_NO;		//��У����
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;	//�ر�RTS��CTS
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;	//����/����ģʽ
	
    USART_Initializes(USARTy, &USART_InitStructure);	//���ô���

    USART_DMA_Transfer_Enable(USARTy, USART_DMAREQ_RX | USART_DMAREQ_TX);	//����USARTy DMA Rx��TX����

    //USART_Interrput_Enable(USARTy, USART_INT_RXDNE);	/* ����USARTy�����ж� */
	USART_Flag_Clear(USARTy, USART_FLAG_RXDNE);			/* ����жϱ�־λ */

    DMA_Channel_Enable(USARTy_Tx_DMA_Channel);	/* ����USARTy TX DMAͨ�� */
	DMA_Channel_Enable(USARTy_Rx_DMA_Channel);	/* ����USARTy RX DMAͨ�� */

    USART_Enable(USARTy);				/*����USARTy */

//    /* Receive the TxBuffer2 */
//    while (indexFlag < TxBufferSize2)
//    {
//        while (USART_Flag_Status_Get(USARTy, USART_FLAG_RXDNE) == RESET)
//        {
//			
//        }
//        RxBuffer1[indexFlag++] = USART_Data_Receive(USARTy);
//    }

    /* �ȵ�USARTy TX DMAͨ��������� */
//    while (DMA_Flag_Status_Get(DMA,USARTy_Tx_DMA_FLAG) == RESET)
//    {		
//    }
		DMA_Flag_Status_Clear(DMA,USARTy_Tx_DMA_FLAG);
}


/* �������ʱ�� */
void RCC_Configuration(void)
{
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);		//����DMAʱ��
	
    RCC_AHB_Peripheral_Clock_Enable(USARTy_GPIO_CLK);			//����GPIOʱ��
	
	RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);		//��������ʱ��
	
    USARTy_APBxClkCmd(USARTy_CLK);			//��������ʱ��
}


/* ����GPIO */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_Structure_Initialize(&GPIO_InitStructure);	//����GPIOȱʡֵ
    
    /* Configure USARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTy_TxPin;    
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;			//��������
    GPIO_InitStructure.GPIO_Alternate = USARTy_Tx_GPIO_AF;			//GPIO����
    GPIO_Peripheral_Initialize(USARTy_GPIO, &GPIO_InitStructure);

    /* Configure USARTx Rx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTy_RxPin;
    GPIO_InitStructure.GPIO_Alternate = USARTy_Rx_GPIO_AF;			//GPIO����
    GPIO_Peripheral_Initialize(USARTy_GPIO, &GPIO_InitStructure);      
}


/* ���ý����ж� */
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Enable the USARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                 	= USART1_IRQn;			//�ж�������
    NVIC_InitStructure.NVIC_IRQChannelSubPriority		= NVIC_SUB_PRIORITY_1;	//�ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd              	= ENABLE;				//ʹ��
    NVIC_Initializes(&NVIC_InitStructure);
}

DMA_InitType DMA_Init_Structure;

/* DMA���� */
void DMA_Configuration(void)
{	
	/* USARTy_Tx_DMA_Channel(��USARTy Tx�¼�����)���� */
    DMA_Reset(USARTy_Tx_DMA_Channel);
    DMA_Init_Structure.PeriphAddr     = USARTy_DAT_Base;			//�������ַ
    DMA_Init_Structure.MemAddr        = (uint32_t)send_buf;			//�ڴ����ַ
    DMA_Init_Structure.Direction      = DMA_DIR_PERIPH_DST;			//DMA���ݴ��䷽�򣺴��ڴ浽��������ݴ���
    DMA_Init_Structure.BufSize        = TxBufferSize1;				//���ݴ�С
    DMA_Init_Structure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;	//ָ�������ַ�Ĵ����Ƿ������
    DMA_Init_Structure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;		//ָ���ڴ��ַ�Ĵ����Ƿ������
    DMA_Init_Structure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;		//ָ���������ݿ�ȡ�
    DMA_Init_Structure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;		//ָ���ڴ����ݿ�ȡ�
    DMA_Init_Structure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;		//ѭ��ģʽ-0������һ�ִ����ͨ��ֹͣ��1��ͨ������Ϊѭ��ģʽ��
																		//*ע�⣺�����ѡ���ͨ���������ڴ浽�ڴ�����ݴ��䣬����ʹ��ѭ��������ģʽ��
    DMA_Init_Structure.Priority       = DMA_CH_PRIORITY_HIGHEST;		//���ȼ�
    DMA_Init_Structure.Mem2Mem        = DMA_MEM2MEM_DISABLE;			//ָ��DMAy Channelx�Ƿ������ڴ浽�ڴ洫�䡣
    DMA_Initializes(USARTy_Tx_DMA_Channel, &DMA_Init_Structure);
    DMA_Channel_Request_Remap(USARTy_Tx_DMA_Channel,USARTy_Tx_DMA_REMAP);
	
	/* USARTy RX DMA1ͨ��(��USARTy RX�¼�����)���� */
    DMA_Reset(USARTy_Rx_DMA_Channel);
    DMA_Init_Structure.PeriphAddr = USARTy_DAT_Base;
    DMA_Init_Structure.MemAddr    = (uint32_t)RxBuffer1;
    DMA_Init_Structure.Direction  = DMA_DIR_PERIPH_SRC;		//DMA���ݴ��䷽�򣺴����赽�ڴ�����ݴ���
    DMA_Init_Structure.BufSize    = TxBufferSize2;
	DMA_Init_Structure.CircularMode   = DMA_CIRCULAR_MODE_ENABLE;		//ѭ��ģʽ-0������һ�ִ����ͨ��ֹͣ��1��ͨ������Ϊѭ��ģʽ��
    DMA_Initializes(USARTy_Rx_DMA_Channel, &DMA_Init_Structure);
    DMA_Channel_Request_Remap(USARTy_Rx_DMA_Channel, USARTy_Rx_DMA_REMAP);
}

void DMA_Restart(u8 len)
{
	DMA_Flag_Status_Clear(DMA,USARTy_Tx_DMA_FLAG);	/* ���DMA��־λ */
	
	DMA_Channel_Disable(USARTy_Tx_DMA_Channel);		/* �ر�USARTy TX DMAͨ�� */
	
	DMA_Current_Data_Transfer_Number_Set(USARTy_Tx_DMA_Channel, (u16)len);	/* ���¼������ݴ�С */
	
	DMA_Channel_Enable(USARTy_Tx_DMA_Channel);		/* ����USARTy TX DMAͨ�� */
}

void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 i;
	if(len>28)return;	//���28�ֽ�����
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];					//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];			//����У���
	//for(i=0;i<len+4;i++)Usart_SendByte(USART1,send_buf[i]);		//�������ݵ�����1
	DMA_Restart(len+4);		//����DMA����
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
	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
}

void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
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
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
}






