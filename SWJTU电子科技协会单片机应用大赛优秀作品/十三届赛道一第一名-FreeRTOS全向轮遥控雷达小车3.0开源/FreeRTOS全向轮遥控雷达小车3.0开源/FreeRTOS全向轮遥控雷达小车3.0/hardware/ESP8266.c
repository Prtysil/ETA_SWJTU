#include "stm32f10x.h"                  // Device header

#include "delay.h"
#include "USART.h"


char command0[]="AT+RST\r\n";
char command1[]="AT+CWMODE=3\r\n"; 
char command2[]="AT+CWSAP_DEF=\"ESP8266AP_test\",\"12345678\",1,3\r\n";
char command3[]="AT+CIPMUX=1\r\n"; //开启多连接
char command4[]="AT+CIPSTART=4,\"UDP\",\"192.168.4.2\",8080,8080,0\r\n"; //开启udp链接
char command5[]="AT+CIPMODE=1\r\n"; //开启透传
void ESP_Init(void)
{
	Serial_Init();
	delay_ms(100);
	Serial_SendString(command0);
	delay_ms(500);
	Serial_SendString(command1);
	delay_ms(100);
	Serial_SendString(command2);
	delay_ms(100);
	Serial_SendString(command3);
	delay_ms(100);
	Serial_SendString(command4);
	delay_ms(100);
	Serial_SendString(command5);
	delay_ms(100);
}

