/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
*\*\file main.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
**/

#include "main.h"

RCC_ClocksType RCC_ClockFreq;

extern uint16_t Channel1Pulse;
extern int Encoder_Left,Encoder_Right;	//编码器数据（速度）
extern uint8_t NbrOfDataToRead;

extern float Pitch,Roll,Yaw;			//角度
extern short gyrox,gyroy,gyroz;			//陀螺仪--角速度
extern short aacx,aacy,aacz;			//加速度

extern uint8_t RxBuffer1[TxBufferSize2];	//数据接收缓存

extern float 
	Vertical_Kp,	//直立环KP、KD
	Vertical_Kd;
extern float 
	Velocity_Kp,	//速度环KP、KI
	Velocity_Ki;

extern float Target_Speed;	//期望速度

int Speed_Coeff; //速度系数

/* 主函数 */
int main(void)
{
	RCC_Clocks_Frequencies_Value_Get(&RCC_ClockFreq);
	
	//ESP12F_IO0_Init();	//硬件已做修改
	ADC_Configuration();
	
	LED_GPIO_Init();
	Key_GPIO_Init();
	
	TIMx_PWM_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM3_Init();
	
	//log_init();
	
	MPU_Init();								//初始化MPU6050
	while( mpu_dmp_init() )	{;}				//等待MPU初始化完成
	LED_On;
	
	EXTI_Init(KEY_INPUT_PORT, KEY_INPUT_PIN);
	
	Usart_DMA_Init();
	
	ULTR_Init();	
	
	while(1)
	{
		//Load(Channel1Pulse, 0);
		
		if(DMA_Flag_Status_Get(DMA,USARTy_Rx_DMA_FLAG) != RESET)
		{
			DMA_Flag_Status_Clear(DMA, USARTy_Rx_DMA_FLAG);
			
			if(RxBuffer1[0] == 0x5a)
			{
//				Vertical_Kp = -(float)( RxBuffer1[1]<<8 | RxBuffer1[2] );
//				Vertical_Kd = -(float)(RxBuffer1[3]);
//				Velocity_Kp = -( (float)(RxBuffer1[4]) )/10;
//				Velocity_Ki = Velocity_Kp/200;
				
				Vertical_Kp = -(float)( (RxBuffer1[1]>>4)*1000 + (RxBuffer1[1]&0x0f)*100 + (RxBuffer1[2]>>4)*10 + (RxBuffer1[2]&0x0f) );
				Vertical_Kd = -(float)( (RxBuffer1[3]>>4)*10 + (RxBuffer1[3]&0x0f) );
				Velocity_Kp = -( (float)( (RxBuffer1[4]>>4)*10 + (RxBuffer1[4]&0x0f) ) )/10;
				Velocity_Ki = Velocity_Kp/200;
			}
			else if(RxBuffer1[0] == 0x5e)
			{
				if(RxBuffer1[3] == 0)	Speed_Coeff = -1;
				else 					Speed_Coeff = 1;
				
				Target_Speed = Speed_Coeff * ( (RxBuffer1[4]>>4)*10 + (RxBuffer1[4]&0x0f) );
			}
		}
		
		if(ULTR_Status && Speed_Coeff) Target_Speed = 0;
		//usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(Roll*100),(int)(Pitch*100),(int)(Yaw*10));
	}
}


