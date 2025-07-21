#include "pid.h"
#include "route.h"
#include "motor.h"

int TurnChoose[5] = {0,0,0,0,0};//路口行动选择，由串口数据修改
uint8_t Dir = 1;//方向分送药和返程
uint8_t Des = 0;//路口分路中和到达
uint8_t Num = 0;//记录路口数量

void turn_process()
{
	//停止
	Load(0,0);
	Pid_Close();
	Route_Close();
	if(Des == 1)//若为到达
	{
		Turn_Back();
		Des = 0;//清空标志位
		Dir = 0;//返回
	}
	else//否则为路中
	{
		if(Dir == 1)//若为送药
		{
			switch(TurnChoose[Num])
			{
				case 0:
					GO_Distance(100);
					break;//不转弯，直接过
				case 1:
					if((Num == 0)||(Num == 1)||(Num == 3))
					{
						Des = 1;//在这三个路口发生左右转，意味着下一个路口为到达
					}
					GO_Distance(100);
					Turn_Left();
					GO_Distance(100);
					break;
				case 2:
					if((Num == 0)||(Num == 1)||(Num == 3))
					{
						Des = 1;//在这三个路口发生左右转，意味着下一个路口为到达
					}
					GO_Distance(100);
					Turn_Right();
				  GO_Distance(100);
				  break;
			}
			Num++;//路口数后自增
	  }
		else//否则为返回
		{
			Num--;//路口数先自减
			if(Num == 0)
			{
				Des = 1;//下一个路口为到达
			}
			switch(TurnChoose[Num])//原路返回
			{
				case 0:
					GO_Distance(100);
					break;//不转弯，直接过
				case 1:
					GO_Distance(100);
					Turn_Right();
					GO_Distance(100);
			  	break;
				case 2:
					GO_Distance(100);
					Turn_Left();
					GO_Distance(100);
					break;
			}
		}
		//若在路中，则执行完行动后继续
		Route_Open();
		Pid_Open(200);
	}
}