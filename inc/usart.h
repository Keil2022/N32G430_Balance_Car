#ifndef __USART_H
#define __USART_H

#include "main.h"

#define countof(a) (sizeof(a) / sizeof(*(a)))	//数组内存大小/第一个数据内存大小 = 数据个数
	
#define TxBufferSize1 ( countof(TxBuffer1) )	//发送数据大小
#define TxBufferSize2 4							//接收数据大小

#define USARTy                  USART1
#define USARTy_CLK              RCC_APB2_PERIPH_USART1
#define USARTy_GPIO             GPIOA
#define USARTy_GPIO_CLK         RCC_AHB_PERIPH_GPIOA
#define USARTy_RxPin            GPIO_PIN_10
#define USARTy_TxPin            GPIO_PIN_9
#define USARTy_Rx_GPIO_AF       GPIO_AF5_USART1
#define USARTy_Tx_GPIO_AF       GPIO_AF5_USART1
#define USARTy_APBxClkCmd       RCC_APB2_Peripheral_Clock_Enable
#define USARTy_DAT_Base         (USART1_BASE + 0x04)

#define USARTy_Tx_DMA_Channel   DMA_CH2
#define USARTy_Tx_DMA_FLAG      DMA_CH2_TXCF
#define USARTy_Tx_DMA_REMAP     DMA_REMAP_USART1_TX

#define USARTy_Rx_DMA_Channel	DMA_CH3
#define USARTy_Rx_DMA_FLAG      DMA_CH3_TXCF
#define USARTy_Rx_DMA_REMAP     DMA_REMAP_USART1_RX

extern uint8_t TxBuffer1[];
extern uint8_t RxBuffer1[TxBufferSize2];

extern uint8_t NbrOfDataToRead;
extern uint32_t indexFlag;

void Usart_DMA_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void DMA_Configuration(void);
void DMA_Restart(u8 len);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);










#endif

