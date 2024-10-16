#include "stm32f10x.h"                  // Device header
#include "freertos.h"
#include  "task.h"
#include "usart.h"
#include "motor.h"
#include "timers.h"
#include "encoder.h"
#include "ESP8266.h"
#include "key.h"
#include "delay.h"
#include "pid.h"
#include "oled.h"
#include "pwm.h"
#include <stdlib.h>
#include "hcsr04.h"
#include "pwm.h"
#include "math.h"
#define pi 3.14
#define x 8
#define y 16


int keynum;
int mspeed=0,speed=0;
double angle;
int dis;
int16_t Speed_On;			//定义速度变量
int16_t Speed_Left;
int16_t Speed_Right;
int16_t Speed_Turn1;
int16_t Speed_Turn2;
int16_t Speed_Turn3;

int t;
extern uint64_t time;


TaskHandle_t serHandler;

TimerHandle_t Tim_speed;

TimerHandle_t Tim_servo;

TaskHandle_t starttaskHandler;//动态创建任务句柄
int list1[181];
int list2[181];
int min;

float returnp(int *list,int n)
{
	int k=0;
	min=list[0];
	
	int loc1=0;
	int loc2=0;

	for(k=1;k<n;k++)
	{
		
		
		if(list[k]<=510&&list[k-1]>510)
		{
			
			loc1=k;
		}
		if(list[k]>510&&list[k-1]<=510)
		{

			loc2=k;
		}
	}
	return (loc1+loc2)/2.0;
}
static void Tim_servo_ct(TimerHandle_t xTimer)
{
	time++;
}
static void Tim_speed_ct(TimerHandle_t xTimer)
{
	t++;
}
int loc1;
int loc2;

