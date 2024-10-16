#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include <string.h>
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag(void);

extern char Serial_RxPacket[]; // 单片机收到的数据
extern uint8_t Serial_RxFlag; //  判断是否接收完毕

#endif
