#include "stm32f10x.h"                  // Device header

void LightSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t LightSensor_Get1(void)
{
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11);
}
uint8_t LightSensor_Get2(void)
{
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
}
uint8_t LightSensor_Get3(void)
{
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12);
}

