#ifndef __ROUTE_H
#define __ROUTE_H		

#include "stm32f4xx.h"                  // Device header

void Get_Route_Error_Tick();
float Get_Route_Error();
void Route_Open();
void Route_Close();
extern volatile uint8_t Turn_Flag;

#endif
