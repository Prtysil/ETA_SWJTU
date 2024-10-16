#include "BEEP.h"

void BEEP_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BEEP_RCC, ENABLE);	 //使能端口时钟

	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;	 //设置输出引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);	 //初始化GPIO
}

