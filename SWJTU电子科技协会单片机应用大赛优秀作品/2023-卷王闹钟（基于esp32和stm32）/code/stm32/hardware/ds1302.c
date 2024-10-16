#include "ds1302.h"
static void DS1302_WriteByte(uint8_t addr,uint8_t data);
static uint8_t DS1302_ReadByte(uint8_t addr);

static void DS1302_Init(void);
static void DS1302_SetTime(uint8_t *writeTime);
static void DS1302_GetTime(DS1302TimeStruct *timeData);

DS1302ClassStruct DS1302Class = {
	.Init = DS1302_Init,
	.SetTime = DS1302_SetTime,
	.GetTime = DS1302_GetTime
};

/*
初始时间定义{0x20,0x21,0x11,0x13,0x10,0x10,0x00,0x06};
初始时间2021年11月13号10点10分00秒 星期六
*/
uint8_t time_buf[8] = {0x20};//初始时间
uint8_t readtime[8] = {0x20};//当前时间

//DS1302引脚定义,可根据实际情况自行修改端口定义
#define DS1302_CLK_PORT					GPIOA
#define DS1302_CLK_CLK					RCC_APB2Periph_GPIOA
#define DS1302_CLK_PIN					GPIO_Pin_4

#define DS1302_DAT_PORT					GPIOA
#define DS1302_DAT_CLK					RCC_APB2Periph_GPIOA
#define DS1302_DAT_PIN					GPIO_Pin_5

#define DS1302_RST_PORT					GPIOA
#define DS1302_RST_CLK					RCC_APB2Periph_GPIOA
#define DS1302_RST_PIN					GPIO_Pin_6

// Pin0-7设置CRL寄存器 Pin8-15设置CRH寄存器 配置输入输出模式
#define DS1302_DAT_OutPut_Mode()	{DS1302_DAT_PORT->CRL &= 0xFF0FFFFF;DS1302_DAT_PORT->CRL |= 0x00300000;}
#define DS1302_DAT_InPut_Mode()		{DS1302_DAT_PORT->CRL &= 0xFF0FFFFF;DS1302_DAT_PORT->CRL |= 0x00400000;}

//DS1302引脚输出输入
#define DS1302_DAT_IN()  		GPIO_ReadInputDataBit(DS1302_DAT_PORT, DS1302_DAT_PIN)
#define DS1302_CLK_0 				DS1302_CLK_PORT->BSRR = (uint32_t)DS1302_CLK_PIN<<16 // 置0
#define DS1302_CLK_1      	DS1302_CLK_PORT->BSRR = (uint32_t)DS1302_CLK_PIN // 置1
#define DS1302_DAT_0 				DS1302_DAT_PORT->BSRR = (uint32_t)DS1302_DAT_PIN<<16 // 置0
#define DS1302_DAT_1				DS1302_DAT_PORT->BSRR = (uint32_t)DS1302_DAT_PIN // 置1
#define DS1302_RST_0 				DS1302_RST_PORT->BSRR = (uint32_t)DS1302_RST_PIN<<16 // 置0
#define DS1302_RST_1 				DS1302_RST_PORT->BSRR = (uint32_t)DS1302_RST_PIN // 置1

//DS1302地址定义
#define DS1302_SEC_ADDR           0x80		//秒数据地址
#define DS1302_MIN_ADDR           0x82		//分数据地址
#define DS1302_HOUR_ADDR          0x84		//时数据地址
#define DS1302_DAY_ADDR           0x86		//日数据地址
#define DS1302_MONTH_ADDR         0x88		//月数据地址
#define DS1302_WEEK_ADDR          0x8a		//星期数据地址
#define DS1302_YEAR_ADDR          0x8c		//年数据地址
#define DS1302_CONTROL_ADDR       0x8e		//控制数据地址
#define DS1302_CHARGER_ADDR       0x90 		//充电功能地址			 
#define DS1302_CLKBURST_ADDR      0xbe

//向DS1302写入一字节数据
static void DS1302_WriteByte(uint8_t addr,uint8_t data) 
{
	uint8_t i;
	DS1302_RST_0; //禁止数据传输 ！！！这条很重要
	DS1302_CLK_0; //确保写数据前SCLK为低电平
	DS1302_RST_1;	//启动DS1302总线	
	DS1302_DAT_OutPut_Mode();
	addr=addr&0xFE;  //最低位置零，寄存器0位为0时写，为1时读
	for(i=0;i<8;i++) //写入目标地址：addr
	{
		if (addr&0x01) DS1302_DAT_1;
		else DS1302_DAT_0;
		DS1302_CLK_1; //时钟上升沿写入数据
		DS1302_CLK_0;
		addr=addr>>1;
	}	
	for (i=0;i<8;i++) //写入数据：data
	{
		if(data&0x01) DS1302_DAT_1;
		else DS1302_DAT_0;
		DS1302_CLK_1;    //时钟上升沿写入数据
		DS1302_CLK_0;
		data = data >> 1;
	}
	DS1302_CLK_1;    // 将时钟电平置于高电平状态 ，处于已知状态
	DS1302_RST_0;	//停止DS1302总线
}
 
