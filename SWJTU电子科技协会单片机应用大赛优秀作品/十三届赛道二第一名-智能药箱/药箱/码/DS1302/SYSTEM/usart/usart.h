#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"                  // Device header

#include "stdio.h"	
#include "sys.h" 

///*-------------------------------------------------------------------------------
//RXD→PB10(USART3_TX)
//TXD→PB11(USART3_RX)
//---------------------------------------------------------------------------------*/

//USART
#define		USART_ClockCmd			RCC_APB1PeriphClockCmd
#define		USART_CLK  					RCC_APB1Periph_USART3
#define		USART		  					USART3
#define		USART_IRQHandler		USART3_IRQHandler
#define		USART_IRQn  				USART3_IRQn
//TX、RX
#define		USART_GPIO_CLK     	RCC_APB2Periph_GPIOB
#define		USART_GPIO_PORT  		GPIOB
#define		USART_GPIO_PIN_TX  	GPIO_Pin_10
#define		USART_GPIO_PIN_RX  	GPIO_Pin_11

#define		USART_REC_LEN  			200  	//定义最大接收字节数 200
#define		EN_USART_RX 				1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         				//接收状态标记	

/**************************** 函数声明 ****************************/

void uart_init(u32 bound);

#endif

