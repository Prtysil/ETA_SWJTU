#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header
void key_init(void);
void Key_GetNum(void);

extern uint8_t clock_h, clock_m; 
extern uint8_t num;
#endif

