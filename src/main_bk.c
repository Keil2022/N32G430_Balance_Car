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
#include "i2c_drv.h"
#include "oled.h"
#include "mpu6050.h"
#include "bsp_delay.h"
#include "mpu6050.h"
#include "math.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "timer.h"
#include "oled.h"

void Print_Acc(short Accx,short Accy,short Accz);
void Print_yaw(float pitch,float roll,float yaw);
void ACC_Self_Calibration(short *cali_x,short *cali_y,short *cali_z);
void Time_Init(void);
void Get_Data_And_Deal(float a_x,float a_y);
void Get_Acc_Average(short *acc_x,short *acc_y,short *acc_z);
void Draw_Trajectory(void);

uint8_t interrupt_200ms_flag = 0;
// x，y 位移坐标  
static float s_x = 0,s_y = 0;

#define    Capture_Time_N						100

static u16 index = 0;
static float acc_x_buffer[Capture_Time_N] = {0.0},acc_y_buffer[Capture_Time_N] = {0.0};
/**
 *\*\name   main.
 *\*\fun    main function.
 *\*\param  none.
 *\*\return none.
**/
int main(void)
{
	float pitch,roll,yaw,A_x,A_y; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short cali_x,cali_y,cali_z;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	u8 t=0;
	log_init();
	log_info("BEEPER Output Test Start\r\n");
	i2c_master_init();
	oled_init();
	MPU_Init();					//初始化MPU6050
	Time_Init();
	while(mpu_dmp_init());
	
//	OLED_ShowString(0,0,"ALIENTEK",24);  
//	OLED_ShowString(0,24, "0.96' OLED TEST",16);  
// 	OLED_ShowString(0,40,"ATOM 2015/1/14",12);  
// 	OLED_ShowString(0,52,"ASCII:",12);  
// 	OLED_ShowString(64,52,"CODE:",12);
	//Move_Step();
	//Random_Rresh_Block();
	OLED_Refresh_Gram();		//?????OLED 
	ACC_Self_Calibration(&cali_x,&cali_y,&cali_z);
	log_info("BEEPER Output Test End\r\n");
	while(1)
	{
		float temp1,temp2;
		u16 i = 0 ; 
		if(interrupt_200ms_flag )				//50ms 中断一次
		{
				interrupt_200ms_flag = 0;
				//MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
				Get_Acc_Average(&aacx,&aacy,&aacz);
				while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0);		//获取当前的航向角
			 //加速度转换为大地坐标系下的分量
				aacx = aacx - cali_x;
				aacy = aacy - cali_y;
				aacz = aacz - cali_z;
			
				//扣除重力加速度的分量
				temp1 = 16384*sin((pitch*3.1415)/180);
				temp2 = 16384*sin((roll*3.1415)/180);
				temp1 = (aacx*1.0 + temp1)/16384.0;    //这是就是+ ，为抵消重量加速分量
				temp2 = (aacy*1.0 - temp2)/16384.0;
			
				//投射到绝对坐标系
				A_x = temp1*cos(yaw/180*3.1415) + temp2*sin(yaw/180*3.1415);
				A_y = temp2*cos(yaw/180*3.1415) - temp1*sin(yaw/180*3.1415);
				if(index >= Capture_Time_N -2 )
					index++;
				else
					index = 0;
				Get_Data_And_Deal(A_x,A_y);
				Draw_Trajectory();
        //log_info("a_x[%.1f],a_y[%.1f]\r\n",A_x,A_y);
				if((t%20)==0)								//1s 中断一次		
				{ 
//						while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0);		//获取当前的航向角
//					
//						Get_Acc_Average(&aacx,&aacy,&aacz);

//						aacx = aacx - cali_x;
//						aacy = aacy - cali_y;
//						aacz = aacz - cali_z;
//						log_info("trans before :pitch = %f,roll = %f,yaw = %f\r\n",pitch,roll,yaw);
//						Print_Acc(aacx,aacy,aacz);
//					
//						//扣除重力加速度的分量
//						temp1 = 16384*sin((pitch*3.1415)/180);
//						temp2 = 16384*sin((roll*3.1415)/180);
//						temp1 = (aacx*1.0 + temp1)/16384.0;    //这是就是+ ，为抵消重量加速分量
//						temp2 = (aacy*1.0 - temp2)/16384.0;
//					
//						//投射到绝对坐标系
//						A_x = temp1*cos(yaw/180*3.1415) + temp2*sin(yaw/180*3.1415);
//						A_y = temp2*cos(yaw/180*3.1415) - temp1*sin(yaw/180*3.1415);
//						for(i = 0;i<60;i++)
//							log_info("[%d]%f ",i,acc_x_buffer[i]);
//						for(i = 0;i<60;i++)
//							log_info("[%d]%f ",i,acc_y_buffer[i]);
						
		//				Get_Data_And_Deal(A_x,A_y);
		//				Draw_Trajectory();
						 t=0;
				}
				t++;	
		}
 
	}
}
void Print_Acc(short Accx,short Accy,short Accz)
{
		short temp;
		temp=Accx;
		if(temp<0)
		{
			temp=-temp;		//转为正数
			log_info("Accx = -%d\r\n",temp);
		}
		else
		{
			log_info("Accx = %d\r\n",temp);
		}
		temp=Accy;
		if(temp<0)
		{
			temp=-temp;		//转为正数
			log_info("Accy = -%d\r\n",temp);
		}else
		{
			log_info("Accy = %d\r\n",temp);
		}
		temp=Accz;
		if(temp<0)
		{
			temp=-temp;		//转为正数
			log_info("Accz = -%d\r\n",temp);
		}else
		{
			log_info("Accz = %d\r\n",temp);
		}

}
void Print_yaw(float pitch,float roll,float yaw)
{
	float temp;
	temp=pitch;
		if(temp<0)
		{
			temp=-temp;		//转为正数
			log_info("pitch = -%f\r\n",temp);
		}
		else
		{
			log_info("pitch = %f\r\n",temp);
		}
		temp=roll;
		if(temp<0)
		{
			temp=-temp;		//转为正数
			log_info("rol = -%f\r\n",temp);
		}else
		{
			log_info("rol = %f.\r\n",temp);
		}
		temp=yaw;
		if(temp<0)
		{
			temp=-temp;		//转为正数
			log_info("yaw = -%f\r\n",temp);
		}else
		{
			log_info("yaw = %f\r\n",temp);
		}
}

