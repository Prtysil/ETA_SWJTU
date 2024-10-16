#ifndef __TB6612FNG_H
#define __TB6612FNG_H

#include "stm32f10x.h"

/*-------------------------------------------------------------------------------
小车四轮驱动
电机1：左上 PA0 PA1		电机3：右上 PA5 PA6
STBY1：		   PA2			STBY2：		   PA7
电机2：左下 PA3 PA4		电机4：右下 PB0 PB1
BrakeAll：刹车   Go：前进   BackGo：后退   CW：顺时针   CCW：逆时针   BRAKE：刹车

---------------------------------------------------------------------------------*/

//电机1
#define  MOTOR1_GPIO_CLK   RCC_APB2Periph_GPIOA
#define  MOTOR1_GPIO_PORT  GPIOA
#define  MOTOR1_GPIO_PIN1  GPIO_Pin_7
#define  MOTOR1_GPIO_PIN2  GPIO_Pin_5
#define  MOTOR1_CW				 {GPIO_ResetBits(MOTOR1_GPIO_PORT,MOTOR1_GPIO_PIN1);GPIO_SetBits(MOTOR1_GPIO_PORT,MOTOR1_GPIO_PIN2);}//顺时针：IN1=0，IN2=1
#define  MOTOR1_CCW				 {GPIO_SetBits(MOTOR1_GPIO_PORT,MOTOR1_GPIO_PIN1);GPIO_ResetBits(MOTOR1_GPIO_PORT,MOTOR1_GPIO_PIN2);}//逆时针：IN1=1，IN2=0
#define  MOTOR1_BRAKE			 GPIO_ResetBits(MOTOR1_GPIO_PORT,MOTOR1_GPIO_PIN1|MOTOR1_GPIO_PIN2);//刹车
//STBY1
/*#define  STBY1_GPIO_CLK    RCC_APB2Periph_GPIOA
#define  STBY1_GPIO_PORT 	 GPIOA
#define  STBY1_GPIO_PIN  	 GPIO_Pin_2
#define  STBY1_High				 GPIO_SetBits(STBY1_GPIO_PORT,STBY1_GPIO_PIN);	//STBY=1
#define  STBY1_Low				 GPIO_ResetBits(STBY1_GPIO_PORT,STBY1_GPIO_PIN);//STBY=0*/
/*//电机2
#define  MOTOR2_GPIO_CLK   RCC_APB2Periph_GPIOA
#define  MOTOR2_GPIO_PORT  GPIOA
#define  MOTOR2_GPIO_PIN1	 GPIO_Pin_6
#define  MOTOR2_GPIO_PIN2	 GPIO_Pin_4
#define  MOTOR2_CW				 {GPIO_ResetBits(MOTOR2_GPIO_PORT,MOTOR2_GPIO_PIN1);GPIO_SetBits(MOTOR2_GPIO_PORT,MOTOR2_GPIO_PIN2);}//顺时针：IN1=0，IN2=1
#define  MOTOR2_CCW				 {GPIO_SetBits(MOTOR2_GPIO_PORT,MOTOR2_GPIO_PIN1);GPIO_ResetBits(MOTOR2_GPIO_PORT,MOTOR2_GPIO_PIN2);}//逆时针：IN1=1，IN2=0
#define  MOTOR2_BRAKE			 GPIO_ResetBits(MOTOR2_GPIO_PORT,MOTOR2_GPIO_PIN1|MOTOR2_GPIO_PIN2);*///刹车

//电机3
#define  MOTOR3_GPIO_CLK   RCC_APB2Periph_GPIOA
#define  MOTOR3_GPIO_PORT  GPIOA
#define  MOTOR3_GPIO_PIN1	 GPIO_Pin_6
#define  MOTOR3_GPIO_PIN2	 GPIO_Pin_4
#define  MOTOR3_CW				 {GPIO_ResetBits(MOTOR3_GPIO_PORT,MOTOR3_GPIO_PIN1);GPIO_SetBits(MOTOR3_GPIO_PORT,MOTOR3_GPIO_PIN2);}//顺时针：IN1=0，IN2=1
#define  MOTOR3_CCW				 {GPIO_SetBits(MOTOR3_GPIO_PORT,MOTOR3_GPIO_PIN1);GPIO_ResetBits(MOTOR3_GPIO_PORT,MOTOR3_GPIO_PIN2);}//逆时针：IN1=1，IN2=0
#define  MOTOR3_BRAKE			 GPIO_ResetBits(MOTOR3_GPIO_PORT,MOTOR3_GPIO_PIN1|MOTOR3_GPIO_PIN2);//刹车
/*//STBY2
#define  STBY2_GPIO_CLK    RCC_APB2Periph_GPIOA
#define  STBY2_GPIO_PORT 	 GPIOA
#define  STBY2_GPIO_PIN  	 GPIO_Pin_7
#define  STBY2_High				 GPIO_SetBits(STBY2_GPIO_PORT,STBY2_GPIO_PIN);	//STBY=1
#define  STBY2_Low				 GPIO_ResetBits(STBY2_GPIO_PORT,STBY2_GPIO_PIN);//STBY=0*/
//电机4
/*#define  MOTOR4_GPIO_CLK   RCC_APB2Periph_GPIOB
#define  MOTOR4_GPIO_PORT  GPIOB
#define  MOTOR4_GPIO_PIN1  GPIO_Pin_0
#define  MOTOR4_GPIO_PIN2  GPIO_Pin_1
#define  MOTOR4_CW				 {GPIO_ResetBits(MOTOR4_GPIO_PORT,MOTOR4_GPIO_PIN1);GPIO_SetBits(MOTOR4_GPIO_PORT,MOTOR4_GPIO_PIN2);}//顺时针：IN1=0，IN2=1
#define  MOTOR4_CCW				 {GPIO_SetBits(MOTOR4_GPIO_PORT,MOTOR4_GPIO_PIN1);GPIO_ResetBits(MOTOR4_GPIO_PORT,MOTOR4_GPIO_PIN2);}//逆时针：IN1=1，IN2=0
#define  MOTOR4_BRAKE			 GPIO_ResetBits(MOTOR4_GPIO_PORT,MOTOR4_GPIO_PIN1|MOTOR4_GPIO_PIN2);*///刹车
/**************************** 函数声明 ****************************/

void TB6612FNG_GPIO_Init(void);     //电机GPIO设置

void TB6612FNG_CarStop(void);											//小车停止0
void TB6612FNG_CarTurnfront(void); 								//小车前进1
void TB6612FNG_CarTurnback(void);  								//小车后退2
void TB6612FNG_CarTurnleft(void);									//小车左转3
void TB6612FNG_CarTurnright(void);								//小车右转4
void TB6612FNG_CarLeftaround(void);								//小车左退5
void TB6612FNG_CarRightaround(void);							//小车右退6
void TB6612FNG_CarAnticlockwise(void);						//小车逆时针7
void TB6612FNG_CarClockwise(void);								//小车顺时针8

#endif
