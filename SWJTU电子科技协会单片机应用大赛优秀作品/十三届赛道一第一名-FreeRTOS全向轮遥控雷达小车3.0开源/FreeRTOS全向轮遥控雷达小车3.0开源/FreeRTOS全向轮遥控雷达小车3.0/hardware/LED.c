#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

void LED1_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void LED1_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	}
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
}

void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	}
}

void LED7_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}

void LED7_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_12);
}

void LED7_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_12);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	}
}

void LED8_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}

void LED8_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
}

void LED8_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_15) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_15);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_15);
	}
}

