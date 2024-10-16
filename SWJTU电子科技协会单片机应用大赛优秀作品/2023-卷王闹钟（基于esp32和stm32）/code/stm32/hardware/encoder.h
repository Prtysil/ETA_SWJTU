#include "stm32f10x.h"                  // Device header
#ifndef __ENCODER_H
#define __ENCODER_H
void encoder_init(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
int16_t data_read(void);
extern int16_t encoder_count;
#endif


