#include "stm32f10x.h"                  // Device header



float err1=0,last_err1=0,next_err1=0,pwm1=0,add1=0,p=0.5,i=1,d=0;

float err2=0,last_err2=0,next_err2=0,pwm2=0,add2=0;

float err3=0,last_err3=0,next_err3=0,pwm3=0,add3=0;

float err4=0,last_err4=0,next_err4=0,pwm4=0,add4=0;

float err5=0,last_err5=0,next_err5=0,pwm5=0,add5=0;

float err6=0,last_err6=0,next_err6=0,pwm6=0,add6=0;

void clear_on()
{
	last_err1=0,next_err1=0,pwm1=0;
}



void clear_left(void)
{
	last_err2=0,next_err2=0,pwm2=0;
}

void clear_right(void)
{
	last_err3=0,next_err3=0,pwm3=0;
}

void clear_turn(void)
{
	last_err4=0,next_err4=0,pwm4=0;

	last_err5=0,next_err5=0,pwm5=0;
	
	last_err6=0,next_err6=0,pwm6=0;
}

float mabs(float speed)

{
	if(speed<0)
		return (-speed);
	else
		return speed;
	
}

void pwm_control(int pwm)
{
	if(pwm>99)
	{
		
		pwm=99;
	}
	if(pwm<0)
	{
		
		pwm=0;
	}
	
}

float pid_on(int16_t speed,float tar)
{
	err1=tar-(speed);
	add1=p*(err1-last_err1)+i*(err1)+d*(err1+next_err1-2*last_err1);
	pwm1=pwm1+add1;
	pwm_control(pwm1);
	next_err1=last_err1;
	last_err1=err1;
	return pwm1;
	
}

float pid_left(int16_t speed,float tar)
{
	err2=tar-(speed);
	add2=p*(err2-last_err2)+i*(err2)+d*(err2+next_err2-2*last_err2);
	pwm2=pwm2+add2;
	pwm_control(pwm2);
	next_err2=last_err2;
	last_err2=err2;
	return pwm2;
	
}

float pid_right(int16_t speed,float tar)
{
	err3=tar-(speed);
	add3=p*(err3-last_err3)+i*(err3)+d*(err3+next_err3-2*last_err3);
	pwm3=pwm3+add3;
	pwm_control(pwm3);
	next_err3=last_err3;
	last_err3=err3;
	return pwm3;
	
}

float pid_turn1(int16_t speed,float tar)
{
	err4=tar-(speed);
	add4=p*(err4-last_err4)+i*(err4)+d*(err4+next_err4-2*last_err4);
	pwm4=pwm4+add4;
	pwm_control(pwm4);
	next_err4=last_err4;
	last_err4=err4;
	return pwm4;
	
}
float pid_turn2(int16_t speed,float tar)
{
	err5=tar-(speed);
	add5=p*(err5-last_err5)+i*(err5)+d*(err5+next_err5-2*last_err5);
	pwm5=pwm5+add5;
	pwm_control(pwm5);
	next_err5=last_err5;
	last_err5=err5;
	return pwm5;
	
}
float pid_turn3(int16_t speed,float tar)
{
	err6=tar-(speed);
	add6=p*(err6-last_err6)+i*(err6)+d*(err6+next_err6-2*last_err6);
	pwm6=pwm6+add6;
	pwm_control(pwm6);
	next_err6=last_err6;
	last_err6=err6;
	return pwm6;
	
}
