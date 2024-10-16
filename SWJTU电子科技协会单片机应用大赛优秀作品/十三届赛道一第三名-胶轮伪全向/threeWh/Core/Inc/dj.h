#ifndef __DJ_H
#define __DJ_H

#include "main.h"
#include "tim.h"
#include "stdint.h"
#include "stm32h7xx.h"
#include "PID.h"

#define DJ_1_TIM &htim8
#define DJ_1_MODEA_GPIOX GPIOB
#define DJ_1_MODEA_GPIO_PIN GPIO_PIN_0
#define DJ_1_MODEB_GPIOX GPIOB
#define DJ_1_MODEB_GPIO_PIN GPIO_PIN_1
#define DJ_1_ENCODE &htim5

#define DJ_2_TIM &htim8
#define DJ_2_MODEA_GPIOX GPIOE
#define DJ_2_MODEA_GPIO_PIN GPIO_PIN_7
#define DJ_2_MODEB_GPIOX GPIOE
#define DJ_2_MODEB_GPIO_PIN GPIO_PIN_8
#define DJ_2_ENCODE &htim2

#define DJ_3_TIM &htim8
#define DJ_3_MODEA_GPIOX GPIOB
#define DJ_3_MODEA_GPIO_PIN GPIO_PIN_10
#define DJ_3_MODEB_GPIOX GPIOB
#define DJ_3_MODEB_GPIO_PIN GPIO_PIN_11
#define DJ_3_ENCODE &htim3

#define DJ_PWM_CRR_MAX 1000
#define DJ_PWM_TIM &htim8
#define DJ_PWM_TIM_2 &htim1
#define DJ_1_PWM_CHANNEL TIM_CHANNEL_1
#define DJ_2_PWM_CHANNEL TIM_CHANNEL_2
#define DJ_3_PWM_CHANNEL TIM_CHANNEL_4

void DJ_All_Init();

/// @brief 切换转动方向
/// @param DJ 轮胎序号(1,2,3)
/// @param mode (0,1) 0:正转 1:反转
void DJ_Set_Mode(uint8_t DJ, uint8_t mode);

void DJ_Set_PWM(uint8_t DJ, uint16_t pwm);

int DJ_Get_Encoder(uint8_t DJ);

void DJ_Get_Encoder_All(int *A, int *B, int *C);

void DJ_Step();

void DJ_Set_Speed(uint8_t DJ, int speed);
void DJ_Set_Speed_All(int A, int B, int C);

int getSpeed(uint8_t DJ);
#endif // __DJ_H