#ifndef STM_KEY_H
#define STM_KEY_H 			   
#include "sys.h" 
#include "delay.h"

#define key_run PBin(4)
#define key_shift PBin(5)
#define key_beep PBin(6)
#define key_up PCin(13)
#define key_down PCin(14)

void KEY_Init(void);
u8 KEY_Scan(void);
#endif
