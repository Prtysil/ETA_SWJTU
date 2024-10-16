#include "stm32f10x.h"                  // Device header

#ifndef __SEG_BUF_H
#define __SEG_BUF_H

void SEG_Init(void);
void SEG_DisplayValue(uint8_t value,uint8_t pos,uint8_t time);
void seg_show_1(void);
void seg_show_2(void);
void seg_show_3(void);
extern uint16_t second,minute,hour;
extern uint8_t screen_flag;
#endif

