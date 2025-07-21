#include "route.h"

float Route_Error = 0;//存储最终输出误差
volatile uint8_t Route_Flag = 0;
volatile uint8_t Turn_Flag = 0;//是否转弯

//权值法
int8_t senser_value[6] = {-15,-10,-5,5,10,15};

//8路灰度用其6路,检测到黑线为高电平
uint8_t senser_state[6] = {0};



float Get_Route_Error()
{
	return Route_Error;
}

void Route_Open()
{
	Route_Flag = 1;
}
void Route_Close()
{
	Route_Flag = 0;
	
	//关闭后状态复位
	for(int i = 0; i<6; i++)
	{
		senser_state[i] = 0;
	}
	Route_Error = 0;
}


void Get_Route_Error_Tick()
{
	if(Route_Flag == 1)
	{
		static int count;
		int8_t error_buffer = 0;
		uint8_t senser_num = 0;
 		
		count++;
		if(count >= 10)//每10ms一次
		{
			count = 0;
			error_buffer = 0;
			senser_num = 0;
			
			senser_state[0] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
			senser_state[1] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
			senser_state[2] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
			senser_state[3] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
			senser_state[4] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
			senser_state[5] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12);
			
			for(uint8_t i = 0; i < 6 ;i++)
			{
				if(senser_state[i] == GPIO_PIN_SET)
				{
					senser_num++;
					error_buffer += senser_value[i];
				}
			}
			Route_Error = (senser_num == 0)?0.0f:(float)error_buffer/senser_num;
			if(senser_num >= 5)
			{
				Turn_Flag = 1;
			}
		}
	}
}