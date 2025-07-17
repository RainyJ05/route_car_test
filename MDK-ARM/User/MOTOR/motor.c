#include "motor.h" 


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

int Left_Encoder = 0;
int Right_Encoder = 0;

uint8_t Get_Speed_Flag = 0;


#define PWM_MAX 1000
#define PWM_MIN -1000

void Motor_Init()
{
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5,TIM_CHANNEL_ALL);
}

int abs(int p)//直流电机pwm绝对值函数
{
	if(p>0)
		return p;
	else
		return -p;
}

void Load(int moto1,int moto2)//直流电机占空比修改函数-1000~1000
{
	if(moto1<0)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	}
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,abs(moto1));
	if(moto2<0)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
	}
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,abs(moto2));
}

void Limit(int *motoA,int *motoB)//直流电机pwm限幅
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/*
测速相关
*/
float Get_Left_Speed()
{
	return (float)Left_Encoder*100/13/4/30;
}

float Get_Right_Speed()
{
	return (float)Right_Encoder*100/13/4/30;
}

void Get_Speed_Open()
{
	Get_Speed_Flag = 1;
}

void Get_Speed_Close()
{
	Get_Speed_Flag = 0;
}

void Get_Speed_Tick()
{
	if(Get_Speed_Flag == 1)
	{
		static uint8_t count;
		count++;
		if(count >= 10)
		{
			count = 0;
			
			Left_Encoder = -(short)__HAL_TIM_GetCounter(&htim4);
			Right_Encoder = (short)__HAL_TIM_GetCounter(&htim5);
			
			__HAL_TIM_SetCounter(&htim4,0);
			__HAL_TIM_SetCounter(&htim5,0);
		}
	}
}





