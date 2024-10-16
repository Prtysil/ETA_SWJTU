#include "stm32f10x.h"                  // Device header

#include "pwm.h"
#include "tb6612fng.h"

//=============================================================================
//文件名称：pwm
//功能概要：PWM的IO口初始化   PWM的定时器的初始化   PWM初始化
//参数说明：输入speed控制速度
//函数返回：无
//=============================================================================

//PWM的IO口初始化
static void PWM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(PWM_TIM_CH1_GPIO_CLK|PWM_TIM_CH2_GPIO_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //推挽复用
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	// TIM2 输出比较通道1 GPIO 初始化
  GPIO_InitStructure.GPIO_Pin = PWM_TIM_CH1_PIN;
  GPIO_Init(PWM_TIM_CH1_PORT, &GPIO_InitStructure);
	// TIM2 输出比较通道2 GPIO 初始化
  GPIO_InitStructure.GPIO_Pin = PWM_TIM_CH2_PIN;
  GPIO_Init(PWM_TIM_CH2_PORT, &GPIO_InitStructure);


//PWM的定时器的初始化
static void PWM_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
	// 开启定时器时钟,即内部时钟CK_INT=72M
	PWM_TIM_APBxClock_Cmd(PWM_TIM_CLK, ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	// 配置周期
	TIM_TimeBaseStructure.TIM_Period=PWM_TIM_ARR;									// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Prescaler= PWM_TIM_PSC;							// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;					// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;								// 重复计数器的值，没用到不用管
	TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseStructure);						// 初始化定时器
	
	/*--------------------输出比较结构体初始化-------------------*/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							// 配置为PWM模式：PWM模式，低于ARR有效
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 比较输出使能：使能，输出需要PWM
	
	// 输出比较通道 1
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			// 输出通道电平极性配置：高	
	TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(PWM_TIM, TIM_OCPreload_Enable); 					//使能TIM14在CCR1上的预装载寄存器
	
	// 输出比较通道 2
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			// 输出通道电平极性配置：高
	TIM_OC2Init(PWM_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);					//使能TIM14在CCR1上的预装载寄存器
	
	
	// 使能计数器
	TIM_Cmd(PWM_TIM, ENABLE);

}

//PWM初始化
void PWM_Init(void)
{
	PWM_GPIO_Init();
	PWM_TIM_Init();
}

//小车走
void PWM_CarGo(u16 motor1,u16 motor3)
{

	TIM_SetCompare2(PWM_TIM,motor3);
	TIM_SetCompare1(PWM_TIM,motor1);
}
