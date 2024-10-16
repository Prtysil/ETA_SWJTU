#ifndef __DS1302_H
#define __DS1302_H
#include "stm32f10x.h"                  // Device header

typedef struct {
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  minute;
	uint8_t  second;
	uint8_t  week;
} DS1302TimeStruct;

typedef struct {
	void (* Init)(void);
	void (* SetTime)(uint8_t *writeTime);
	void (* GetTime)(DS1302TimeStruct *timeData);
} DS1302ClassStruct;

extern DS1302ClassStruct DS1302Class;
 
#endif
