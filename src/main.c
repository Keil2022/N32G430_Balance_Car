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
//#include "log.h"
//#include "i2c_drv.h"
//#include "oled.h"
//#include "mpu6050.h"
//#include "bsp_delay.h"
//#include "mpu6050.h"
//#include "math.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h"

#include "esp12f_io0.h"
#include "led.h"
#include "key.h"
#include "adc.h"
#include "exti.h"

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
	
	EXTI_Init(KEY_INPUT_PORT, KEY_INPUT_PIN);
	
	while(1)
	{
		//else	LED_On;
	}
}

void EXTI9_5_IRQHandler(void)
{
    if (RESET != EXTI_Interrupt_Status_Get(KEY_INPUT_EXTI_LINE))	//一级判断
    {
        if(Exti == 0)	//二级判断
		{
			LED_Blink;
			EXTI_Interrupt_Status_Clear(KEY_INPUT_EXTI_LINE);
		}
    }
}


