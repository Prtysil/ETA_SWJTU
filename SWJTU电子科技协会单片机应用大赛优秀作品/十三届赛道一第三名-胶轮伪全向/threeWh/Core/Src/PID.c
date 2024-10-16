#include "PID.h"
double P_out;
double D_out;
double PID_out;


double PID(double now_speed,double aim_speed,double P,double I,double D,double last_speed, double *I_out)
{
	if(aim_speed>600) aim_speed=600;
	P_out=P*(aim_speed-now_speed);
	*I_out+=I*(aim_speed-now_speed);
	
	//积分限幅
	if(*I_out>10)
		 *I_out=10;
	if(*I_out<-10)
		*I_out=-10;
	
	D_out=D*(now_speed-last_speed);
	
	PID_out=P_out+(*I_out)+D_out;
	last_speed=now_speed;
	if(PID_out>1000) PID_out = 1000;
	if(PID_out<-1000) PID_out = -1000;
	return PID_out;
}