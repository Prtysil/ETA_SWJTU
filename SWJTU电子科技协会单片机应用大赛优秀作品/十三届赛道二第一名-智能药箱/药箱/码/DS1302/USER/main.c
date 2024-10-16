#include "sys.h"
#include "delay.h"
#include "BEEP.h"
#include "key.h"
#include "DS1302.h"
#include "oled.h"
#include "stdio.h"
#include "DS18B20.h"
#include "LightSensor.h"
#include "pwm.h"

uchar time_data[6]={0};//读取DS1302当前时间缓存矩阵
uchar bell_data[3]={0};//闹铃设置界面
uchar bell_flag=0;//闹铃功能开关
uchar bell_onoff=0;//闹铃开关
void display_year(uchar a,uchar flag)//flag为1 时，显示数据，0时对应清除屏幕
{
	if(flag)
	{
	OLED_ShowChar(24, 0, a/10+'0');//年	
	OLED_ShowChar(32, 0, a%10+'0');
	}
	else
	{
	//清除年位
	OLED_ClearChar(24, 0);
	}	
}
void display_month(uchar a,uchar flag)
{
	if(flag)
	{
	OLED_ShowChar(56, 0, a/10+'0');//月
	OLED_ShowChar(64, 0, a%10+'0');
	}
	else
	{
	//清除月位
	OLED_ClearChar(56, 0);
	}
}
void display_day(uchar a,uchar flag)
{
	if(flag)
	{
	OLED_ShowChar(88, 0, a/10+'0');//日
	OLED_ShowChar(96, 0, a%10+'0');
	}
	else
	{
	//清除日位
	OLED_ClearChar(88, 0);
	}
}
void display_hour(uchar a,uchar flag)
{
	if(flag)
	{
	OLED_ShowChar24(16, 3, a/10+'0');//时
	OLED_ShowChar24(28, 3, a%10+'0');
	}
	else
	{
	//清除时位
	OLED_ClearChar24(16, 3);
	}
}
void display_min(uchar a,uchar flag)
{
	if(flag)
	{
	OLED_ShowChar24(52, 3, a/10+'0');//分
	OLED_ShowChar24(64, 3, a%10+'0');
	}
	else
	{
	//清除分位
	OLED_ClearChar24(52, 3);
	}
}
void display_sec(uchar a,uchar flag)
{
	if(flag)
	{
	OLED_ShowChar24(88, 3, a/10+'0');//秒
	OLED_ShowChar24(100, 3, a%10+'0');
	}
	else
	{
	//清除秒位
	OLED_ClearChar24(88, 3);
	}
}

void display(uchar a,uchar flag,uchar shift)
{
	switch(shift)
	{
	case 0:	display_sec(a,flag); break;
	case 1:	display_min(a,flag);  break;
	case 2:	display_hour(a,flag);  break;
	case 3:	display_day(a,flag);  break;
	case 4:	display_month(a,flag);  break;
	case 5:	display_year(a,flag); break;
	default : break;
	}

}


void show_time(uchar show_data[6])//时间打印函数
{

	OLED_ShowChar(24, 0, show_data[0]/10+'0');//年	
	OLED_ShowChar(32, 0, show_data[0]%10+'0');
	
	OLED_ShowChar(56, 0, show_data[1]/10+'0');//月
	OLED_ShowChar(64, 0, show_data[1]%10+'0');
	
	OLED_ShowChar(88, 0, show_data[2]/10+'0');//日
	OLED_ShowChar(96, 0, show_data[2]%10+'0');
	
	OLED_ShowChar24(16, 3, show_data[3]/10+'0');//时
	OLED_ShowChar24(28, 3, show_data[3]%10+'0');
		
	OLED_ShowChar24(40, 3, ':');
		
	OLED_ShowChar24(52, 3, show_data[4]/10+'0');//分
	OLED_ShowChar24(64, 3, show_data[4]%10+'0');
				
	OLED_ShowChar24(76, 3, ':');
		
	OLED_ShowChar24(88, 3, show_data[5]/10+'0');//秒
	OLED_ShowChar24(100, 3, show_data[5]%10+'0');	
}





