/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */
//UI
#include "my_ui\myGUI.h"
#include "my_ui\demo.h"
#include "my_ui\demo1.h"
#include "my_ui\demo2.h"
//驱动
#include "dev_port\dsp_dev.h"
#include "dev_port\in_dev.h"
//udp
 #include "udp_rtc.h"
//ADC

#include "esp32-hal-adc.h"
//
#include "filter\kalman.h"

 #include <fsm_fsm.h>

 


// #include <nf_fsm.h>
//#include <stdlib.h>
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include "C:\Users\17752352458\.platformio\packages\framework-arduinoespressif32\libraries\HTTPClient\src\HTTPClient.h"


#define USE_SERIAL Serial

WiFiMulti wifiMulti;

const int ledPin = 26;  // 15 corresponds to GPIO16
 
// setting PWM properties
const int freq = 10000;
const int ledChannel = 0;
const int resolution = 8;
// const char* ssid = "yjtdenova9";
// const char* password = "12345678";


long rtc_time = 0;
 long timer =0;
 long timer_2 = 0;

// 电位器连接到GPIO 32(模拟ADC1_CH4)
const int potPin = 32;
// 用于存储电位器值的变量
// int potValue = 0;

// float dutyCycle = 0.0;
int dutyCycle_int = 0;
 int flag_0 = 1;//服务器传输，1正常
int dutyCycle_int_keep = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ADC 均值滤波转换
// 参数说明     ch              选择 
// 参数说明     count           均值滤波次数
// 返回参数     uint16          转换的 ADC 值
// 使用示例     adc_mean_filter_convert(ADC1_CH0_A0, 5);                        // 采集5次 然后返回平均值
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
unsigned int adc_mean_filter_convert (unsigned int abc, unsigned int count)
{
    unsigned int i;
    unsigned int sum;

    //zf_assert(count);// 断言次数不能为0

    sum = 0;
    for(i=0; i<count; i++)
    {
        sum += abc;
    }

    sum = sum/count;

    return  int(sum);
}

int adc_button_filter()
{
  double potValue;
  double dutyCycle;
 potValue = analogRead(potPin);
 dutyCycle=(double)(adc_mean_filter_convert(int(potValue),3))/1024 *255;
return dutyCycle;
}

int Flag_1_Judge()//1是解锁，0是锁定
{

static int time_2=millis();
static int Flag_1=2;
static int dutyCycle_keep =adc_button_filter();


if(millis()-time_2>20000){
if(abs(adc_button_filter()-dutyCycle_keep)<30){
 time_2=millis();
 Flag_1=0;
dutyCycle_keep =adc_button_filter();
return 0;
}else if(abs(adc_button_filter()-dutyCycle_keep)>30)
{
return 1;
}
}

}



//状态机部分

/* 转换执行的外部函数声明 */


/* 信号产生者 */
void Test_Flag_Process(void)
{
	// static NF_Bool last_q_val = NF_Bool_False;
	// static NF_Bool last_e_val = NF_Bool_False;

	// NF_Bool then_q_val;
	// NF_Bool then_e_val;

	// then_q_val = KEY_VALUE('Q');
	// then_e_val = KEY_VALUE('E');

	/* people_flag事件处理 */
	if (flag_0==0)
	{
		NF_FSM_Dispatch(&FSM_fsm, NF_FSM_Event("people_flag0"));
	} 
	else if (flag_0==1)
	{
		NF_FSM_Dispatch(&FSM_fsm, NF_FSM_Event("people_flag1"));
	}

	/* Botton_flag事件处理 */
	if (Flag_1_Judge() ==1)
	{
		NF_FSM_Dispatch(&FSM_fsm, NF_FSM_Event("Botton_flag1"));
	} 
	else if (Flag_1_Judge() ==0)
	{
		NF_FSM_Dispatch(&FSM_fsm, NF_FSM_Event("Botton_flag0"));
	}

	// last_q_val = then_q_val;
	// last_e_val = then_e_val;
}




void Light_OFF(void)
{
  ledcWrite(0, 255);
  Serial.println("姿势不正确，关灯！");

}

 void LED_adc_work(void)
 {

   Serial.println("PWM（变化）:1-姿势正确");
   dutyCycle_int= adc_button_filter();

   ledcWrite(0, dutyCycle_int);

 }
 void LED_adc_keep(void){
   Serial.println("PWM（锁定）:");
   dutyCycle_int_keep= adc_button_filter();
   ledcWrite(0, dutyCycle_int_keep);

 }
 void Light_ON(void){
   Serial.println("1-姿势正确");
 }


















