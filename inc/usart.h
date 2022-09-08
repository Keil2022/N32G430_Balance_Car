#ifndef __USART_H
#define __USART_H

#include "main.h"

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

void Usart_DMA_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void DMA_Configuration(void);













#endif

