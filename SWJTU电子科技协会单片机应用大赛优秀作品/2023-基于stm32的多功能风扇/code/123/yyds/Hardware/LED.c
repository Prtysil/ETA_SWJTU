#include "stm32f10x.h"                  // Device header

void LEDbz_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_0);
}

void BZ_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void BZ_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void LED_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void LED_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}

