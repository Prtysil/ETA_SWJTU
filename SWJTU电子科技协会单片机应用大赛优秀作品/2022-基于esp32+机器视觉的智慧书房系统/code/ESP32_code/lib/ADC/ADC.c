#include <Arduino.h>

//ADC

#include "esp32-hal-adc.h"





void adc_1_4_init()
{
  adc_attenuation_t adc_attenuation_1;
  analogReadResolution(10);//ADC分辨率将获得0-1024个值

} 

int adc_1_test(int sensorValue)//均值滤波后输出adc的值，这里作为电位器输出值
{
   static int a=0; 
   static int sensorValue_sum=0;
   int sensorValue_sum_old=0;
   sensorValue = analogRead(adc1_4);//adc值
   sensorValue_sum=sensorValue_sum+sensorValue;
   a++;
   if(a==10)
   {
    sensorValue_sum_old=sensorValue_sum;
    sensorValue_sum=0;
    a=0;
    return sensorValue_sum_old;
   }
}
