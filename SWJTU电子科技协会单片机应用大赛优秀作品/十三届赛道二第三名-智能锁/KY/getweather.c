#include "stm32f10x.h"                  // Device header

/*获取天气*/
/*
如果今天有雨或雪，函数返回1
如果今天没有雨和雪，函数返回0
*/
uint8_t Getweather(void)
{
	uint8_t status = 0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);			

	status = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2);
	return status ;
}
