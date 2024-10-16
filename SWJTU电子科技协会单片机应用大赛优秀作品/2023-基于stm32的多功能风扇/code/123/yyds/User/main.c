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
char speed = 5;//一共10档位
char keytimer10ms = 0;
int hctimer500ms = 0;
int temptimer250ms = 0;
char keyval = 0;
int length = 0;
float temp[21] = {24.5,24.7,25.2,25.7,26.8,27.5,27.9,28.4,28.9,29.3,29.5,29.5,29.6,29.1,28.5,28.1,27.5,27.0,26.7,26.7,26.7};
char tempi = 0;
char flag = 0;
	char bzflag = 0;
	char keyflag = 0;
int bztimer500ms = 0;
void oledshow(void)
{
	OLED_ShowString(1, 1, "CHECK:");
	OLED_ShowNum(1, 7, check,1);
//	OLED_ShowString(2, 1, "MODE:");
//	OLED_ShowNum(2, 8, mode,1);
//	OLED_ShowString(3, 1, "temperature:");
//	OLED_ShowNum(3, 13, (int)temperature,2);
//	OLED_ShowString(3, 15, ".");
//	OLED_ShowNum(3, 16, (int)(temperature*10)%10,1);
//	OLED_ShowString(4, 1, "speed:");
//	OLED_ShowNum(4, 9, speed ,2);
//	OLED_ShowString(1, 1, "CHECK:");
//	OLED_ShowNum(1, 7, check,1);
//	OLED_ShowString(1, 1, "MODE:");
//	OLED_ShowNum(1, 8, mode,1);
//	OLED_ShowString(2, 1, "distance:");
//	OLED_ShowNum(2, 11, length,3);
	OLED_ShowString(2, 1, "on/off:");
	OLED_ShowNum(2, 9, flag,1);
//	OLED_ShowString(3, 1, "temperature:");
//	OLED_ShowNum(3, 13, (int)temperature,2);
//	OLED_ShowString(3, 15, ".");
//	OLED_ShowNum(3, 16, (int)(temperature*10)%10,1);
//	OLED_ShowString(4, 1, "speed:");
//	OLED_ShowNum(4, 9, speed ,2);
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
//		PWM_SetCompare1(50);
		if(keytimer10ms>=10)
		{
			keytimer10ms = 0;
			
			oledshow();
			keyval = keyscan();
			if(keyval==2)
			{
				if(speed > 1)
					speed --;
				keyflag ++;
			}
			else if(keyval ==3)
			{
				if(speed <10)
					speed ++;
			}
			speed = (temperature-23)*5+50;
			
			if(bzflag == 1)
			{
				if(keyval == 2)
					keyflag ++;
			}
		}
		if(bzflag&&keyflag==1)
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
		if(hctimer500ms>=500)
		{
			flag = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
			if(flag)
				flag  =0;
			else
				flag = 1;
			
			if(flag)
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
			hctimer500ms = 0;
			length  = Hcsr04GetLength();
		}
		if(temptimer250ms>=500)
		{
			temptimer250ms = 0;
			tempi++;
			if(tempi>20)
				tempi = 0;
			temperature = temp[tempi];
		}
//		LED_ON();
//		BZ_ON();
	}
}
