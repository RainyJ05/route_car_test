#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f4xx.h"                  // Device header

void Load(int moto1,int moto2);
void Motor_Init();

float Get_Left_Speed();
float Get_Right_Speed();
float Get_Left_Distance();
float Get_Right_Distance();
void Get_Encoder_Open();
void Get_Encoder_Close();
void Get_Encoder_Tick();
void Turn_Back();
void Turn_Left();
void Turn_Right();
void GO_Distance(int distance);
#endif
