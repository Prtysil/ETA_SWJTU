#include "seg_buf.h"
#include "dht11.h"
#include "delay.h"
#include "lightsensor.h"
#include "key.h"
#define sclk GPIO_Pin_4
#define rclk GPIO_Pin_5
#define DIO GPIO_Pin_6
uint8_t seg[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xc6}; //0-F;
uint16_t second=00,minute=53,hour=22;
uint8_t count_seg=0;
uint8_t screen_flag=0;
uint8_t temp=24, water=35;
void SEG_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void SEG_DisplayValue(uint8_t value,uint8_t pos,uint8_t time)
{
	uint8_t i = 0;
	uint8_t code_tmp = 0;
	code_tmp = ~seg[value];
	
	switch(pos)
	{
		case 1 : pos = 0xef; break;
		case 2 : pos = 0xdf; break;
		case 3 : pos = 0xbf; break;
		case 4 : pos = 0x7f; break;
		case 5 : pos = 0xfe; break;
		case 6 : pos = 0xfd; break;
		case 7 : pos = 0xfb; break;
		case 8 : pos = 0xf7; break;
	}
	
	for(i=0;i<8;i++)
	{
		if(code_tmp & 0x80)
		{
			GPIO_SetBits(GPIOA,DIO);
		}
		else
		{
			GPIO_ResetBits(GPIOA,DIO);
		}
		GPIO_SetBits(GPIOA,sclk);//缁欎竴涓笂鍗囨部鍐欏叆
		code_tmp = code_tmp << 1;   
		GPIO_ResetBits(GPIOA,sclk);
	}
	count_seg++;	
	if(count_seg < time)
	{
		pos = pos|0xff;
	}
	else if(count_seg == 50)
	{
		count_seg = 0;
	}
		for(i=0;i<8;i++)
		{
			if(pos & 0x80)
			{
				GPIO_SetBits(GPIOA,DIO);
			}
			else
			{
				GPIO_ResetBits(GPIOA,DIO);
			}          
			GPIO_SetBits(GPIOA,sclk);//缁欎竴涓笂鍗囨部鍐欏叆
			pos = pos << 1;   
			GPIO_ResetBits(GPIOA,sclk);
		}
		GPIO_SetBits(GPIOA,rclk);
		GPIO_ResetBits(GPIOA,rclk);
  
}

void seg_show_1()
{
	if(second>=60)
	{
		second = 0;
		minute++;
	}
	if(minute>=60)
	{
		minute = 0;
		hour++;
	}
	if(hour>=24)
	{
		hour = 0;
		minute = 0;
		second = 0;
	}
	
	if(lightsensor_get()==0)
	{
	SEG_DisplayValue(hour/10,1,0);
	SEG_DisplayValue(hour%10,2,0);
	
	SEG_DisplayValue(minute/10,3,0);
	SEG_DisplayValue(minute%10,4,0);
	
	SEG_DisplayValue(second/10,5,0);
	SEG_DisplayValue(second%10,6,0);
	}
	else if(lightsensor_get()==1)
	{
	SEG_DisplayValue(hour/10,1,45);
	SEG_DisplayValue(hour%10,2,45);
	
	SEG_DisplayValue(minute/10,3,45);
	SEG_DisplayValue(minute%10,4,45);
	
	SEG_DisplayValue(second/10,5,45);
	SEG_DisplayValue(second%10,6,45);
	}
}

void seg_show_2()
{
	SEG_DisplayValue(clock_h/10,1,0);
	SEG_DisplayValue(clock_h%10,2,0);
	
	SEG_DisplayValue(clock_m/10,3,0);
	SEG_DisplayValue(clock_m%10,4,0);	
}

void seg_show_3()
{
	SEG_DisplayValue(temp/10,2,0);
	SEG_DisplayValue(temp%10+(int)((second%10)/8),3,0);
	SEG_DisplayValue(10,1,0);
	SEG_DisplayValue(water/10,5,0);
	SEG_DisplayValue(water%10+(int)((second%10)/3),6,0);	
}