//从DS1302读出一字节数据
static uint8_t DS1302_ReadByte(uint8_t addr) 
{
	uint8_t i,temp;	
	DS1302_RST_0; //这条很重要
	DS1302_CLK_0; //先将SCLK置低电平,确保写数居前SCLK被拉低
	DS1302_RST_1; //启动DS1302总线
	DS1302_DAT_OutPut_Mode();
	//写入目标地址：addr
	addr=addr|0x01; //最低位置高，寄存器0位为0时写，为1时读
	for(i=0;i<8;i++) 
	{
		if (addr&0x01) DS1302_DAT_1;
		else DS1302_DAT_0;
		DS1302_CLK_1; //写数据
		DS1302_CLK_0;
		addr = addr >> 1;
	}	
	//从DS1302读出数据：temp
	DS1302_DAT_InPut_Mode();
	for(i=0;i<8;i++)
	{
		temp=temp>>1;
		if (DS1302_DAT_IN()) temp|=0x80;
		else temp&=0x7F;
		DS1302_CLK_1;
		DS1302_CLK_0;
	}	
	DS1302_CLK_1;  //将时钟电平置于已知状态
	DS1302_RST_0;	//停止DS1302总线
	return temp;
}
/**
  * @brief  DS1302引脚初始化
  * @param  None
  * @retval None
  * @note   None
  */
static void DS1302_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DS1302_CLK_CLK | DS1302_DAT_CLK | DS1302_RST_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=DS1302_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DS1302_CLK_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=DS1302_RST_PIN;
	GPIO_Init(DS1302_RST_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=DS1302_DAT_PIN;
	GPIO_Init(DS1302_DAT_PORT,&GPIO_InitStructure);
}
/**
  * @brief  写入DS1302时间数据，初始化计时初值
  * @param  None
  * @retval None
  * @note   None
  */
static void DS1302_SetTime(uint8_t *writeTime) 
{
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x00);       //关闭写入
	DS1302_WriteByte(DS1302_SEC_ADDR,0x80);           //暂停时钟 
	//DS1302_WriteByte(DS1302_CHARGER_ADDR,0xa9);     //涓流充电 
	DS1302_WriteByte(DS1302_YEAR_ADDR,writeTime[1]);   //年 
	DS1302_WriteByte(DS1302_MONTH_ADDR,writeTime[2]);  //月 
	DS1302_WriteByte(DS1302_DAY_ADDR,writeTime[3]);    //日 
	DS1302_WriteByte(DS1302_HOUR_ADDR,writeTime[4]);   //时 
	DS1302_WriteByte(DS1302_MIN_ADDR,writeTime[5]);    //分
	DS1302_WriteByte(DS1302_SEC_ADDR,writeTime[6]);    //秒
	DS1302_WriteByte(DS1302_WEEK_ADDR,writeTime[7]);	  //周 
	DS1302_WriteByte(DS1302_CHARGER_ADDR,0xA5);//打开充电功能 选择2K电阻充电方式
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x80);//打开写入 
}
/**
  * @brief  获取DS1302时间数据
  * @param  None
  * @retval None
  * @note   None
  */
static void DS1302_GetTime(DS1302TimeStruct *timeData)
{ 
	uint8_t readtime[8] = {0x20};//当前时间
  readtime[1]=DS1302_ReadByte(DS1302_YEAR_ADDR);          //年 
	readtime[2]=DS1302_ReadByte(DS1302_MONTH_ADDR);         //月 
	readtime[3]=DS1302_ReadByte(DS1302_DAY_ADDR);           //日 
	readtime[4]=DS1302_ReadByte(DS1302_HOUR_ADDR);          //时 
	readtime[5]=DS1302_ReadByte(DS1302_MIN_ADDR);           //分 
	readtime[6]=(DS1302_ReadByte(DS1302_SEC_ADDR))&0x7f;    //秒，屏蔽秒的第7位，避免超出59
	readtime[7]=DS1302_ReadByte(DS1302_WEEK_ADDR);          //周 	
	timeData->year=(readtime[0]>>4)*1000+(readtime[0]&0x0F)*100+(readtime[1]>>4)*10+(readtime[1]&0x0F); //计算年份
	timeData->month=(readtime[2]>>4)*10+(readtime[2]&0x0F);  //计算月份
	timeData->day=(readtime[3]>>4)*10+(readtime[3]&0x0F);    //计算日期
	timeData->hour=(readtime[4]>>4)*10+(readtime[4]&0x0F);   //计算小时
	timeData->minute=(readtime[5]>>4)*10+(readtime[5]&0x0F); //计算分钟
	timeData->second=(readtime[6]>>4)*10+(readtime[6]&0x0F); //计算秒钟
	timeData->week=(readtime[7]&0x0F);                       //计算星期		
#ifdef DEBUG_printf
	//printf("read: %d年%d月%d日 %d时%d分%d秒\n", timeData->year, timeData->month, timeData->day, timeData->hour, timeData->minute, timeData->second);	
#endif
}
