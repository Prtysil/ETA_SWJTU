#ifndef __MOTOR_H
#define __MOTOR_H

void motor_init(void);
void speed_right(int16_t speed);
void speed_left(int16_t speed);
void speed_on(int16_t speed);
void speedcontrol(int speed,double angle,int so,int sr,int sl);
void speedturn(int tar,int re1,int re2,int re3);
void Servo_SetAngle(float Angle);

#endif
