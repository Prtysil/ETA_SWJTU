#include "DS1302.h"

 /*****************************************
 * 函数名：void DS1302_GPIOInit(void)
 * 描述  ：    DS1302 GPIO配置
 * 输入  ：
 * 输出  ：无
 * 调用  ：
        CLK---PB13,
        IO--->PA9,
        RES--->PA10,
 *************************************/
void DS1302_GPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructre1;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);            /*open GPIO  clock*/
  GPIO_InitStructre1.GPIO_Pin= GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructre1.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructre1.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(DS1302_PORT, &GPIO_InitStructre1);
  GPIO_InitTypeDef GPIO_InitStructre2;	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);            /*open GPIO  clock*/
  GPIO_InitStructre2.GPIO_Pin= GPIO_Pin_13;
  GPIO_InitStructre2.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructre2.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructre2);
}
 /*****************************************
 * 函数名：void DS1302_IO_GPIO(void)
 * 描述  ：    DS1302 之 IO GPIO 输入输出配置
 * 输入  ：FLAG标志位
 * 输出  ：无
* 调用  ：OUT:表示输出，IN:表示输入
          FLAG：
 *************************************/
void DS1302_IO_GPIO(uchar FLAG)
{
  GPIO_InitTypeDef GPIO_InitStructre;
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);            /*open GPIO  clock*/
    /**********配置数据IO端口 输出 **********/
    if(FLAG==0x01)
  {
   GPIO_InitStructre.GPIO_Pin= GPIO_Pin_9;//配置IO_GPIO
  GPIO_InitStructre.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructre.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(DS1302_PORT, &GPIO_InitStructre);
  }
    /**********配置数据IO端口 输入**********/
    else if(FLAG==0x00)
  {
     GPIO_InitStructre.GPIO_Pin= GPIO_Pin_9;//配置IO_GPIO
   GPIO_InitStructre.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructre.GPIO_Mode=GPIO_Mode_IPU;  //配置上拉输入
   GPIO_Init(DS1302_PORT, &GPIO_InitStructre);
   }
}
  /*****************************************
 * 函数名：void DS1302_delay(u8 dd)
 * 描述  ：简单延时
 * 输入  ：
 * 输出  ：无
 * 调用  ：
 *************************************/
void DS1302_delay(u8 dd)
{
    u8 i;
    for(;dd>0;dd--)
  for(i=110;i>0;i--);
}

  /*****************************************
 * 函数名：void DS1302_Write(uchar add,uchar dat)
 * 描述  ：DS1302写指令和数据
*  输入  ：add:发送地址，dat：所在数据
 * 输出  ：无
 * 调用  ：
 *************************************/
 void DS1302_Write(uchar add,uchar dat)
 {
 uchar i,temp1,temp2;
 temp1=add;
 temp2=dat;
 RES_Set_1;//RET=1;
//发送地址
for(i=0;i<8;i++)
 {
        if(temp1&0x01)
         {IO_Set_1;  }     //IO=1;
        else
        { IO_Reset_0;}   //IO=0;
        temp1=temp1>>1;
        CLK_Set_1;            //CLK=1;
        DS1302_delay(2);
        CLK_Reset_0;    //CLK=0;
  }

//发送数据
for(i=0;i<8;i++)
    {
        /*IO=(temp2>>i)&0x01;这一句代替下面屏蔽的内容  */
        if(temp2&0x01)
         {IO_Set_1;  }     //IO=1;
        else
         { IO_Reset_0;}    //IO=0;
            temp2=temp2>>1;
            CLK_Set_1;            //CLK=1;
            DS1302_delay(2);
            CLK_Reset_0;    //CLK=0;
    }
RES_Reset_0;// RET=0;
 }




/*****************************************
 * 函数名：uchar DS1302_Read(uchar add)
 * 描述  ：DS1302读数据
 *  输入  ：add:发送所在地址
 * 输出  ：
 * 调用  ：
 *************************************/
