#include "control.h"
#include "n32g430.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "exti.h"
#include "encoder.h"
#include "led.h"

float Med_Angle = 0;	//机械中值。---在这里修改你的机械中值即可。
float Target_Speed = 0;	//期望速度。---二次开发接口，用于控制小车前进后退及其速度。

float 
	Vertical_Kp = 200,//直立环KP、KD
	Vertical_Kd = 0;
float 
	Velocity_Kp = 0,//速度环KP、KI
	Velocity_Ki = 0;

int Vertical_out,Velocity_out,Turn_out;//直立环&速度环&转向环 的输出变量

float Pitch,Roll,Yaw;				//角度
short gyrox,gyroy,gyroz;			//陀螺仪--角速度
short aacx,aacy,aacz;				//加速度
int Encoder_Left,Encoder_Right;		//编码器数据（速度）
int MOTO1,MOTO2;					//电机装载变量

void EXTI9_5_IRQHandler(void)
{
    if (RESET != EXTI_Interrupt_Status_Get(KEY_INPUT_EXTI_LINE))	//一级判断
    {
        int PWM_out;
		if( (KEY_INPUT_PORT->PID & KEY_INPUT_PIN) == 0)	//二级判断
		{
			EXTI_Interrupt_Status_Clear(KEY_INPUT_EXTI_LINE);
			
			//LED_On;	//50ms+ 的运行时间
			//LED_Blink;
			
			//【1】采集编码器数据&MPU6050角度信息。
			Encoder_Left  = Read_Speed(2);	//电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反。
			Encoder_Right = -Read_Speed(3);
			
			while(mpu_dmp_get_data(&Pitch,&Roll,&Yaw) != 0)	//获取当前的航向角
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);		//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);		//得到陀螺仪数据
			
//			//【2】将数据压入闭环控制中，计算出控制输出量。
//			Velocity_out = Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//速度环
//			Vertical_out = Vertical(Velocity_out+Med_Angle,Pitch,gyroy);		//直立环
//			//Turn_out = Turn(gyroz);											//转向环
//			PWM_out = Vertical_out;//最终输出
			
			//2.将数据压入闭环控制中，计算出控制输出量。
			Vertical_out = Vertical(Med_Angle,Pitch,gyroy);				//直立环
			Velocity_out = Velocity(Target_Speed,Encoder_Left,Encoder_Right);			//速度环
			//Turn_out = Turn(gyroz);										//转向环
			
			PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//最终输出
			
			//【3】把控制输出量加载到电机上，完成最终的的控制。
			MOTO1 = PWM_out-Turn_out;	//左电机
			MOTO2 = PWM_out+Turn_out;	//右电机
			Limit(&MOTO1,&MOTO2);	 	//PWM限幅			
			Load(MOTO1,MOTO2);		 	//加载到电机上。
			
//			Stop(&Med_Angle,&Pitch);
			//LED_Off;
		}
	}
}




/*********************
直立环PD控制器：Kp*Ek+Kd*Ek_D

入口：期望角度、真实角度、真实角速度
出口：直立环输出
*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_out;
}


/*********************
速度环PI：Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int Encoder_S,EnC_Err_Lowout_last,PWM_out,Encoder_Err,EnC_Err_Lowout;
	float a=0.7;
	
	//1.计算速度偏差
	Encoder_Err=((encoder_left+encoder_right)-Target);//舍去误差--我的理解：能够让速度为"0"的角度，就是机械中值。
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//使得波形更加平滑，滤除高频干扰，防止速度突变。
	EnC_Err_Lowout_last=EnC_Err_Lowout;//防止速度过大的影响直立环的正常工作。
	//3.对速度偏差积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;
	//4.积分限幅
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	
	//5.速度环控制输出计算
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_out;
}



/*********************
转向环：系数*Z轴角速度
*********************/
int Turn(int gyro_Z)
{
	int PWM_out;
	
	PWM_out = (-0.6)*gyro_Z;
	return PWM_out;
}

