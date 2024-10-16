#include "HCSR04.h"

uint16_t Time;

void HCSR04_Init(void)
{
	HAL_TIM_Base_Stop(&htim6);
}

//void HCSR04_Strat(void)
//{
//	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
//	HAL_Delay(1);
//	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
//}

//uint16_t HCSR04_GetValue(void)
//{
//	HCSR04_Strat();
//	while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)==GPIO_PIN_RESET);
//	__HAL_TIM_SetCounter(&htim6,0);
//	HAL_TIM_Base_Start(&htim6);
//	while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)==GPIO_PIN_SET);
//	Time=__HAL_TIM_GetCounter(&htim6);
//	HAL_TIM_Base_Stop(&htim6);
//	return ((Time*0.0001)*31400)/2;
//	
//}
void HCSR04_Start(void)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
}

uint16_t HCSR04_GetValue(void)
{
	HCSR04_Start();
	return ((Time*0.0001)*31400)/2;
}
