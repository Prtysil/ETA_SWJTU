#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "gpio.h"
#include "tim.h"
#include "pid.h"
#include "math.h"
#include "FreeRTOS.h"
#include "task.h"


/// @brief hong ding yi canshu
#define pi 3.1416f //Π
#define L  11.5f  //轴长

extern pid_type_def pid_1, pid_2, pid_3;
extern fp32 PID_1[3];
extern fp32 PID_2[3];
extern fp32 PID_3[3];
extern fp32 Vx_set, Vy_set, Vw_set;// x y w的速度设定值
extern fp32 speed_set_1, speed_set_2, speed_set_3;//1，2，3电机的速度设定值
extern uint8_t resert_flag;
extern int pid_out_1, pid_out_2, pid_out_3;// pid的输出
extern short num_1, num_2, num_3;// 编码器的计数
extern fp32 speed_1, speed_2, speed_3;// 速度
extern uint8_t angle_set;

void Pid_task(void*argument);
void Motor_Init(void);
void Motor_Counter(fp32 Vx_set, fp32 Vy_set, fp32 Vw_set);
void Speed_Counter(void);
void Angle_Speed(uint16_t angle, fp32 speed);
void Rotate_Counter(uint8_t angle_set);

#endif // !_MOTOR_H_
