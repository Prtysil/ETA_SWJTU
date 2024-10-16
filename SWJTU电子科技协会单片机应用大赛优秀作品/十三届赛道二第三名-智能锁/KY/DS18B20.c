#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/*硬件接线：数据线接在PB15上*/

//配置函数，将接口配置为推挽输出模式
void DS_PPOUT(void)
{
	//配置GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//数据线（单总线）配置为PB15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//配置函数：将接口配置为浮空输入模式
void DS_FIN(void)
{
	//配置GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//数据线（单总线）配置为PB15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//初始化函数
void Init_DS1B20(void)
{
	uint8_t t = 0;//循环辅助变量
	DS_PPOUT();
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	Delay_us(600);//延时>480us
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	Delay_us(50); //等待15-60us
	DS_FIN();
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==SET)
	{
		Delay_us(1);
		t++;
		if(t>=240)//如果超过240us数据线没有被拉低，说明ds18b20丢失了连接
		  {return;}
	}
	t = 0;
	
	Delay_us(240);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	Delay_us(240);
}
//单总线发送一位0
void DS_Writebit_0(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	Delay_us(60);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	Delay_us(10);
	
}
//单总线发送一位1
void DS_Writebit_1(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	Delay_us(10);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	Delay_us(60);
	
}
//单总线读取一位
uint8_t DS_Readbit(void)
{
	uint8_t temp;
	DS_PPOUT();
	GPIO_ResetBits(GPIOB ,GPIO_Pin_15);
	Delay_us(5);
	DS_FIN();
	Delay_us(5);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == SET)
	{
		temp = 1;
	}
	else
	{
		temp = 0;
	}
	Delay_us(50);
	return  temp;
}
//单总线发送一个字节
void DS_Writebyte(uint8_t Data)
{
	uint8_t i = 0;
	DS_PPOUT();
	for(i=0;i<8;i++)
	{
		if((Data&0x01) == 1)
		{
			DS_Writebit_1();
		}
		else
		{
			DS_Writebit_0();
		}
		Data >>=1;
	}
}
//单总线读取1个字节
uint8_t DS_Readbyte(void)
{
	uint8_t i,Data = 0;
	
	for(i = 0;i<8;i++)
	{
		Data = Data|(DS_Readbit()<<i);
	}
	return Data;
}

//配置温度转化定时器初始化
void Tim3_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3的时钟
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM3);		//选择TIM3为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10 -1;				//计数周期，即ARR的值,这里配置为1ms中断一次
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);				//将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元	
	
	/*中断输出配置*/
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);						//清除定时器更新标志位
																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
																//若不清除此标志位，则开启中断后，会立刻进入一次中断
																//如果不介意此问题，则不清除此标志位也可
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);					//开启TIM2的更新中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//选择配置NVIC的TIM3线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//指定NVIC线路的响应优先级为2
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE);			//使能TIM3，定时器开始运行
}

//温度转化函数
void Convert_Tempreture(void)
{
	//开始温度转化
	Init_DS1B20();
	DS_Writebyte(0xcc); //跳过rom
	DS_Writebyte(0x44); //启动温度转化
}
//DS18B20读取温度
float DS_ReadTempreture(void)
{
	uint8_t dl =0;
	uint8_t dh = 0;
	uint16_t  temp;
	//读取温度
	Init_DS1B20();
	DS_Writebyte(0xcc);
	DS_Writebyte(0xbe);
	dl= DS_Readbyte();
	dh= DS_Readbyte();
	
	temp = (dh<<8)|dl;
	
	return temp/16.0;
}

void DS_Configure(void)
{
	Init_DS1B20();
	DS_Writebyte(0xcc);
	DS_Writebyte(0x4e);
	DS_Writebyte(0x50);
	DS_Writebyte(0x05);
	DS_Writebyte(0x7f);
	
}



