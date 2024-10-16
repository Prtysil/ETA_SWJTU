#include "stm32f10x.h"            
#include "OLED.h"
#include "Timer.h"
#include "PWM.h"
#include "Motor.h"
#include "MotorRun.h"
#include "sys.h"
#include "usmart.h"		
#include "delay.h"
#include "led.h"	
#include "pstwo.h"
#include "motor.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
//#include "Encoder.h"


 int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;
 
int main(void)
{
	
	
	
	Timer_Init();
	Motor_Init();
	PWM_Init();
	OLED_Init();
	Stm32_Clock_Init(72); //系统时钟设置
	delay_init(72);	     //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);
	delay_ms(1000);	//串口初始化为115200
	PS2_Init();		
	PS2_SetInit();	//驱动端口初始化
	delay_ms(1000);

	//Encoder_Init();
    

	
	
	while(1)
	{	
		delay_ms(50);
		PS2_LX=PS2_AnologData(PSS_LX);
		PS2_LY=PS2_AnologData(PSS_LY);
		PS2_RX=PS2_AnologData(PSS_RX);
		PS2_RY=PS2_AnologData(PSS_RY);
		PS2_KEY=PS2_DataKey();
	OLED_ShowNum(1,1,PS2_KEY,4);
		switch(PS2_KEY)
		{
			case 5:
				stop_1();    //前
				run_2(58);
				backrun_3(58);
				break;
			case 6:
				run_1(50);     //右
				backrun_2(100);
				backrun_3(100);
				break;
			case 7:
				stop_1();		//后
				backrun_2(58);
				run_3(58);
				break;
			case 8:
				backrun_1(50);   //左
				run_2(100);
				run_3(100);
				break;
			case 13: 
				run_1(58); 		//右前
				stop_2();
				backrun_3(58);
				break;
			case 14:        //右后
				run_1(58);
				backrun_2(58);
				stop_3();
				break;
			case 15:         //左后
				backrun_1(58);
				stop_2();
				run_3(58);
				break;
			case 16:          
				//左前
				backrun_1(58);
				run_2(58);
				stop_3();
				break;
			case 10: //正旋
				run_1(50);
				run_2(50);
				run_3(50);
				break;
			case 9: //逆旋
				backrun_1(50);
				backrun_2(50);
				backrun_3(50);
				break;
			case 0: stop_1();
					stop_2();
					stop_3();
					break;
				
		}
		OLED_ShowNum(2,1,4,4);
		delay_us(50);
	}
		
}

	
	
/*void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
        //每隔一段时间获取速度
		//Speed = Encoder_Get();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}*/

