#include "stm32f10x.h"             
#include "PWM.h"
 
//左右电机初始化
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//电机控制模拟输入端
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//控制电机->PA4 PA5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_3| GPIO_Pin_2| GPIO_Pin_1| GPIO_Pin_0;//PWM1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;//PWM2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;//PWM3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	*/
	PWM_Init();
}
 
/*--------------------------------------------一号轮子控制----------------*/
//电机向前
void moto_go_1(void)
{
	//正转
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}
 
//电机向后
void moto_back_1(void)
{
	//反转
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
}
 
//电机停止
void moto_stop_1(void)
{
	//停车
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}
/*--------------------------------------------二号轮子控制----------------*/
void moto_go_2(void)
{
	//正转
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}
 
//电机向后
void moto_back_2(void)
{
	//反转
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
}
 
//电机停止
void moto_stop_2(void)
{
	//停车
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}
/*--------------------------------------------三号轮子控制----------------*/
void moto_go_3(void)
{
	//正转
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}
 
//电机向后
void moto_back_3(void)
{
	//反转
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}
 
//电机停止
void moto_stop_3(void)
{
	//停车
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

