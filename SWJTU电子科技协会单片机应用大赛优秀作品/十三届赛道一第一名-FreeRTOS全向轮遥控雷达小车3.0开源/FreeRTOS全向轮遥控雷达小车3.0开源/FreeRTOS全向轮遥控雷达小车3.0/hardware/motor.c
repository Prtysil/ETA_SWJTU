#include "stm32f10x.h"                  // Device header
#include "PWM.h"  
#include <math.h>
#include "pid.h"
#include "encoder.h"
#include "delay.h"
#define pi 3.14
float speedtable[]={  1,    0,
					-0.5,-0.866,
					-0.5,0.866};
void motor_init(void)
{
	PWM_init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	
	
}

void speed_right(int16_t speed)  //-100<speed<100
{
	if(speed>0)	
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		PWM_SetCompare1(speed);
	}
	else if(speed<0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		PWM_SetCompare1(-speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
		PWM_SetCompare1(speed);
	}
}


void speed_left(int16_t speed)
{
	if(speed<0)	
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_SetBits(GPIOB,GPIO_Pin_9);
		PWM_SetCompare3(-speed);
	}
	else if(speed>0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_8);
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
		PWM_SetCompare3(speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
		PWM_SetCompare3(speed);
	}
}

void speed_on(int16_t speed)
{
	if(speed<0)	
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		PWM_SetCompare2(-speed);
	}
	else if(speed>0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		PWM_SetCompare2(speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
		PWM_SetCompare2(speed);
	}
}
float vx;
float vy;
float x;
float y;
float z;
int s1;
int s2;
int s3;
int sa;
int sb;
int sc;
void speedcontrol(int speed,double angle,int so,int sr,int sl)
{
	vx=speed*sin(angle*pi/180.0);
	vy=speed*cos(angle*pi/180.0);
	x=vx*speedtable[0]+vy*speedtable[1];
	y=vx*speedtable[2]+vy*speedtable[3];
	z=vx*speedtable[4]+vy*speedtable[5];
	s1=pid_on(so,-x);
    s2=pid_right(sr,-y);
	s3=pid_left(sl,-z);
	speed_on(s1);
	speed_right(s2);
	speed_left(s3);
}
void speedturn(int tar,int re1,int re2,int re3)
{
	sa=pid_turn1(re1,tar);
	sb=pid_turn2(re2,tar);
	sc=pid_turn3(re3,tar);
	speed_on(sa);
	speed_right(sb);
	speed_left(sc);
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare4(Angle/180*2000+500);
}
