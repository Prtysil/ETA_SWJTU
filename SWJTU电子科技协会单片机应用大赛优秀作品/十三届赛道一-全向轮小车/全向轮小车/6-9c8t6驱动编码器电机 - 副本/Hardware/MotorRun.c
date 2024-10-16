#include "stm32f10x.h"                  
#include "Motor.h"
#include "PWM.h"
	

//小车调速函数11111111111111111111111111111111111111111111111111
void Speed_Control_1(int8_t Compare)
{
	Compare-=100;			    //100->20000	
	PWM_SetCompare1(Compare); 	//调速
}
 
//小车前进函数
void run_1(int8_t Compare)
{
	moto_go_1();       		//右电机往前
	Speed_Control_1(Compare);
	
}
 
//小车后退函数
void backrun_1(int8_t Compare)
{
	moto_back_1();     		//右电机往后
	Speed_Control_1(Compare);
	
}
 
//小车停车函数
void stop_1(void)         		
{   	
	moto_stop_1();     		//右电机停止
}
//小车调速函数222222222222222222222222222222222222222222222222222
void Speed_Control_2(int8_t Compare)
{
	Compare*=0.01; 				//100->20000	
	PWM_SetCompare2(Compare); 	//调速
}
 
//小车前进函数
void run_2(int8_t Compare)
{
	moto_go_2();       		//右电机往前
	Speed_Control_2(Compare);
	
}
 
//小车后退函数
void backrun_2(int8_t Compare)
{
	moto_back_2();     		//右电机往后
	Speed_Control_2(Compare);
	
}
 
//小车停车函数
void stop_2(void)         		
{   	
	moto_stop_2();     		//右电机停止
}
//小车调速函数3333333333333333333333333333333333333333333333333333333
void Speed_Control_3(int8_t Compare)
{
	Compare*=0.01;				//100->20000	
	PWM_SetCompare3(Compare); 	//调速
}
 
//小车前进函数
void run_3(int8_t Compare)
{
	moto_go_3();       		//右电机往前
	Speed_Control_3(Compare);
	
}
 
//小车后退函数
void backrun_3(int8_t Compare)
{
	moto_back_3();     		//右电机往后
	Speed_Control_3(Compare);
	
}
 
//小车停车函数
void stop_3(void)         		
{   	
	moto_stop_3();     		//右电机停止
}
