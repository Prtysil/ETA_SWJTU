#ifndef _UT_H_
#define _UT_H_

#include "gpio.h"
#include "tim.h"
#include "motor.h"
#include "FreeRTOS.h"
#include "task.h"


extern int time;//us计时时间
extern float distance;//距离
extern float distances[41];//距离数组
extern float smallest;//最小值
extern float angle;//最小值的角度
extern uint16_t angle_val;
extern uint8_t servo_flag;


void trigger_start(void);
void Servo_Set(void);
void Servo_Init(void);
void UT_Test(void);
void Buzz_Start(void);
void UT_Task(void*argument);
#endif // !_UT_H_