uchar DS1302_Read(uchar add)
{
  uchar i,suf,temp,mm,nn,value;
   temp=add;
   RES_Set_1;//RET=1;
    //写地址
  for(i=0;i<8;i++)
      {
       if(temp&0x01)
       {IO_Set_1;  }     //IO=1;
      else
         { IO_Reset_0;}    //IO=0;
      temp=temp>>1;
        CLK_Set_1;            //CLK=1;
        DS1302_delay(2);
        CLK_Reset_0;    //CLK=0;
      }
 //读数据

DS1302_IO_GPIO(IN);//配置IO为输入
  for(i=0;i<8;i++)
    {
     suf=suf>>1;//读数据变量
     if(IO_Read)    //IO=1
     {
         suf=suf|0x80;
     }
     else     //IO=0
     {
     suf=suf&0x7f;
     }

        CLK_Set_1;            //CLK=1;
        DS1302_delay(2);
        CLK_Reset_0;    //CLK=0;
    }
    RES_Reset_0;    // RET=0;

DS1302_IO_GPIO(OUT);//配置IO为输出,恢复正常状态
//数据处理转化十进制
mm=suf/16;
nn=suf%16;
value=mm*10+nn;
return value;
}
/*****************************************
 * 函数名：void DS1302_SetTime(uchar *ad)
 * 描述  ：DS1302 写入设置时间
 *  输入  ：add:发送所在地址
 * 输出  ：
 * 调用  ：
 *************************************/
void DS1302_SetTime(uchar *ad)
  {
   DS1302_Write(0x8e,0x00);   //WP=0 允许数据写入DS1302
/**********以下对时分秒的初始化*************/
        DS1302_Write(ds1302_sec_addr,ad[5]/10*16+ad[5]%10);  //秒
        DS1302_Write(ds1302_min_addr,ad[4]/10*16+ad[4]%10);   //分
        DS1302_Write(ds1302_hour_addr,ad[3]/10*16+ad[3]%10);   //时
      /**********以下对年月日的初始化*************/
     DS1302_Write(ds1302_day_addr,ad[2]/10*16+ad[2]%10);
   DS1302_Write(ds1302_month_addr,ad[1]/10*16+ad[1]%10);
      DS1302_Write(ds1302_year_addr,ad[0]/10*16+ad[0]%10);

     DS1302_Write(0x8e,0x80);   //0x8e控制字节地址,bit7=WP WP=1 禁止数据写入DS1302
  }


/*****************************************
 * 函数名： void DS1302_OFF(void)
 * 描述  ：DS1302时间禁止走时
 *  输入  ：
 * 输出  ：
 * 调用  ：
 *************************************/
  void DS1302_OFF(void)
  {
   uchar temp;
   temp=DS1302_Read(0x81);//读取表地址时间
   DS1302_Write(0x8e,0x00);//WP=0 允许数据写入DS1302
   temp=temp|(1<<7);
   DS1302_Write(0x80,temp);//WP=1 禁止数据写入DS1302

  }
/*****************************************
 * 函数名： void DS1302_ON(void)
 * 描述  ：DS1302时间开始运行，走时
 *  输入  ：
 * 输出  ：
 * 调用  ：
 *************************************/
  void DS1302_ON(void)
  {
   uchar temp;
   temp=DS1302_Read(0x81);
   DS1302_Write(0x8e,0x00);//WP=0 允许数据写入DS1302
   temp=temp|(0<<7);
   DS1302_Write(0x80,temp);//WP=0 允许数据写入DS1302

  }






 /*****************************************
 * 函数名：void DS1302_init(uchar *time)
 * 描述  ：    DS1302初始化
 * 输入  ：无
 * 输出  ：无
* 调用  ：
 *************************************/
void DS1302_init(uchar *time)
{
    DS1302_GPIOInit();//GPIO初始化配置
    DS1302_delay(2);
  RES_Reset_0; //RET=0;
  CLK_Reset_0;// CLK=0;
    //下面是对DS1302启动电池，不掉电   设置时间
    DS1302_Write(0x8e,0x00);//WP=0 允许数据写入DS1302
  //  DS1302_Write(0x90,0xA7);//充电(1个二极管+8K电阻)
  //  DS1302_Write(0x8E,0X80);//开启保护 WP=1
//查询DS302时钟是否启动,如果时钟停止走动：启动时钟+初始化时钟
    
        DS1302_SetTime(time);//设置设置初始时钟
    
    //否则跳过

}
 /*****************************************
 * 函数名：void DS1302_Readtime(void)
 * 描述  ：    DS1302时间读出
 * 输入  ：无
 * 输出  ：无
* 调用  ：
 *************************************/
void DS1302_Readtime(void)//
{
			time_data[0]=DS1302_Read( 0x8D);
			time_data[1]=DS1302_Read( 0x89);
			time_data[2]=DS1302_Read( 0x87);
			time_data[3]=DS1302_Read( 0x85);
			time_data[4]=DS1302_Read( 0x83);
			time_data[5]=DS1302_Read( 0x81);
}
