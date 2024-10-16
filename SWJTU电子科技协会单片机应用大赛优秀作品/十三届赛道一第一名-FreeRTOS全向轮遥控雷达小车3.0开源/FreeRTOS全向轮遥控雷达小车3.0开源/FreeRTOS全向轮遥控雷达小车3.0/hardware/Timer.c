#include "stm32f10x.h"                  // Device header

void Timer3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//启用TIM3时钟
																
	TIM_InternalClockConfig(TIM3);								//设置TIM3使用内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;			//定义结构体，配置定时器
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//设置1分频（不分频）
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//设置计数模式为向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;			//设置最大计数值，达到最大值触发更新事件，因为从0开始计数，所以计数10次是10-1,每10微秒触发一次
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;			//设置时钟预分频，72-1就是每 时钟频率(72Mhz)/72=1000000 个时钟周期计数器加1,每1微秒+1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;		//重复计数器（高级定时器才有，所以设置0）
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);			//初始化TIM3定时器
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);			//清除更新中断标志位
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);		//开启更新中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//设置中断优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体，配置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//指定中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//中断使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//设置响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);							//开启定时器
}

/*
void TIM3_IRQHandler(void)			//更新中断函数
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)		//获取TIM3定时器的更新中断标志位
	{
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			//清除更新中断标志位
	}
}*/

void Timer4_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//启用TIM3时钟
																
	TIM_InternalClockConfig(TIM4);								//设置TIM3使用内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;			//定义结构体，配置定时器
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//设置1分频（不分频）
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//设置计数模式为向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;			//设置最大计数值，达到最大值触发更新事件，因为从0开始计数，所以计数10次是10-1,每10微秒触发一次
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;			//设置时钟预分频，72-1就是每 时钟频率(72Mhz)/72=1000000 个时钟周期计数器加1,每1微秒+1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;		//重复计数器（高级定时器才有，所以设置0）
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);			//初始化TIM3定时器
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);			//清除更新中断标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);		//开启更新中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//设置中断优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体，配置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//指定中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//中断使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//设置响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4, ENABLE);							//开启定时器
}

/*
void TIM4_IRQHandler(void)			//更新中断函数
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)		//获取TIM4定时器的更新中断标志位
	{
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);			//清除更新中断标志位
	}
}*/

