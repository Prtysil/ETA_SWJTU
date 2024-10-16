#ifndef __DELAY_H
#define __DELAY_H 
 
#include "stm32f10x.h"
 
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
 
 
//void delay_init(u8 SYSCLK);
void delay_init(void);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void delay_xms(u32 nms);
 
#endif
