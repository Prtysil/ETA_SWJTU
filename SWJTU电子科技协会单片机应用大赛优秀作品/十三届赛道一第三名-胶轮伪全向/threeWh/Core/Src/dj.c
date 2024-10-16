#include "dj.h"

int now_speed1, now_speed2, now_speed3;
int set_speed1, set_speed2, set_speed3;
int last_speed1, last_speed2, last_speed3;
double AP=0.6, AI=0.01, AD=0.1;
double PWM1, PWM2, PWM3;
double I1=0, I2=0, I3=0;
uint8_t Mode_1=0, Mode_2=0, Mode_3=0;

void DJ_All_Init(){
    // 初始化电机pwm
    __HAL_TIM_SetCompare(DJ_1_TIM, DJ_1_PWM_CHANNEL, 0);
    __HAL_TIM_SetCompare(DJ_2_TIM, DJ_2_PWM_CHANNEL, 0);
    __HAL_TIM_SetCompare(DJ_3_TIM, DJ_3_PWM_CHANNEL, 0);

    DJ_Set_Mode(1, 0);
    DJ_Set_Mode(2, 0);
    DJ_Set_Mode(3, 0);

    HAL_TIM_Encoder_Start(DJ_1_ENCODE, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(DJ_2_ENCODE, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(DJ_3_ENCODE, TIM_CHANNEL_ALL);

    HAL_TIM_Base_Start(DJ_1_TIM);
    HAL_TIM_PWM_Start(DJ_1_TIM, DJ_1_PWM_CHANNEL);  // 此处由于三个公用一个定时器，故...
    HAL_TIM_PWM_Start(DJ_1_TIM, DJ_2_PWM_CHANNEL);  
    HAL_TIM_PWM_Start(DJ_1_TIM, DJ_3_PWM_CHANNEL);  

    

    set_speed1 = 0;
    set_speed2 = 0;
    set_speed3 = 0;
}

void DJ_Set_Mode(uint8_t DJ, uint8_t mode){
    switch(DJ){
        case 1:
            Mode_1 = mode;
            break;
            
        case 2:
            Mode_2 = mode;
            break;
            
        case 3:
            Mode_3 = mode;
            break;
    }
}

void DJ_Set_PWM(uint8_t DJ, uint16_t pwm){
	if(pwm>700) pwm = 700;
    switch(DJ){
            case 1:
                __HAL_TIM_SetCompare(DJ_PWM_TIM, DJ_1_PWM_CHANNEL, pwm);
                break;
                
            case 2:
                __HAL_TIM_SetCompare(DJ_PWM_TIM, DJ_2_PWM_CHANNEL, pwm);
                break;
                
            case 3:
                __HAL_TIM_SetCompare(DJ_PWM_TIM, DJ_3_PWM_CHANNEL, pwm);
                break;
        }

}

int DJ_Get_Encoder(uint8_t DJ){
    switch(DJ){
        case 1:
            __HAL_TIM_SetCounter(DJ_1_ENCODE, 0);
            HAL_Delay(20);
            return __HAL_TIM_GET_COUNTER(DJ_1_ENCODE);
            break;
            
        case 2:
            __HAL_TIM_SetCounter(DJ_2_ENCODE, 0);
            HAL_Delay(20);
            return __HAL_TIM_GET_COUNTER(DJ_2_ENCODE);
            break;
            
        case 3:
            __HAL_TIM_SetCounter(DJ_3_ENCODE, 0);
            HAL_Delay(20);
            return __HAL_TIM_GET_COUNTER(DJ_3_ENCODE);
            break;
    }
		return 0;
}

void DJ_Get_Encoder_All(int *A, int *B, int *C){
    __HAL_TIM_SetCounter(DJ_1_ENCODE, 0);
    __HAL_TIM_SetCounter(DJ_2_ENCODE, 0);
    __HAL_TIM_SetCounter(DJ_3_ENCODE, 0);
    HAL_Delay(60);
    *A = __HAL_TIM_GET_COUNTER(DJ_1_ENCODE);
    *B = __HAL_TIM_GET_COUNTER(DJ_2_ENCODE);
    *C = __HAL_TIM_GET_COUNTER(DJ_3_ENCODE);
}

void DJ_Step(){
		// nowSpeed = __HAL_TIM_GET_COUNTER(&htim1);
    DJ_Get_Encoder_All(&now_speed1, &now_speed2, &now_speed3);

    now_speed1 = now_speed1<0?-now_speed1:now_speed1;
    now_speed2 = now_speed2<0?-now_speed2:now_speed2;
    now_speed3 = now_speed3<0?-now_speed3:now_speed3;

    if(now_speed3>30000)
    now_speed3 = 0xffff-now_speed3;
    if(Mode_1 == 0 && set_speed1>0 || Mode_1 == 1 && set_speed1<0){
        HAL_GPIO_WritePin(DJ_1_MODEA_GPIOX, DJ_1_MODEA_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DJ_1_MODEB_GPIOX, DJ_1_MODEB_GPIO_PIN, GPIO_PIN_SET);
    }else{
        HAL_GPIO_WritePin(DJ_1_MODEA_GPIOX, DJ_1_MODEA_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(DJ_1_MODEB_GPIOX, DJ_1_MODEB_GPIO_PIN, GPIO_PIN_RESET);
    }
    if(Mode_2 == 0 && set_speed2>0 || Mode_2 == 1 && set_speed2<0){
        HAL_GPIO_WritePin(DJ_2_MODEA_GPIOX, DJ_2_MODEA_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DJ_2_MODEB_GPIOX, DJ_2_MODEB_GPIO_PIN, GPIO_PIN_SET);
    }else{
        HAL_GPIO_WritePin(DJ_2_MODEA_GPIOX, DJ_2_MODEA_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(DJ_2_MODEB_GPIOX, DJ_2_MODEB_GPIO_PIN, GPIO_PIN_RESET);
    }
    
    if(Mode_3 == 0 && set_speed3>0 || Mode_3 == 1 && set_speed3<0){
        HAL_GPIO_WritePin(DJ_3_MODEA_GPIOX, DJ_3_MODEA_GPIO_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DJ_3_MODEB_GPIOX, DJ_3_MODEB_GPIO_PIN, GPIO_PIN_SET);
    }else{
        HAL_GPIO_WritePin(DJ_3_MODEA_GPIOX, DJ_3_MODEA_GPIO_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(DJ_3_MODEB_GPIOX, DJ_3_MODEB_GPIO_PIN, GPIO_PIN_RESET);
    }
    set_speed1 = set_speed1<0?-set_speed1:set_speed1;
    set_speed2 = set_speed2<0?-set_speed2:set_speed2;
    set_speed3 = set_speed3<0?-set_speed3:set_speed3;
    
    PWM1 += PID(now_speed1, set_speed1, AP, AI, AD, last_speed1, &I1);
    if(PWM1>1000) PWM1=1000;
    if(PWM1<0) PWM1=0;
    
    PWM2 += PID(now_speed2, set_speed2, AP, AI, AD, last_speed2, &I2);
    if(PWM2>1000) PWM2=1000;
    if(PWM2<0) PWM2=0;
    
    PWM3 += PID(now_speed3, set_speed3, AP, AI, AD, last_speed3, &I3);
    if(PWM3>1000) PWM3=1000;
    if(PWM3<0) PWM3=0;

    last_speed1 = now_speed1;
    last_speed2 = now_speed2;
    last_speed3 = now_speed3;
    
    DJ_Set_PWM(1, PWM1);
    DJ_Set_PWM(2, PWM2);
    DJ_Set_PWM(3, PWM3);
}

void DJ_Set_Speed(uint8_t DJ, int speed){
    switch(DJ){
        case 1:
            set_speed1 = speed;
            break;
            
        case 2:
            set_speed2 = speed;
            break;
            
        case 3:
            set_speed3 = speed;
            break;
    }
}

void DJ_Set_Speed_All(int A, int B, int C){
    set_speed1 = A;
    set_speed2 = B;
    set_speed3 = C;
}

int getSpeed(uint8_t DJ){
    switch(DJ){
        case 1:
            return now_speed1;
            break;
            
        case 2:
            return now_speed2;
            break;
            
        case 3:
            return now_speed3;
            break;
    }
		return 0;
}