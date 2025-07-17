#include "route.h"

int Route_State = 0;
int Route_Flag = 0;
uint8_t senser1 = 0;
uint8_t senser2 = 0;
uint8_t senser3 = 0;
uint8_t senser4 = 0;

/*

返回值  4路模块
0				1 0 0 1  直线
-1      1 1 0 1  左偏
1       1 0 1 1  右偏
-2      1 1 1 0  左转
2       0 1 1 1  右转 
*/
int Get_Route_State()
{
	return Route_State;
}

void Route_Open()
{
	Route_Flag = 1;
}
void Route_Close()
{
	Route_Flag = 0;
}



void Get_Route_State_Tick()
{
	if(Route_Flag == 1)
	{
		static int count;
		count++;
		if(count >= 10)//每10ms一次
		{
			count = 0;
			senser1 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
			senser2 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
			senser3 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
			senser4 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
			if((senser1 == 1)&&(senser2 == 0)&&(senser3 == 0)&&(senser4 == 1))
			{
				Route_State =  0;
			}
			if((senser1 == 1)&&(senser2 == 1)&&(senser3 == 0)&&(senser4 == 1))
			{
				Route_State =  -1;
			}
			if((senser1 == 1)&&(senser2 == 0)&&(senser3 == 1)&&(senser4 == 1))
			{
				Route_State =  1;
			}
			if((senser1 == 1)&&(senser2 == 1)&&(senser3 == 1)&&(senser4 == 0))
			{
				Route_State =  -2;
			}
			if((senser1 == 0)&&(senser2 == 1)&&(senser3 == 1)&&(senser4 == 1))
			{
				Route_State =  2;
			}
		}
	}
}