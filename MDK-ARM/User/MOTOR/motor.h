#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f4xx.h"                  // Device header

void Load(int moto1,int moto2);
void Motor_Init();

float Get_Left_Speed();
float Get_Right_Speed();
void Get_Speed_Open();
void Get_Speed_Close();
void Get_Speed_Tick();


#endif
