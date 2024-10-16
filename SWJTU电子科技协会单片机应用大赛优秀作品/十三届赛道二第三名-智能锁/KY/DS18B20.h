#ifndef __DS18B20_H__
#define __DS18B20_H__
uint8_t DS_Readbyte(void);
void Tim3_Init(void);
float DS_ReadTempreture(void);
void Convert_Tempreture(void);
void DS_Writebyte(uint8_t Data);
uint8_t DS_Readbit(void);
void DS_Writebit_1(void);
void DS_Writebit_0(void);
void Init_DS1B20(void);
void DS_FIN(void);
void DS_PPOUT(void);	
void DS_Configure(void);
#endif

