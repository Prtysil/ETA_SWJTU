#include "delay.h"
#include "DHT11.h"

//PB10 Data

/*
 * 函数名：DHT11_init
 * 描述  ：DHT11的初始化，只需修改.h文件的管脚
 * 作者  ：Q小鑫
 * 输入  ：无
 * 输出  ：无
 */

void DHT11_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	
	GPIO_SetBits(DHT11_PORT, DHT11_PIN);	
}

//将引脚设置为上拉输入模式
static void DHT11_DIR_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}

//将引脚设置为推挽输出模式
static void DHT11_DIR_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}

//从DHT11读取一个字节，MSB先行
static uint8_t DHT11_Read_Byte()
{
	uint8_t i,temp = 0;
	for(i=0;i<8;i++){
		while(DHT11_DATA_IN() == 0);//等待50us低电平结束
		
		//DHT11 以26-28us的高电平表示0，以70us高电平表示1
		//通过检测x us 后的电平即可区别这两个状态
		
		delay_us(40);//延时x us，这个延时需要大于数据0持续的时间即可
		
		if(DHT11_DATA_IN() == 1){
			while(DHT11_DATA_IN() == 1);//等待数据1的高电平结束
			temp|=(uint8_t)(0x01<<(7-i));//把第7-i位置1，MSB先行
		}
		else{
			temp&=(uint8_t)~(0x01<<(7-i));//把第7-i位置0，MSB先行
		}
		
	}
	return temp;
}

/*
 * 函数名：Read_DHT11
 * 描述  ：DHT11的数据读取，可以读取湿度和温度的数据
 * 作者  ：Q小鑫
 * 输入  ：输入一个结构体
 * 输出  ：输出湿度和温度的数据
 */
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{
	//输出模式
	DHT11_DIR_OUT();
	
	//主机拉低
	DHT11_DATA_OUT(0);
	
	//延时18ms
	delay_ms(18);
	
	//总线拉高，主机延时30us
	DHT11_DATA_OUT(1);
	delay_us(30);//延时30us
	
	//主机设为输入，判断从机响应信号
	DHT11_DIR_IN();
	
	//判断从机是否有低电平响应信号，如果不响应则跳出，响应则向下运行
	if(DHT11_DATA_IN() == 0){
		//循环遍历直到从机发出80us的低电平，标志信号结束
		while(DHT11_DATA_IN() == 0);
		
		//循环遍历直到从机发出80us的高电平，标志信号结束
		while(DHT11_DATA_IN() == 1);
		
		//开始接收数据
		DHT11_Data->humi_int = DHT11_Read_Byte();
		DHT11_Data->humi_deci = DHT11_Read_Byte();
		
		DHT11_Data->temp_int = DHT11_Read_Byte();
		DHT11_Data->temp_deci = DHT11_Read_Byte();
		
		DHT11_Data->check_sum = DHT11_Read_Byte();
		
		//读取结束，引脚改为输出模式
		DHT11_DIR_OUT();
		
		//主机拉高
		DHT11_DATA_OUT(1);
		
		//检查读取的数据是否正确
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return 1;
		else 
			return 0;			
	}
	else return 0;
}

