#include "pid.h"
#include "route.h"
#include "motor.h"

uint8_t Pid_Flag = 0;//控制标志位
int Base_Pwm = 0;//基础速度


// PID参数
#define KP 1.0f   // 比例系数
#define KI 0.0f   // 积分系数
#define KD 0.0f  // 微分系数

void Pid_Open(int base)
{
	Pid_Flag = 1;
	Base_Pwm = base;
}

void Pid_Close()
{
	Pid_Flag = 0;
	Load(0,0);
}

void Pid_Controller_Tick()//中断里20ms执行一次
{
	if(Pid_Flag == 1)
	{
		static uint8_t count;
		static float error_sum;
		static float last_error;
		
		count++;
		if(count >= 20)
		{
			count = 0;
			
			float error = 0;
			float output = 0;
		
			error = Get_Route_State();
			error_sum += error;
			
			output = KP * error + KI * error_sum + KD * (error - last_error);
			
			Load(Base_Pwm - output*100,Base_Pwm + output*100);
		}
	}
}