#ifndef __PID_H
#define __PID_H
float mabs(float speed);
void clear_on(void);
void clear_left(void);
void clear_right(void);
void clear_turn(void);
float pid_on(int16_t speed,float tar);
float pid_left(int16_t speed,float tar);
float pid_right(int16_t speed,float tar);
float pid_turn1(int16_t speed,float tar);
float pid_turn2(int16_t speed,float tar);
float pid_turn3(int16_t speed,float tar);
#endif
