#include "motor.h" 


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

int Left_Encoder = 0;
int Right_Encoder = 0;

int Left_Distance = 0;
int Right_Distance = 0;


uint8_t Get_Encoder_Flag = 0;
uint8_t Get_Distance_Flag = 0;


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
	if(moto1>0)
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
	if(moto2>0)
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
	return (float)Left_Encoder*100/13/2/30;
}

float Get_Right_Speed()
{
	return (float)Right_Encoder*100/13/2/30;
}

float Get_Left_Distance()
{
	return (float)Left_Distance*100/13/2/30;
}

float Get_Right_Distance()
{
	return (float)Right_Distance*100/13/2/30;
}

void Get_Encoder_Open()
{
	Get_Encoder_Flag = 1;
}

void Get_Encoder_Close()
{
	Get_Encoder_Flag = 0;
}

void Get_Encoder_Tick()
{
	if(Get_Encoder_Flag == 1)
	{
		static uint8_t count;
		count++;
		if(count >= 20)
		{
			count = 0;
			
			Left_Encoder = -(short)__HAL_TIM_GetCounter(&htim5);
			Right_Encoder = (short)__HAL_TIM_GetCounter(&htim4);
			
			if(Get_Distance_Flag == 1)
			{
				Left_Distance += Left_Encoder;
				Right_Distance += Right_Encoder;
			}
		
			
			__HAL_TIM_SetCounter(&htim5,0);
			__HAL_TIM_SetCounter(&htim4,0);
	
		}
	}
}

void Clear_Distance()
{
	Left_Distance = 0;
	Right_Distance = 0;
}

void Turn_Back()
{
//	Get_Encoder_Open();
	Load(-100,100);
	Get_Distance_Flag = 1;
	Clear_Distance();
	int16_t target_left = 0;
	int16_t target_right = 0;
	while((target_left>-240)&&(target_right<240))
	{
		target_left = Get_Left_Distance();
		target_right = Get_Right_Distance();
	}
	Load(0,0);
	Get_Distance_Flag = 0;
//	Get_Encoder_Close();
}

void Turn_Left()
{
//	Get_Encoder_Open();
	Load(-100,100);
	Get_Distance_Flag = 1;
	Clear_Distance();
	int16_t target_left = 0;
	int16_t target_right = 0;
	while((target_left>-120)&&(target_right<120))
	{
		target_left = Get_Left_Distance();
		target_right = Get_Right_Distance();
	}
	Load(0,0);
	Get_Distance_Flag = 0;
//	Get_Encoder_Close();
}

void Turn_Right()
{
//	Get_Encoder_Open();
	Load(100,-100);
	Get_Distance_Flag = 1;
	Clear_Distance();
	int16_t target_left = 0;
	int16_t target_right = 0;
	while((target_left < 120)&&(target_right > -120))
	{
		target_left = Get_Left_Distance();
		target_right = Get_Right_Distance();
	}
	Load(0,0);
	Get_Distance_Flag = 0;
//	Get_Encoder_Close();
}

void GO_Distance(int distance)
{
	Get_Distance_Flag = 1;
	Load(150,150);
	Clear_Distance();
	int16_t target_left = 0;
	int16_t target_right = 0;
	while((target_left <distance)&&(target_right < distance))
	{
		target_left = Get_Left_Distance();
		target_right = Get_Right_Distance();
	}
	Load(0,0);
	Get_Distance_Flag = 0;
}

