#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "PWM.h"
#include "hc.h"
#include "ds18b20.h"

unsigned char check  = 1;//0:无烟雾报警，1:烟雾报警
char mode = 1;//0:手动调档，1:自动调档
float temperature = 24.5;
char speed = 50;//一共10档位
char keytimer10ms = 0;
int hctimer500ms = 0;
int temptimer250ms = 0;
char keyval = 0;
int length = 0;
char flag = 0;
char hcon = 0;
char bzflag = 0;
char keyflag = 0;
char page = 0;
int bztimer500ms = 0;
void oledshow1(void)//页面1
{
	OLED_ShowString(1, 1, "CHECK:");
	OLED_ShowNum(1, 7, check,1);
	OLED_ShowString(2, 1, "MODE:");
	OLED_ShowNum(2, 8, mode,1);
	OLED_ShowString(3, 1, "temperature:");
	OLED_ShowNum(3, 13, (int)temperature,2);
	OLED_ShowString(3, 15, ".");
	OLED_ShowNum(3, 16, (int)(temperature*10)%10,1);
	OLED_ShowString(4, 1, "speed:");
	OLED_ShowNum(4, 9, speed ,2);
}
void oledshow2(void)//页面2
{
	OLED_ShowString(2, 1, "distance:");
	OLED_ShowNum(2, 11, length,3);
	OLED_ShowString(2, 1, "on/off:");
	OLED_ShowNum(2, 9, hcon,1);
}

void oledshowcheck(void)//检测页面
{
	OLED_ShowString(1, 1, "CHECK:");
	OLED_ShowNum(1, 7, check,1);
	OLED_ShowString(2, 1, "on/off:");
	OLED_ShowNum(2, 9, flag,1);
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		keytimer10ms++;
		hctimer500ms++;
		temptimer250ms++;
		bztimer500ms++;
		if(bztimer500ms>=1000)
				bztimer500ms = 0;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


int main(void)
{
	OLED_Init();
	Hcsr04Init();
	Key_Init();
	LEDbz_Init();
	Motor_Init();
	PWM_Init();
	Timer2_1ms_Init();
	DS18B20_Init();
	Delay_ms (50);
	
	while (1)
	{
		if(keytimer10ms>=10)//每10ms执行一次
		{
			keytimer10ms = 0;
			keyval = keyscan();//按键扫描
			if(keyval == 1)//按键1换页
			{
				page ++;
				page = page %2;
			}
			else if(keyval == 4)//按键4模式改变
			{
				mode ++;
				mode = mode %2;
			}
			if(check == 0)//没有检测时候显示其他参数
			{
				if(page == 0)
					oledshow1 ();
				else 
					oledshow2 ();
			}
			else //检测的时候显示检测页面
				oledshowcheck();
			if(length <50&&check == 0)//没有检测的时候距离符合
			{
				hcon = 1;
				if(mode == 0)//手动
				{
					speed = speed /10;
					PWM_SetCompare1(speed*10);
					if(keyval==2)
					{
						if(speed > 1)//减档
							speed --;
					}
					else if(keyval ==3)//加档
					{
						if(speed <10)
							speed ++;
					}
				}
				else if(mode == 1)//温控模式
				{
					speed = (temperature-23)*5+50;
					PWM_SetCompare1(speed);
				}
				
			}
			else if(length  >=50&&check == 0)//超出距离
			{
				hcon = 0;
				PWM_SetCompare1(0);
			}
			else if(check == 1)//检测模式
			{
				oledshowcheck();
				flag = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
				if(flag)
					flag  =0;
				else
					flag = 1;
				
				if(flag)//超标
				{
					PWM_SetCompare1(70);
					LED_ON ();
					bzflag = 1;
					if(keyflag == 0)
					keyflag  = 1;
				}
				else
				{
					PWM_SetCompare1(0);
					bzflag = 0;
					keyflag = 0;
					LED_OFF ();
					BZ_OFF ();
				}
				if(bzflag == 1)
				{
					if(keyval == 2)//关闭
						keyflag ++;
				}
			}
		}
		if(bzflag&&keyflag==1)//没有按键关闭蜂鸣器
		{
			if(bztimer500ms<500)
				BZ_ON();
			else if(bztimer500ms<1000)
				BZ_OFF();
		}
		else
		{
			BZ_OFF ();
		}
		if(hctimer500ms>=500)//每隔500ms更新一次距离
		{
			hctimer500ms = 0;
			length  = Hcsr04GetLength();
		}
		if(temptimer250ms>=250)//每隔250ms更新一次温度
		{
			temptimer250ms = 0;
			temperature = DS18B20_GetTemperture();
		}
	}
}
