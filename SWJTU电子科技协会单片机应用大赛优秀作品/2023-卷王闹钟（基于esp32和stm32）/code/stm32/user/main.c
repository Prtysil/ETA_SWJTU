#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "key.h"
#include "seg_buf.h"
#include "OLED.h"
#include "countsensor.h"
#include "lightsensor.h"
#include "encoder.h"
#include "ds1302.h"
#include "dht11.h"
#include "delay.h"
#include "timer.h"

extern char DS1302_Time[7];
int main(void)
{
	key_init();
	SEG_Init();
	Timer_Init();
	lightsensor_init();
	
	while(1)
	{
		Key_GetNum();	
		if(screen_flag==0)
		{
			seg_show_1();
		}
		if(screen_flag==1)
		{
			seg_show_2();
		}
		if(screen_flag==2)
		{
			seg_show_3();
		}
	}
}



void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		second++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

