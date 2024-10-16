#ifndef _HCSR04_H__
#define _HCSR04_H__

#include "stm32h7xx.h"                  // Device header
#include "main.h"
#include "tim.h"
void HCSR04_Init(void);
void HCSR04_Strat(void);
uint16_t HCSR04_GetValue(void);


#endif
