#include "stm32f10x.h"                  // Device header
#include "Delay.h"


#define DQ_H     GPIO_SetBits(GPIOB,GPIO_Pin_0)        			//PB0拉高
#define DQ_L     GPIO_ResetBits(GPIOB,GPIO_Pin_0)						//PB0拉低
#define DQ_Read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)    //PB0读取

uint8_t flag=0;

void DS18B20_Init(void) 																		//PB0初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;             //DQ=PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DS18B20_Output(void)      //输出模式            
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;             
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DS18B20_Input(void)      //输入模式
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;             
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t DS18B20_Reset(void)      //复位
{
	uint8_t flag;
	DS18B20_Output();
	DQ_H;
	Delay_us(5);
	
	DQ_L;
	Delay_us(480);
	DQ_H;
	Delay_us(60);
	DS18B20_Input();
	flag=DQ_Read;
	Delay_us(480);
	DS18B20_Output();
	DQ_H;
	return flag;
}

void DS18B20_WriteData(uint8_t data)     // 写数据
{
	for(uint8_t i=0;i<8;i++)
	{
		DS18B20_Output();  //输出状态
		DQ_L;
		Delay_us(2);
		if(data&0x01)     
		{
			DQ_H;
		}
		else
		{
			DQ_L;
		}
		Delay_us(60);
		DQ_H;
		data = data>>1;
	}
	
}

uint8_t DS18B20_ReadData(void)      //读数据
{
	uint8_t data =0;
	
	for(uint8_t i=0;i<8;i++)
	{
		data=data>>1;
		DS18B20_Output();   //输出状态
		DQ_L;
		Delay_us(2);
		DQ_H;
		Delay_us(2);
		DS18B20_Input();   //输入状态
		if(DQ_Read)
			data|=0x80;     //放入高位，再移位到低位，看上一节视频有详细讲解
		Delay_us(60);
	}
	return data;
}

uint16_t DS18B20_ReadTemp(void)   //读取温度
{
	uint8_t DL,DH;
	uint16_t data;	
	uint16_t Temperature=0;
	flag=0;
	DS18B20_Reset();              //复位
	DS18B20_WriteData(0XCC);      //跳过ROM检测
	DS18B20_WriteData(0X44);      //启动温度转换
	Delay_ms(750);                //延时，等待转换完成
	DS18B20_Reset();              //复位
	DS18B20_WriteData(0XCC);      //跳过ROM检测
	DS18B20_WriteData(0XBE);      //读取暂存器指令
	DL=DS18B20_ReadData();        //读温度低位
	DH=DS18B20_ReadData();        //读温度高位
	data=DH;
	data=data<<8;
	data|=DL;
	if((data&0XF800)==0XF800)   
	{
		data=~data+0X01;
		flag=1;
	}
		
	Temperature=data * 0.0625*10;
	return Temperature;
}

