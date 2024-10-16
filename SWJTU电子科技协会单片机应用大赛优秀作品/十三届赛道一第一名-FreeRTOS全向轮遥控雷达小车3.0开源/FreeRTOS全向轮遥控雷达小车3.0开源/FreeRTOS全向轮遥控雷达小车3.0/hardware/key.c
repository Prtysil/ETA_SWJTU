#include "stm32f10x.h"                  // Device header
#include "delay.h"


#define key1  GPIO_Pin_12

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = key1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	
	if (GPIO_ReadInputDataBit(GPIOA, key1) == 0)
	{
		delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, key1) == 0);
		delay_ms(20);
		KeyNum = 2;
	}
	
	return KeyNum;
}