// void led_work(int flag_1)
// {
//   static int potValue;
//   int  potValue_last=potValue;
//   static int a=0;
//   int b=0;
//   static int dutyCycle_int_keep=0;
//   static int flag_2=0;
//   static int flag_3=0;

//    // 读取电位计值
//   potValue = analogRead(potPin);
//  // Serial.println(potValue);
//   delay(500);

//    //锁定LEDPWM值保持稳定，大拧锁定，或姿势不正确信号锁定
//  ///  if(abs(potValue_last-potValue)<50&&abs(int(a-millis()))<500&&flag_1==1&&flag_2==1)//10ms不动则锁定
  
//   if(flag_1==1) {
//  Serial.println("PWM（变化）:1-姿势正确");
//    dutyCycle=(double)(potValue)/1024 *255;
//    dutyCycle_int=(int)dutyCycle;
//    dutyCycle_int_keep= dutyCycle_int;
//    ledcWrite(ledChannel, dutyCycle_int_keep);
//    Serial.println("PWM（锁定）:");
//    Serial.println(dutyCycle_int_keep);
   
//    }


//    if(abs(potValue_last-potValue)>20)
//    flag_2=0;

// //     if(flag_2==0&&flag_1==1)//判断人为拧动，解锁
// //    {
// //      if(flag_3==0)
// //    a=millis();//a重置
// //    Serial.println("PWM（jiesuo）:");
// //    flag_3=1;
 
// //    dutyCycle=(double)(potValue)/1024 *255;
// //    dutyCycle_int=(int)dutyCycle;
// //    ledcWrite(ledChannel, dutyCycle_int);
// //    Serial.println("PWM（变化）:");
// //    Serial.println(dutyCycle_int);
    
// //      if(a-millis()>30000)
// //      {
// //      flag_2=1;
// //      flag_3=0;
// //      a=millis();//a重置
// //      Serial.println("时间:");
// //      }
// //    }


//    if(flag_1==0)
//    {
//    a=millis();//a重置
//   ledcWrite(ledChannel, 0);
//    Serial.println("PWM（变化）:0-姿势不正确");
//    Serial.println(0);
//    }


// }


void setup() {

    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    // for(uint8_t t = 4; t > 0; t--) {
    //     USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    //     USE_SERIAL.flush();
    //     delay(1000);
    // }
      WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  //UDP INT
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.remotePort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(100);
 //   wifiMulti.addAP("yjtdenova9","12345678");

  //LVGL初始化
  lv_init();
  //屏幕驱动
  lv_port_disp_init();
  //输入驱动
  lv_port_indev_init();

  lv_ex_tabview_1();
  //lv_demo_widgets();//官方demo
  //lv_demo_stress();//官方demo1
  //lv_ex_tileview_1();//官方demo2

   // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
 NF_FSM_Translate(&FSM_fsm, NF_FSM_State(FSM_fsm_Light_mode_ON)) ;//初始化状态机
digitalWrite(BEEP, HIGH);

}

void loop() 
{


    // wait for WiFi connection
//      if(millis()-timer_2>10){
//          Serial.println("DDDDDDDDD");
// //led_work(flag_0);
// timer_2=0;
//      }
 
// void FSM_fsm_Light_mode_change(NF_FSM* me, NF_Event event);
// void FSM_fsm_Light_mode_OFF(NF_FSM* me, NF_Event event);
// void FSM_fsm_Light_mode_ON(NF_FSM* me, NF_Event event);
   Test_Flag_Process();

     if(millis()-rtc_time>1000)
  {
       digitalWrite(BEEP, LOW);
     now();
    digitalClockDisplay();
   rtc_time=millis();
  }
   
      if (millis() - timer >= 10000) 
   {    // 执行需要执行的任务
        Serial.println("10秒已经过去了！");
        if(WiFi.status() == WL_CONNECTED) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin("https://4g526880v1.goho.co/b"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
              
                    if (payload.equals("1"))
                        flag_0 =1;//0是不亮
                    else
                        flag_0=0;
            }
        } else {
             USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                
        }

       
         USE_SERIAL.print("[HTTP] GET...\n");
        http.end();
       }
        timer = millis(); // 重置计时器
     
   }
  


  digitalWrite(BEEP, HIGH);
  



  
  lv_task_handler(); /* let the GUI do its work */
  delay(5);

}