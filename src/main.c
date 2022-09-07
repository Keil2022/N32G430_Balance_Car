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
#include "log.h"
//#include "i2c_drv.h"
//#include "oled.h"
//#include "math.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "bsp_delay.h"

#include "esp12f_io0.h"
#include "led.h"
#include "key.h"
#include "adc.h"
#include "exti.h"
#include "pwm.h"
#include "encoder.h"
#include "motor.h"

int Speed_L,Speed_R;

extern uint16_t Channel1Pulse;
extern int Encoder_Left,Encoder_Right;	//编码器数据（速度）

/**
 *\*\name   main.
 *\*\fun    main function.
 *\*\param  none.
 *\*\return none.
**/
int main(void)
{
	ESP12F_IO0_Init();
	ADC_Configuration();
	
	LED_GPIO_Init();
	Key_GPIO_Init();
	
	TIMx_PWM_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM3_Init();
	
	log_init();
	
	MPU_Init();								//初始化MPU6050
	while( mpu_dmp_init() )	{;}				//等待MPU初始化完成
	LED_On;
	
	EXTI_Init(KEY_INPUT_PORT, KEY_INPUT_PIN);
	
	while(1)
	{
//		Load(Channel1Pulse,Channel1Pulse);
//		Encoder_Left  = Read_Speed(2);	//电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反。
//		Encoder_Right = -Read_Speed(3);
//		SysTick_Delay_Ms(100);
		
	}
}


