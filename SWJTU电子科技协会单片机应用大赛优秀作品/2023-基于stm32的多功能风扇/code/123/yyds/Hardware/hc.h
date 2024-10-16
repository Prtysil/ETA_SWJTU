#ifndef __HC_H
#define __HC_H	

#include "stm32f10x.h"

void Hcsr04Init(void);
static void NVIC_Config(void);
void initHcsr04(void);
static void OpenTimer(void);
static void CloseTimer(void);
void TIM2_IRQHandler(void);
u32 GetEchoTimer(void);
float Hcsr04GetLength(void );


#endif

