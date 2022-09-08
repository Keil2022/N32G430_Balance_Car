#include "usart.h"

#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define TxBufferSize2 (countof(TxBuffer2) - 1)
#define countof(a) (sizeof(a) / sizeof(*(a)))	//�����ڴ��С/��һ�������ڴ��С = ���ݸ���

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

    RCC_Configuration();				//������Ӧʱ��

    NVIC_Configuration();				//���ý����ж�

    GPIO_Configuration();				//����GPIO

    DMA_Configuration();				//����DMA

    /* USARTy and USARTz configuration */
    USART_InitStructure.BaudRate            = 115200;			//������
    USART_InitStructure.WordLength          = USART_WL_8B;		//���ݳ���
    USART_InitStructure.StopBits            = USART_STPB_1;		//1��ֹͣλ
    USART_InitStructure.Parity              = USART_PE_NO;		//��У����
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;	//�ر�RTS��CTS
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;	//����/����ģʽ
	
    USART_Initializes(USARTy, &USART_InitStructure);	//���ô���

    USART_DMA_Transfer_Enable(USARTy, USART_DMAREQ_RX | USART_DMAREQ_TX);	//����USARTy DMA Rx��TX����

    USART_Interrput_Enable(USARTy, USART_INT_RXDNE);	//����USARTy�����ж�

	USART_Flag_Clear(USARTy, USART_FLAG_RXDNE);

    DMA_Channel_Enable(USARTy_Tx_DMA_Channel);	/*����USARTy TX DMAͨ��*/

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

/* DMA���� */
void DMA_Configuration(void)
{
    DMA_InitType DMA_Init_Structure;
	
	/* USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config */
    DMA_Reset(USARTy_Tx_DMA_Channel);
    DMA_Init_Structure.PeriphAddr     = USARTy_DAT_Base;			//�������ַ
    DMA_Init_Structure.MemAddr        = (uint32_t)TxBuffer1;		//�ڴ����ַ
    DMA_Init_Structure.Direction      = DMA_DIR_PERIPH_DST;			//DMA���ݴ��䷽�򣺴��ڴ浽��������ݴ���
    DMA_Init_Structure.BufSize        = TxBufferSize1;				//���ݴ�С
    DMA_Init_Structure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;	//ָ�������ַ�Ĵ����Ƿ������
    DMA_Init_Structure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;		//ָ���ڴ��ַ�Ĵ����Ƿ������
    DMA_Init_Structure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;	//ָ���������ݿ�ȡ�
    DMA_Init_Structure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;		//ָ���ڴ����ݿ�ȡ�
    DMA_Init_Structure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;	//ѭ��ģʽ-0������һ�ִ����ͨ��ֹͣ��1��ͨ������Ϊѭ��ģʽ��
																	//*ע�⣺�����ѡ���ͨ���������ڴ浽�ڴ�����ݴ��䣬����ʹ��ѭ��������ģʽ��
    DMA_Init_Structure.Priority       = DMA_CH_PRIORITY_HIGHEST;		//���ȼ�
    DMA_Init_Structure.Mem2Mem        = DMA_MEM2MEM_DISABLE;			//ָ��DMAy Channelx�Ƿ������ڴ浽�ڴ洫�䡣
    DMA_Initializes(USARTy_Tx_DMA_Channel, &DMA_Init_Structure);
    DMA_Channel_Request_Remap(USARTy_Tx_DMA_Channel,DMA_REMAP_USART1_TX);
}










