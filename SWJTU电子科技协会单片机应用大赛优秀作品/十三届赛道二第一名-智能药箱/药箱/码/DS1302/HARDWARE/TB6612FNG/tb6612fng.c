#include "tb6612fng.h"

//=============================================================================
//文件名称：tb6612fng
//功能概要：MOTOR的IO口初始化  刹车  前进  后退  顺时针  逆时针
//参数说明：输入speed控制速度
//函数返回：无
//=============================================================================

//MOTOR的IO口初始化
void TB6612FNG_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(MOTOR1_GPIO_CLK|MOTOR3_GPIO_CLK,ENABLE);//时钟
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	
	//电机1
	GPIO_InitStructure.GPIO_Pin = MOTOR1_GPIO_PIN1|MOTOR1_GPIO_PIN2;
	GPIO_Init(MOTOR1_GPIO_PORT, &GPIO_InitStructure);
	//电机2
	GPIO_InitStructure.GPIO_Pin = MOTOR3_GPIO_PIN1|MOTOR3_GPIO_PIN2;
	GPIO_Init(MOTOR3_GPIO_PORT, &GPIO_InitStructure);
	//小车刹车
//	MOTOR_CarBrakeAll();
}

//小车停止0
void TB6612FNG_CarStop(void)
{
	MOTOR1_BRAKE;	//小车运动：电机1
	MOTOR3_BRAKE;	//小车运动：电机2
	
}

//小车前进1
void TB6612FNG_CarTurnfront(void)
{
	MOTOR1_CCW;	//小车运动：电机1
	MOTOR3_CCW;	//小车运动：电机2
	
}

//小车后退2
void TB6612FNG_CarTurnback(void) 								
{
	MOTOR1_CW;	//小车运动：电机1
	MOTOR3_CW;	//小车运动：电机2

}

//小车左转3
void TB6612FNG_CarTurnleft(void)
{
	MOTOR1_CCW; //小车运动：电机1
	MOTOR3_CCW; //小车运动：电机2
	
}

//小车右转4
void TB6612FNG_CarTurnright(void)
{
	MOTOR1_CCW;	//小车运动：电机1
	MOTOR3_CCW;	//小车运动：电机2
	
}

//小车逆时针5
void TB6612FNG_CarAnticlockwise(void)
{
	MOTOR1_CW; //小车运动：电机1
	MOTOR3_CCW; //小车运动：电机2
	
}

//小车顺时针6
void TB6612FNG_CarClockwise(void)
{
	MOTOR1_CCW;	//小车运动：电机1
	MOTOR3_CW;	//小车运动：电机2
	
}
