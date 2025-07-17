#include "key.h"

uint8_t Key_Num;
uint8_t Key_Flag = 0;

//控制开关
void Key_Open()
{
	Key_Flag = 1;
}
void Key_Close()
{
	Key_Flag = 0;
}


//返回键值
uint8_t Key_GetNum(void)
{
	uint8_t temp = Key_Num;
	Key_Num = 0;
	return temp;
}
//扫描按键
uint8_t Key_GetState(void)
{
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == 0)
	{
		return 1;
	}
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) == 0)
	{
		return 2;
	}
	return 0;
}
//在定时器中断里运行，实现非阻塞扫描按键，定时器中断1kKz,每20毫秒扫描一次，天然消抖
void Key_Tick(void)
{
	if(Key_Flag == 1)
	{
		static uint8_t count;
		static uint8_t CurrState,PreState;
		
		count ++;
		if(count >= 20)
		{
			count = 0;
			PreState = CurrState;
			CurrState = Key_GetState();
			
			if(CurrState == 0&&PreState != 0)
			{
				Key_Num = PreState;
			}
		}
	}
}


