#include "UT.h"

/*变量区*/
int time            = 0;
float distance=0;//超声波测距距离
float distances[41] = {0};
uint16_t angle_val         = 0;
float angle           = 0;//最近障碍角度
uint8_t servo_flag=0;


/*函数区*/
void UT_Task(void *argument)
{
    TickType_t xLastWakeTime;
    const TickType_t xDelayms = pdMS_TO_TICKS(50);
    xLastWakeTime             = xTaskGetTickCount();
    for (;;) {
        if (mood == 1) 
        {
            UT_Test();
        }
        vTaskDelayUntil(&xLastWakeTime, xDelayms);
    }
}


/// @brief 开启超声波测距
void trigger_start(void)
{
    HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
    delay_us(20);
    HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
}


/// @brief 舵机的初始化
void Servo_Init(void)
{
  HAL_TIM_PWM_Start(&htim10,TIM_CHANNEL_1);
  __HAL_TIM_SetCompare(&htim10,TIM_CHANNEL_1,10);
}

/// @brief 舵机的设置与超声波距离的比较
void Servo_Set(void)
{
    uint16_t z     = 0;
    float smallest = 0;
    for (angle_val = 10; angle_val < 51; angle_val++) {
        __HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, angle_val);
        vTaskDelay(20);
        trigger_start();
        vTaskDelay(120);
    }
    smallest = distances[0];
    for (z = 0; z < 41; z++) {
        if (distances[z] < smallest) {
            smallest = distances[z];
            angle    = z * 4.5;
        }
    }
    if (angle) {
        Buzz_Start(); // 找到最小值后蜂鸣器响一声
        servo_flag = 1;
    }
}

/// @brief 蜂鸣器响一声
void Buzz_Start(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    delay_ms(50);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}


void UT_Test(void)
{
    if (mood == 1)
     {
        if (servo_flag == 0&&mood==1) 
        {
            angle = 0;
            Servo_Set();//开始测距
        } 
        else if (servo_flag == 1&&mood==1) 
        {
            __HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, ((uint8_t)(angle /4.50) + 10)); // 舵机转到对应的角度
            vTaskDelay(20);
            trigger_start();//开始测距
            Angle_Speed(angle, 50);//向对应角度前进
            if (distance < 15) 
            {
                Buzz_Start();//到达位置蜂鸣器响一声
                Vx_set = Vy_set = 0;
                servo_flag      = 0;
                angle           = 0;
                mood            = 0;
            }
        }
    }
}


