//#include "hc.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/*
	gpiob P5,echo 10
	gpiob P6,trig 11
	使用了TIM2，分组2，优先级1,1
	
*/
u8 msHcCount = 0;//ms计数

static void NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStruct;
	//设置中断组为2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//设置中断来源
	NVIC_InitStruct.NVIC_IRQChannel	= TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	//设置主优先级
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;
	//设置次优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority				= 1;
	//初始化
	NVIC_Init(&NVIC_InitStruct);
	
}

void Hcsr04Init()
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //生成用于定时器设置的结构体
    GPIO_InitTypeDef GPIO_InitStructure;
	/*开启GPIOB时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
     
        //IO初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;   //发送电平引脚TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置推挽输出
    GPIO_Init(GPIOB , &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_11);//一开始低电平
    
    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_10 ;     //返回电平引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_10); //默认低电平   
    
     //定时器初始化 使用基本定时器TIM2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //使能对应RCC时钟
      //配置定时器基础结构体
    TIM_DeInit(TIM3);
     //自动重装载值寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		TIM_TimeBaseStructure.TIM_Period			= 1000;//设置周期为1000us
		//时钟预分频数
		TIM_TimeBaseStructure.TIM_Prescaler		= 72-1;//分频数72
	
		//时钟分频因子
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		//计数器计数模式，设置向上计数，
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
		//重复计数器的值
		//	TIM_TimeBaseStructure.TIM_RepetitionCounter=0 ;
    TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位         
        
   // TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);   //清除更新中断，免得一打开中断立即产生中断
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update); //清除更新中断，免得一打开中断立即产生中断
    TIM_ITConfig(TIM3 ,TIM_IT_Update,ENABLE);    //打开定时器更新中断
		NVIC_Config();
   TIM_Cmd(TIM3 ,DISABLE);     
}

void initHcsr04()
{
	Hcsr04Init();
	//BASIC_TIM_NVIC_Config();
}


static void OpenTimer()        //打开定时器
{
        //	/*清除计数器*/
	TIM_SetCounter(TIM3 ,0);
	msHcCount = 0;
	TIM_Cmd(TIM3 ,ENABLE);//使能定时器
}
 
static void CloseTimer()        //关闭定时器
{
       //	/*关闭计数器使能*/
	TIM_Cmd(TIM3 ,DISABLE);
}
 

//定时器2中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
        //	/*判断中断手否真的产生*/
	if(TIM_GetITStatus(TIM3 ,TIM_IT_Update) != RESET){
		/*清除更新中断标志位*/
		TIM_ClearITPendingBit(TIM3 ,TIM_IT_Update);
		msHcCount++;
	}
	
}

//获取定时器时间
u32 GetEchoTimer(void)
{
   u32 time = 0;
	/*//当回响信号很长是，计数值溢出后重复计数，overCount用中断来保存溢出次数*/
	time = msHcCount*1000;//overCount每++一次，代表overCount毫秒，time微妙
	time += TIM_GetCounter(TIM3);//获取计TIM2数寄存器中的计数值，一边计算回响信号时间
	TIM6->CNT = 0;  //将TIM2计数寄存器的计数值清零
	Delay_ms(50);
	return time;
 
}
float Hcsr04GetLength(void)
{
	/*测5次数据计算一次平均值*/
	float length = 0;
	float t = 0;
	float sum = 0;
	u16  	i = 0;
	while(i != 5){
		GPIO_SetBits(GPIOB ,GPIO_Pin_11 );//trig拉高信号，发出高电平
		Delay_us(20);//持续时间超过10us
		GPIO_ResetBits(GPIOB ,GPIO_Pin_11 );
		/*Echo发出信号 等待回响信号*/
		/*输入方波后，模块会自动发射8个40KHz的声波，与此同时回波引脚（echo）端的电平会由0变为1；
		（此时应该启动定时器计时）；当超声波返回被模块接收到时，回波引 脚端的电平会由1变为0；
		（此时应该停止定时器计数），定时器记下的这个时间即为
			超声波由发射到返回的总时长；*/
		while(GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_10) == 0);//echo等待回响
		/*开启定时器*/
		OpenTimer();
		i = i+1; //每收到一次回响信号+1，收到5次就计算均值
		while(GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_10) == 1);
		/*关闭定时器*/
		CloseTimer();
		/*获取Echo高电平时间时间*/
		t = GetEchoTimer();
		length = (float)t/58;//单位时cm
		sum += length;		
	}
	length = sum/5;//五次平均值
	
	return length;
}