int main(void)
{
	uchar set_run=0;//按键run状态标识符
	uchar set_bell=0;//按键bell状态标识符
	uchar set_shift=0;//调时位置标识符
	uchar set_shift_bell=0;//bell调时位置标识符
	uchar init_time[6]={24,4,9,22,30,15};//初始化时间参数
	uint16_t kind=1;
	uchar set_kind=0;
		DS18B20_Init();//温感初始化
		uint16_t T ;
	delay_init();
	delay_ms(50);
	LightSensor_Init();
	BEEP_GPIO_Init();
	KEY_Init(); 			//按键初始化
	OLED_Init();
	DS1302_init(init_time);
	uint16_t kindflag(uint16_t kind);
	OLED_Clear();
	T = DS18B20_ReadTemp();
	OLED_ShowNum(8,0, 20, 2,0);
	OLED_ShowCH(40,0,"年");
	OLED_ShowCH(72,0,"月");	
	OLED_ShowCH(104,0,"日");
	OLED_ShowNum(26, 6, T/10%100, 2, 0);
	OLED_ShowNum(46, 6, T%10, 1, 0);
	OLED_ShowNum(96, 6, kind, 1, 0);
	BEEP=0;
	
	while(1)
	{
/*按键逻辑*/
	switch(KEY_Scan())
	{
	case 1: //按下run键
			set_run=1;
	//读取当前时间并显示
			DS1302_Readtime();
			show_time(time_data);
			while(set_run)
			{

				//对应设置位闪烁

				display(time_data[5-set_shift],0,set_shift);
				delay_ms(50);
				display(time_data[5-set_shift],1,set_shift);

				switch(KEY_Scan())
				{
					case 1: DS1302_SetTime(time_data);set_run = 0;break;//第二次按下run键保存时间并退出设置
					case 2: if(set_shift++>4)set_shift=0;break;//移位
					case 3: break;
					case 4: time_data[5-set_shift]++;break;//up
					case 5: time_data[5-set_shift]--;break;//down
					default : break;
				}
			}				break;
	case 2:	bell_onoff=0;BEEP=0; break;//关闭闹铃
	case 3: set_bell=1;//按下闹铃设置键
	//读取当前设定时间并显示
			display_hour(bell_data[0],1);
			display_min(bell_data[1],1);
			display_sec(bell_data[2],1);
			while(set_bell)
			{
				//对应设置位闪烁
				display(bell_data[2-set_shift_bell],0,set_shift_bell);
				delay_ms(50);
				display(bell_data[2-set_shift_bell],1,set_shift_bell);

				switch(KEY_Scan())
				{
					case 1: bell_flag=!bell_flag;show_bell(bell_flag);	
									break;//开启或关闭闹钟使能
					case 2: if(set_shift_bell++>1)set_shift_bell=0;break;//移动调整位置
					case 3: set_bell=0;break;//退出时间设置
					case 4: bell_data[2-set_shift_bell]++;break;//up
					case 5: bell_data[2-set_shift_bell]--;break;//down
					default : break;
				}
			}		break;
	case 4: 
				T = DS18B20_ReadTemp();OLED_ShowNum(26, 6, T/10%100, 2, 0);OLED_ShowNum(46, 6, T%10, 1, 0);	break;//更新显示温度
				
			
	case 5: set_kind=1;
				while (set_kind)
				switch (KEY_Scan ())	
				{
					case 1:set_kind =!set_kind;break;
					case 5:kind++;if (kind  == 4) kind =1;	OLED_ShowNum(94, 6, kind, 1, 0);break;//改变药物种类
				}
				break;
	default : break;
	};

		
		DS1302_Readtime();
		show_time(time_data);
		
		if(bell_data[0]==time_data[3] && bell_data[1]==time_data[4] && bell_data[2]==time_data[5])//触发闹铃
		{
					bell_onoff=1;
		}
		if(bell_flag && bell_onoff)
		{
		BEEP=!BEEP;
		}
		if(kind == 1){
		if(LightSensor_Get3()==1){
		if(LightSensor_Get1()==0)
		{
			bell_onoff=0;BEEP=0;
		}}
		else if(bell_flag && bell_onoff) BEEP=!BEEP;
		}
		if(kind == 2){
		if(LightSensor_Get1()==1){
		if(LightSensor_Get3()==0)
		{
			bell_onoff=0;BEEP=0;
		}}
		else if (bell_flag && bell_onoff) BEEP=!BEEP;
		}
		/*OLED_ShowNum(100, 6, LightSensor_Get1(), 1, 0);
		OLED_ShowNum(108, 6, LightSensor_Get2(), 1, 0);
		OLED_ShowNum(116, 6, LightSensor_Get3(), 1, 0);*///检验光感
	}
}
uint16_t kindflag(uint16_t kind)//是否正确拿药
{
		uint8_t flag = 0;
		if((kind ==1)&&(~LightSensor_Get1())&&(~LightSensor_Get2())&&(LightSensor_Get3()))
			flag = 1;
		else if((kind ==2)&&(LightSensor_Get1())&&(~LightSensor_Get2())&&(LightSensor_Get3()))
			flag = 1;
		else if((kind ==3)&&(LightSensor_Get1())&&(~LightSensor_Get2())&&(~LightSensor_Get3()))
			flag = 1;
		else flag = 0;
		return flag;
}

