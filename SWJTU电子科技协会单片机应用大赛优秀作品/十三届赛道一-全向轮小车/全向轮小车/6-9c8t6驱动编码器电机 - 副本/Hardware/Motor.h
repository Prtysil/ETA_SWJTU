#ifndef __MOTOR_H__
#define __MOTOR_H__

void Motor_Init(void);

/*--------------------------------------------一号轮子控制----------------*/
//电机向前
void moto_go_1(void);
//电机向后
void moto_back_1(void);
//电机停止
void moto_stop_1(void);

/*--------------------------------------------二号轮子控制----------------*/
//电机向前
void moto_go_2(void);
//电机向后
void moto_back_2(void);
//电机停止
void moto_stop_2(void);

/*--------------------------------------------三号轮子控制----------------*/
//电机向前
void moto_go_3(void);
//电机向后
void moto_back_3(void);
//电机停止
void moto_stop_3(void);
#endif
