#ifndef __COUNTSENSOR_H
#define __COUNTSENSOR_H
#include "stm32f10x.h"                  // Device header
void sensor_init(void);
void EXTI15_10_IRQHandler(void);
extern uint16_t count;
#endif
