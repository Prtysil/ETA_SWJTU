#ifndef __PID_H
#define __PID_H
#include "stm32h7xx.h" 

double PID(double now_speed,double aim_speed,double P,double I,double D, double last_speed, double *I_out);

#endif