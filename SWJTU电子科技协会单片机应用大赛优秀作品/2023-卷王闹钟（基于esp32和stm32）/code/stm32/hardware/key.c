#include "key.h"
#include "delay.h"
#include "seg_buf.h"
#define key_0 GPIO_Pin_11  //A
#define key_1 GPIO_Pin_4
#define key_2 GPIO_Pin_7
#define key_3 GPIO_Pin_12  //A
#define key_4 GPIO_Pin_5
#define key_5 GPIO_Pin_8
#define key_6 GPIO_Pin_15  //A
#define key_7 GPIO_Pin_6
#define key_8 GPIO_Pin_9
#define key_9 GPIO_Pin_3
#define key_clean GPIO_Pin_9
#define key_set GPIO_Pin_9

uint8_t clock_h=0, clock_m=0,f=0; //按下次数
uint8_t num=0;
void key_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure1.GPIO_Pin = key_1|key_2|key_4|key_5|key_7|key_8|key_9;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure2.GPIO_Pin = key_0|key_3|key_6;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure2);
}

void Key_GetNum(void)
{
	/*
	if(GPIO_ReadInputDataBit(GPIOA, key_0)==1)
	{
		while(GPIO_ReadInputDataBit(GPIOA, key_0)==1);
		num = 0;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, key_1)==1)
	{
		while(GPIO_ReadInputDataBit(GPIOB, key_1)==1);
		num = 1;
	}
	*/
	if(GPIO_ReadInputDataBit(GPIOB, key_2)==1)
	{
		delay_ms(200);
		while(GPIO_ReadInputDataBit(GPIOB, key_2)==1);
		num = 2;
    if(f==3)
		{
		clock_m += 2;
		f++;
		}
		if(f==2)
		{
		clock_m = 20;
		f++;
		}
		if(f==1)
		{
		clock_h += 2;
		f++;
		}
		if(f==0)
		{
		clock_h = 20;
		f++;
		}
	}
	/*
	if(GPIO_ReadInputDataBit(GPIOA, key_3)==1)
	{
		while(GPIO_ReadInputDataBit(GPIOA, key_3)==1);
		num = 3;
	}
	*/
	if(GPIO_ReadInputDataBit(GPIOB, key_4)==1)
	{
		delay_ms(200);
		while(GPIO_ReadInputDataBit(GPIOB, key_4)==1);
		num = 4;
		if(f==3)
		{
		clock_m += 4;
		f++;
		}
		if(f==2)
		{
		clock_m = 40;
		f++;
		}
		if(f==1)
		{
		clock_h += 4;
		f++;
		}
		if(f==0)
		{
		clock_h = 40;
		f++;
		}
	}
	

	if(GPIO_ReadInputDataBit(GPIOB, key_5)==1)
	{
		delay_ms(200);
		while(GPIO_ReadInputDataBit(GPIOB, key_5)==1);
		num = 5;
		if(f==3)
		{
		clock_m += 5;
		f++;
		}
		if(f==2)
		{
		clock_m = 50;
		f++;
		}
		if(f==1)
		{
		clock_h += 5;
		f++;
		}
		if(f==0)
		{
		clock_h = 50;
		f++;
		}
	}
	/*
	if(GPIO_ReadInputDataBit(GPIOA, key_6)==1)
	{
		while(GPIO_ReadInputDataBit(GPIOA, key_6)==1);
		num = 6;
	}
	*/
	if(GPIO_ReadInputDataBit(GPIOB, key_7)==1)
	{
		delay_ms(200);
		while(GPIO_ReadInputDataBit(GPIOB, key_7)==1);
		screen_flag++;
		if(screen_flag == 3)
		{
			screen_flag = 0;
		}
	}
  
	if(GPIO_ReadInputDataBit(GPIOB, key_8)==1)
	{
		delay_ms(200);
		while(GPIO_ReadInputDataBit(GPIOB, key_8)==1);
		if(f==3)
		{
		clock_m += 8;
		f++;
		}
		if(f==2)
		{
		return;
		}
		if(f==1)
		{
		clock_h += 8;
		f++;
		}
		if(f==0)
		{
		return;
		}
	}
	/*
	if(GPIO_ReadInputDataBit(GPIOB, key_9)==1)
	{
		while(GPIO_ReadInputDataBit(GPIOB, key_9)==1);
		num = 9;
	}
	*/
}

