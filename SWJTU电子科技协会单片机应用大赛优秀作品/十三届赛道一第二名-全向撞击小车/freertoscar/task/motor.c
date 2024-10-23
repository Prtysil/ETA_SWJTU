#include "motor.h"

/*变量区*/
float  PID_1[3] = {64.7f, 11.5f, 0.0f};
float  PID_2[3] = {64.7f, 11.5f, 0.0f};
float  PID_3[3] = {64.7f, 11.3f, 0.0f};
pid_type_def pid_1, pid_2, pid_3;
float Vx_set, Vy_set, Vw_set                = 0.0f;
float speed_set_1, speed_set_2, speed_set_3 = 0.0f;
uint8_t resert_flag = 0;// 0:正常 1:停止
int pid_out_1, pid_out_2, pid_out_3 = 0;//PID输出
short num_1, num_2, num_3           = 0;
float speed_1, speed_2, speed_3      = 0.0f;//速度
uint8_t xz_flag              = 0;
uint8_t angle_set           = 0;
float speed_all_1, speed_all_2, speed_all_3 = 0.0f;

/*函数区*/
void Pid_task(void *argument)
{    
    TickType_t xLastWakeTime;
    const TickType_t xDelayms = pdMS_TO_TICKS(50);
    xLastWakeTime             = xTaskGetTickCount();
    for (;;) 
    {    
         Motor_Counter(Vx_set, Vy_set, Vw_set);
         Speed_Counter();
        vTaskDelayUntil(&xLastWakeTime, xDelayms);
    }
}


/// @brief 进行编码器，定时器 PID的初始化
/// @param  void
void Motor_Init(void)
{
    PID_init(&pid_1, PID_DELTA, PID_1, 8400, 4200); 
    PID_init(&pid_2, PID_DELTA, PID_2, 8400, 4200);
    PID_init(&pid_3, PID_DELTA, PID_3, 8400, 4200);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET);
}

/// @brief 进行三个轮子的速度的解算
/// @param Vx_set x方向的速度，向前为正，向后为负
/// @param Vy_set y方向的速度，向右为正，向左为负
/// @param Vw_set 旋转的速度，顺时针为正，逆时针为负
void Motor_Counter(float Vx_set,float Vy_set,float Vw_set)
   {
    
     speed_set_3 = -Vx_set + Vy_set *0+ Vw_set * L;
     speed_set_1 = -Vy_set * 0.866f +Vx_set * 0.5f + Vw_set * L;//0.866025=cos30,0.5=cos60
     speed_set_2 = Vy_set * 0.866f +Vx_set * 0.5f + Vw_set * L;

    if (speed_set_1 > 0) {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
    } else if (speed_set_1 < 0) {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
    }
    else if (speed_set_1==0)
    {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
    }
    if (speed_set_2 > 0) {
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
    } else if (speed_set_2 < 0) {
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
    }
    else if (speed_set_2==0)
    {
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
    }
    if (speed_set_3 > 0) {
        HAL_GPIO_WritePin(CIN1_GPIO_Port, CIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(CIN2_GPIO_Port, CIN2_Pin, GPIO_PIN_SET);
    } else if (speed_set_3 < 0) {
        HAL_GPIO_WritePin(CIN1_GPIO_Port, CIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(CIN2_GPIO_Port, CIN2_Pin, GPIO_PIN_RESET);
    }
    else if (speed_set_3==0)
    {
        HAL_GPIO_WritePin(CIN1_GPIO_Port, CIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(CIN2_GPIO_Port, CIN2_Pin, GPIO_PIN_RESET);
    }
}

/// @brief 编码器的计数，速度的计算，PID的计算，PWM的输出
/// @param  void
void Speed_Counter(void)
{
    num_1 = __HAL_TIM_GET_COUNTER(&htim1);
    num_2 = __HAL_TIM_GET_COUNTER(&htim2);
    num_3 = __HAL_TIM_GET_COUNTER(&htim3);
    __HAL_TIM_SetCounter(&htim1, 0);
    __HAL_TIM_SetCounter(&htim2, 0);
    __HAL_TIM_SetCounter(&htim3, 0);
    speed_1   = (float)(num_1 / 0.05 / 937.2 * pi * 14);
    speed_2   = (float)(num_2 / 0.05 / 937.2 * pi * 14);
    speed_3   = (float)(num_3 / 0.05 / 937.2 * pi * 14);
    if(angle_set!=0)
    {
        Rotate_Counter(angle_set);
    }
    pid_out_1 = (int)PID_rela(PID_calc(&pid_1, PID_rela(speed_1), PID_rela(speed_set_1)));
    pid_out_2 = (int)PID_rela(PID_calc(&pid_2, PID_rela(speed_2), PID_rela(speed_set_2)));
    pid_out_3 = (int)PID_rela(PID_calc(&pid_3, PID_rela(speed_3), PID_rela(speed_set_3))); 
    if (resert_flag == 1)
			  {
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
			  } 
			else if (resert_flag == 0)
		{
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pid_out_1);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pid_out_2);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, pid_out_3);
        }
}


/// @brief 沿一定的角度运动
/// @param angle 设定的角度
/// @param speed 设定的速度
void Angle_Speed(uint16_t angle,float speed)
{
    if (angle == 0) 
    {
        Vx_set = Vy_set = 0;
    }
    Vx_set = speed * cos(angle * pi / 180);
    Vy_set = speed * sin(angle * pi / 180);
}

void Rotate_Counter(uint8_t angle_set)
{
    float speed_all_set;
	speed_all_set= (float)angle_set / 180 * pi * L*1.70f;
    speed_all_1 += speed_1 * 0.05f;
    speed_all_2 += speed_2 * 0.05f;
    speed_all_3 += speed_3 * 0.05f;
    if((speed_all_1 > speed_all_set)&&(speed_all_2 > speed_all_set)&&(speed_all_3 > speed_all_set))
    {
        speed_all_1 = speed_all_2 = speed_all_3 = 0;
        angle_set = 0;
				Vx_set=Vy_set=Vw_set=0;
    }
    else
    { 
        resert_flag = 0;
    }
}