void ser(void * arg)
{
	Tim_servo=xTimerCreate("servo",1,pdTRUE,0,Tim_servo_ct);
	xTimerStart (Tim_servo, 0);
	Tim_speed=xTimerCreate("tim",20,pdTRUE,0,Tim_speed_ct);
	xTimerStart (Tim_speed, 0);
	int i;
	double ty;
	double tx;
	
	
	while(1)
	{		
		OLED_Clear();
		OLED_ShowString(0,0,"Mode1:REMOTE CONTROL",OLED_6X8);
		OLED_Update();
		
		speed_left(0);
		speed_right(0);
		speed_on(0);
		
		servo_init();
		Servo_SetAngle(90);
		delay_ms(20);
		
		ESP_Init();
		motor_init();
		//防止爆转
		Speed_Turn1=Speed_On=Encoder_Get_On();
		Speed_Turn3=Speed_Left=Encoder_Get_Left();
		Speed_Turn2=Speed_Right=Encoder_Get_Right();
		//防止乱动
		Serial_RxPacket[0]='l';
		t=0;
		while(1)
		{
			keynum=Key_GetNum();
			if(t==5)
			{	
			
				Speed_Turn1=Speed_On=Encoder_Get_On();
				Speed_Turn3=Speed_Left=Encoder_Get_Left();
				Speed_Turn2=Speed_Right=Encoder_Get_Right();
			
				if(Serial_GetRxFlag()==1)
				{
					if(Serial_RxPacket[0]=='s')
					{
						angle=atof(Serial_RxPacket+1);//取后面数字构成整型
						speed=10;
					}
					else if(Serial_RxPacket[0]=='r')
					{
						mspeed=atoi(Serial_RxPacket+1)-50;
					}
				}
				if(Serial_RxPacket[0]=='s')
				{		
					speedcontrol(speed,angle,Speed_On,Speed_Right,Speed_Left);
				}
				else if(Serial_RxPacket[0]=='r')
				{
					speedturn(mspeed/5,Speed_Turn1,Speed_Turn2,Speed_Turn3);
				}
				t=0;
			}
			if(keynum)
			{
				break;			
			}
		}
		
		
		
		servo_init();
		OLED_Clear();
		OLED_ShowString(0,0,"Mode 2:RADAR SCAN",OLED_6X8);
		OLED_ShowString(0,1*y,"Angle:",OLED_8X16);
		OLED_ShowString(0,2*y,"Dist_mm:",OLED_8X16);
		OLED_Update();
		Servo_SetAngle(0);
		delay_ms(50);
		for(i=0;i<=180;i+=1)
		{
			Servo_SetAngle(i);
			delay_ms(5);
			dis=sonar_mm();	
			list1[i]=dis;
			OLED_ShowNum(6*x,1*y,i,3,OLED_8X16);
			OLED_ShowNum(8*x,2*y,dis,5,OLED_8X16);
			OLED_Update();	
		}
		
		loc1=returnp(list1,181);		
		ty=pow((48+(min+20)*(min+20)+8*1.732*(min+20)*sin(loc1*pi/180.0)),0.5);
		tx=(min+20)*cos(loc1*pi/180.0);
		angle=acos(tx/ty)/pi*180;
		
		Servo_SetAngle(loc1);
	
		OLED_Clear();
		OLED_ShowNum(0*x,0*y,(loc1+loc2)/2,5,OLED_6X8);
		OLED_ShowNum(0,56,180,3,OLED_6X8);
		OLED_ShowNum(116,56,0,1,OLED_6X8);
		OLED_ShowNum(56,8,90,2,OLED_6X8);
		OLED_DrawCircle(63, 60, 45, OLED_UNFILLED);
		OLED_DrawCircle(63, 60, 22, OLED_UNFILLED);
		OLED_DrawLine(18,60,109,60);
		OLED_DrawLine(63,59,63,16);
		OLED_Update();
		
		for(i=0;i<=180;i++)
		{
			OLED_DrawPoint(63+list1[i]*cos(i)/10,63-list1[i]*sin(i)/10);	
		}
		OLED_Update();
		motor_init();
		delay_ms(1000);
		Speed_On=Encoder_Get_On();
		Speed_Left=Encoder_Get_Left();
		Speed_Right=Encoder_Get_Right();
		clear_on();
		clear_left();
		clear_right();
		t=0;
		while(1)
		{
			keynum=Key_GetNum();
			if(t==5)
			{
				Speed_On=Encoder_Get_On();
				Speed_Left=Encoder_Get_Left();
				Speed_Right=Encoder_Get_Right();
				dis=sonar_mm();
				if(dis<=100)	
				speed=0;
				else
				speed=20;
				speedcontrol(speed,450-(angle),Speed_On,Speed_Right,Speed_Left);
				t=0;
			}
			if(keynum)
			{
				break;
			}
		}
		
	}
}


void starttask( void * arg)
{
	xTaskCreate(ser,"ser",256,NULL,2,&serHandler);
	vTaskDelete(NULL);//必须加上
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	delay_init();
	
	OLED_Init();
	
	Encoder_Init1();//左
	Encoder_Init2();//上
	Encoder_Init3();//右
	
	Key_Init();
	
	HC_SR04_Init();	
	
	
	xTaskCreate(starttask,"starttask",512,NULL,2,&starttaskHandler);
	vTaskStartScheduler();
	while(1)
	{

	}
	
}

StaticTask_t IdleTaskTCB;

StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t 	** ppxIdleTaskTCBBuffer, 
									StackType_t 	** ppxIdleTaskStackBuffer, 
									uint32_t 		*pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer=&IdleTaskTCB;
	* ppxIdleTaskStackBuffer=IdleTaskStack;
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}

StaticTask_t TimerTaskTCB;

StackType_t TimerTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                         StackType_t ** ppxTimerTaskStackBuffer,
                                         uint32_t * pulTimerTaskStackSize )
{
	*ppxTimerTaskTCBBuffer=&TimerTaskTCB;
	* ppxTimerTaskStackBuffer=TimerTaskStack;
	*pulTimerTaskStackSize=configMINIMAL_STACK_SIZE;
	
}
