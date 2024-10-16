#ifndef _LED_H_
#define _LED_H_


const int ledPin = 26;  // 15 corresponds to GPIO16
 
// setting PWM properties
const int freq = 10000;
const int ledChannel = 0;
const int resolution = 8;



// 电位器连接到GPIO 32(模拟ADC1_CH4)
const int potPin = 32;
// 用于存储电位器值的变量
int potValue = 0;

float dutyCycle = 0.0;
int dutyCycle_int = 0;

void led_work(int flag_1);





#endif