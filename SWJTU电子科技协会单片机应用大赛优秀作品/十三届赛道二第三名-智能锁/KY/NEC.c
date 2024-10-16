#include "stm32f10x.h"    
#include "IC.h"

   
	
	uint16_t s[]={1,0,1,1,0,1,0,1};
void NEC_Init(void)
{ 
	int G;
	int D;
	int f=0;
	int c=0;
	
	int a=0;
	G = TIM_GetCapture1(TIM3);
	D = TIM_GetCapture2(TIM3);
	
	if(D==0)
	{
		if(G>=9600)
			f=1;	
	}
	if(f==1)
		if(G==0)
			{
				if(D>=4500)
					f=2;
			}
	
	a=2;
	while(1)
	{
		if(f==2)
		{
			if(D==0)
				if(G<=1000)
				{
					s[c]=1;
					c++;
				}
			if(D==0)
				if(G>500)
				{
					s[c]=0;
					c++;
				}
		}
		if(c==8)
		{
			a=3;
			break;
		}
	}
	f=0;
	c=0;

}
uint32_t Get_NEC(void)
{
	int a=1;
	int n=s[0];
	while(1)
	{
		if(a<=7)
		{
			n=n*10+s[a];
			a++;
		}
		else 
			break ;
		
	}
	return n;
}