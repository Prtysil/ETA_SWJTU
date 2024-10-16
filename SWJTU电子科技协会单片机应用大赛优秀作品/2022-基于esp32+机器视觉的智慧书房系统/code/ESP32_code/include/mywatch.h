#ifndef _MY_WATCH_H
#define _MY_WATCH_H

#include <Arduino.h>

//固件版本号
#define Firmware_version "V 1.0.0"

//是否开启debug功能
const unsigned long BAUD_RATE = 115200;   //波特率

#define DEBUG 

#ifdef DEBUG 
#define DebugPrintln(message) Serial.println(message)
#else 
#define DebugPrintln(message)
#endif

#ifdef DEBUG 
#define DebugPrint(message) Serial.print(message) 
#else 
#define DebugPrint(message)
#endif

//星座信息

//默认WIFI信息
char ssid[] = "yjtdenova9";
char pswd[] = "12345678";

//-----------------------GPIO配置-----------------------------
//LED配置
#define LED         2
#define LED_OFF   digitalWrite(LED, HIGH)//关灯
#define LED_ON    digitalWrite(LED, LOW)//开灯
#define LED_PWM   digitalWrite(LED, !digitalRead(LED))//灯闪烁

//蜂鸣器配置
#define BEEP        25
#define BEEP_ON   digitalWrite(BEEP, HIGH)//蜂鸣器响
#define BEEP_OFF  digitalWrite(BEEP, LOW)//蜂鸣器停
#define BEEP_PWM  digitalWrite(BEEP, !digitalRead(BEEP))//警报

// //振动马达配置
// #define MOTTER      26
// #define MOTTER_ON   digitalWrite(MOTTER, HIGH)//开振动
// #define MOTTER_OFF  digitalWrite(MOTTER, LOW)//振动停
// #define MOTTER_PWM  digitalWrite(MOTTER, !digitalRead(MOTTER))//来回振动DOWN_KEY_PRES

//按键配置
#define UP_KEY      39
#define M_KEY       0
#define DOWN_KEY    34
//读取键值
#define KEYU  digitalRead(UP_KEY)
#define KEYM  digitalRead(M_KEY)
#define KEYD  digitalRead(DOWN_KEY)

#define NO_KEY_PRES     0
#define UP_KEY_PRES     1
#define M_KEY_PRES      2
#define DOWN_KEY_PRES   3

#define UP_KEY_LONG_PRES     11
#define M_KEY_LONG_PRES      22
#define DOWN_KEY_LONG_PRES   33

long up_pres_time = 0;
long m_pres_time = 0;
long down_pres_time = 0;

long key_long_pres_time = 300;   //按键长时间按下时间



//电位器配置
#define adc1_4 32





//屏幕宽长
int screenWidth = 240;
int screenHeight = 135;

#endif /* _MY_WATCH_H */