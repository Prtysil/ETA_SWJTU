#ifndef __DS18B20_H
#define __DS18B20_H
void DS18B20_Init(void);
uint16_t DS18B20_ReadTemp(void);
uint8_t DS18B20_Reset(void);
extern uint8_t flag0;

#endif
