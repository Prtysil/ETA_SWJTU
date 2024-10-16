#include "stm32f10x.h"                  // Device header
#include  <stdio.h>
#include <stdarg.h>

/*简介：本串口配置了USART1 用于进行蓝牙模块的数据接收
*/

uint8_t Serial_RxData;
uint8_t Serial_Rxflag;
/*
#brief USART初始化函数
#para  无
#return  无
*/
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); // 开启USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //开启GPIOA 时钟
	//配置TX引脚
	GPIO_InitTypeDef GPIO_Structure1; 
		GPIO_Structure1.GPIO_Mode = GPIO_Mode_AF_PP; //复用输出模式
	GPIO_Structure1.GPIO_Pin = GPIO_Pin_9;
	GPIO_Structure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Structure1);
	//配置RX引脚	
	GPIO_InitTypeDef GPIO_Structure2; 
	GPIO_Structure2.GPIO_Mode = GPIO_Mode_IPU;//上拉输入模式
	GPIO_Structure2.GPIO_Pin = GPIO_Pin_10;
	GPIO_Structure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Structure2);	
	//USART初始化
	USART_InitTypeDef USART_InitSturcture;
	USART_InitSturcture.USART_BaudRate = 9600;  //配置波特率
	USART_InitSturcture.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用流控制
	USART_InitSturcture.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; //配置输入和输出功能
	USART_InitSturcture.USART_Parity = USART_Parity_No; //不适用奇偶校验
	USART_InitSturcture.USART_StopBits = USART_StopBits_1; //停止位长度为1
	USART_InitSturcture.USART_WordLength  = USART_WordLength_8b; //数据长度为八位
	USART_Init(USART1,&USART_InitSturcture);
	//使能串口中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//配置中断的NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_Initstructure;
	NVIC_Initstructure.NVIC_IRQChannel =  USART1_IRQn ;
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_Initstructure);
	USART_Cmd(USART1,ENABLE);

	/*
	编码经验小结：
	关于串口接收数据，可以采用查询和中断，对于本项目而言，如果直接使用查询
	即如果 
			USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET
	就直接读取数据进行显示，由于为止原因，读数变量temp无法接收到数据
	但是利用中断接收以及自设标志位的结合，temp可以顺利接收到数据
			
	我猜测，查询接收在本项目中无法正常进行的原因可能是因为GPIOA配置了PWM干扰
	此猜测可以通过修改通信串口进行验证，再次搁置，希望日后可以记得并实践
	*/

}



/*brief 串口接收数据中断函数
**para
**return
*/
void   USART1_IRQHandler(void)
{
	
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_Rxflag = 1;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}

}
//自设置标志位
uint8_t Serial_GetRxFlag(void)
{
	if(Serial_Rxflag ==1)
	{
		Serial_Rxflag = 0;
		return 1;
	}
	return 0;
}
//接收单字节数据函数
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData ;
}
	