short Get_Avarage(short *Data, uint8_t len)
{
	u8 i =0;
	int32_t sum   = 0;
	
	for(i =0;i<len;i++)
		sum += (int32_t)(Data[i]);
	
	return (short)(sum /len);
}
#define     Capture_Times_N       3

void Get_Acc_Average(short *acc_x,short *acc_y,short *acc_z)
{
		u8 i =0;
		short temp_x[Capture_Times_N] = {0},temp_y[Capture_Times_N] ={0},temp_z[Capture_Times_N] ={0};
		for(i =0;i<Capture_Times_N;i++)
		{
				MPU_Get_Accelerometer(&temp_x[i],&temp_y[i],&temp_z[i]);
			
		}
		*acc_x = Get_Avarage(temp_x,Capture_Times_N);
		*acc_y = Get_Avarage(temp_y,Capture_Times_N);
		*acc_z = Get_Avarage(temp_z,Capture_Times_N);
}
void ACC_Self_Calibration(short *cali_x,short *cali_y,short *cali_z)
{
		u8 i =0;
		short temp_x[100] = {0},temp_y[100] ={0},temp_z[100] ={0};
		for(i =0;i<100;i++)
		{
				MPU_Get_Accelerometer(&temp_x[i],&temp_y[i],&temp_z[i]);
			
		}
		*cali_x = Get_Avarage(temp_x,100);
		*cali_y = Get_Avarage(temp_y,100);
		*cali_z = Get_Avarage(temp_z,100) - 16384;
		Print_Acc(*cali_x,*cali_y,*cali_z);
}
void Print_Intdata(char *string,int32_t data)
{
	if(data >0)
		log_info("%s data= %d\r\n",string,data);
	else
		log_info("%s data= -%d\r\n",string,-data);

}
void Get_Data_And_Deal(float a_x,float a_y)
{
		int32_t 	temp_x ,temp_y;
		static int32_t 	v_x = 0,v_y = 0;

		temp_x = ((int32_t)(a_x*100));				//将加速度放大100倍数，并且舍弃最后一位小数点
		temp_y = ((int32_t)(a_y*100));
//		if(( (temp_x<20)&&(temp_x >0))||((temp_x <0)&&(-temp_x <20)))
//			temp_x = 0;
//		if(( (temp_y<20)&&(temp_y >0))||((temp_y <0)&&(-temp_y <20)))
//			temp_y = 0;

		if((temp_x == 0)&&(temp_y == 0))
		{
				v_x = 0;
				v_y = 0;
		}
		else
		{
				v_x = v_x + temp_x*0.05*100;
				v_y = v_y + temp_y*0.05*100;
		}
		
	
		s_x = s_x + v_x*0.05*0.01;
		s_y = s_y + v_y*0.05*0.01;
		//log_info("-[%d][%d][%.1f][%.1f]-\r\n",temp_x,temp_y,v_x,v_y);
		log_info("a_x = %.3f,a_x = %.3f,v_x= %d，v_y= %d,s_x= %f,s_y= %f\r\n",a_x,a_y,v_x,v_y,s_x,s_y);
}
#define  MAX_X		50
#define  MIN_X		-50
#define  MAX_Y		25
#define  MIN_Y		-25


extern uint8_t OLED_GRAM[128][8];
void Draw_Trajectory(void)
{
		u8 i,n;
	  //temp_x->[-500,500]对应屏幕像素[-64，64]   temp_y[-250,250]对应屏幕像素[-32，32]
		u8 temp_x = 0,temp_y = 0;
		if(s_y < MIN_X |s_x < MIN_Y | s_x> MAX_Y | s_y > MAX_X)
			return;
		
		temp_x = (u8)(64*s_y/MAX_X+64);
		temp_y = (u8)(32*s_x/MAX_Y+32);

		OLED_DrawPoint(temp_x,temp_y,1);
		log_info("temp_x = %d,a_y = %d\r\n",temp_x,temp_y);
		
		i=7-temp_y/8;
			
		Out_Oled(MOC_Command,0xb0+i);    //?????(0~7)
		Out_Oled(MOC_Command,0x00);      //??????????
		Out_Oled(MOC_Command,0x10);      //??????????  
			
    for(n=0;n<128;n++)Out_Oled(MOC_Data,OLED_GRAM[n][i]); 
    //OLED_Refresh_Gram(); 
}
