#ifndef __DHT11_H__
#define __DHT11_H__
#include "stm32f10x.h"                  // Device header


//DHT11 DATA PB10

//设置DATA管脚！
#define DHT11_CLK  RCC_APB2Periph_GPIOA
#define DHT11_PIN  GPIO_Pin_5
#define DHT11_PORT GPIOA
//



//读取电平
#define DHT11_DATA_IN() GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)

//输出电平
#define DHT11_DATA_OUT(a) if(a) \
														GPIO_SetBits(DHT11_PORT,DHT11_PIN); \
                          else  \
														GPIO_ResetBits(DHT11_PORT,DHT11_PIN)

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
}DHT11_Data_TypeDef;

void DHT11_init(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);

#endif
