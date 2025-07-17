#ifndef __PID_H
#define __PID_H		

#include "stm32f4xx.h"                  // Device header


void Pid_Controller_Tick();
void Pid_Close();
void Pid_Open(int base);

#endif
