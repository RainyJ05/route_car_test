#ifndef __ROUTE_H
#define __ROUTE_H		

#include "stm32f4xx.h"                  // Device header

void Get_Route_State_Tick();
int Get_Route_State();
void Route_Open();
void Route_Close();

#endif
