// //UI
// #include "my_ui\myGUI.h"
// #include "my_ui\demo.h"
// #include "my_ui\demo1.h"
// #include "my_ui\demo2.h"
// //驱动
// #include "dev_port\dsp_dev.h"
// #include "dev_port\in_dev.h"
// //udp
// #include "udp_rtc.h"
// //ADC
// //#include <ESP32AnalogRead.h>
// #include "esp32-hal-adc.h"


// // 电位器连接到GPIO 32(模拟ADC1_CH4)
// const int potPin = 32;
// // 用于存储电位器值的变量
// int potValue = 0;






// void setup() {
//   Serial.begin(115200); /* prepare for possible serial debug */

//   //联网
//   WiFi.begin(ssid, pswd);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   //UDP INT
//   Udp.begin(localPort);
//   Serial.print("Local port: ");
//   Serial.println(Udp.remotePort());
//   Serial.println("waiting for sync");
//   setSyncProvider(getNtpTime);
//   setSyncInterval(100);

//   //LVGL初始化
//   lv_init();
//   //屏幕驱动
//   lv_port_disp_init();
//   //输入驱动
//   lv_port_indev_init();

//   lv_ex_tabview_1();
//   //lv_demo_widgets();//官方demo
//   //lv_demo_stress();//官方demo1
//   //lv_ex_tileview_1();//官方demo2
// }

// long rtc_time = 0;

// void loop() {


//    // 读取电位计值
//   potValue = analogRead(potPin);
//   Serial.println(potValue);




//   if(millis()-rtc_time>1000)
//   {
//     now();
//     digitalClockDisplay();
//   }
//   lv_task_handler(); /* let the GUI do its work */
//   delay(5);
// }


