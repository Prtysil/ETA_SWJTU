/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_spi_130.h"
#include "dj.h"
#include "stdio.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "arm_math.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "HCSR04.h"
#include "servo.h"
//#include "mpu_math.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint16_t Time;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_1)
	{
		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)==GPIO_PIN_SET)
		{
			__HAL_TIM_SetCounter(&htim6,0);
			HAL_TIM_Base_Start(&htim6);
		}
		else 
		{
			Time=__HAL_TIM_GetCounter(&htim6);
			HAL_TIM_Base_Stop(&htim6);
		}
	}
}
//void HAL_Delay_us(uint32_t nus);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
uint8_t getAim();
void ctr_mode(uint32_t sw_data);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int bug;
long long z_add=0;
short aacx,aacy,aacz;			//加速度传感器原始数据
short gyrox,gyroy,gyroz;		//陀螺仪原始数据
short az, az_i;
long long az_j=0;
extern float Yaw,Pitch,Roll;
int speed1=0, speed2=0, speed3=0;
int row=0;
uint8_t azH, azL;
float temp;					    //温度
void sw(uint32_t sw_data);
int distance;
extern int now_speed1, now_speed2, now_speed3;
uint16_t Angle[180]={0};
uint8_t aimAng=0, aimA=0;
int ctr_Flag = 0;

uint8_t getAim(){
	uint8_t start=0, end=0,min_distance=Angle[20];
	for(uint8_t ii=20; ii<160; ii++){
		if(min_distance>Angle[ii])
		{
			min_distance=Angle[ii];
		}
	}
	for(uint8_t ii=0;ii<180;ii++)
	{
		if(!start)
		{
			if(Angle[ii]>=min_distance&&Angle[ii]<=min_distance+10)
			{
				start=ii;
			}
		}
		else 
		{
			if(Angle[ii]>=min_distance&&Angle[ii]<=min_distance+10)
			{
				end=ii;
			}
		}
	}
//		if(!start){
//			if(Angle[ii]>=5 && Angle[ii]<=60 ){
//				start = ii;
//			}
//		}
//		if(Angle[ii]>=5 && Angle[ii]<=30){
//			end = ii;
//		}
//	}
	return (start + end)/2;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  MX_TIM15_Init();
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
  HAL_NVIC_SetPriority(SysTick_IRQn, 1, 0);
  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 2, 0);
  HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 3, 0);
	SPI_LCD_Init();  // LCD Init
	MPU_Init();
	HCSR04_Init();
// HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_Base_Start(&htim8);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);

  // HAL_TIM_Base_Start(&htim1);
  
  //HAL_TIM_Base_Start(&htim1);
	
  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

  LCD_SetTextFont(&CH_Font24);
	LCD_SetAsciiFont(&ASCII_Font24);

	
	//LCD_DrawCircle(0, 160, 80);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  DJ_All_Init();
	DJ_Set_Speed_All(200, 200, 200);
  DJ_Set_Mode(1, 0);
  DJ_Set_Mode(2, 0);
  DJ_Set_Mode(3, 0);
  char *sp1, *sp2, *sp3;
  int k=0;
  MPU_Read_Fast(0x47, &azH);
  MPU_Read_Fast(0x48, &azL);
  az_i = (azH << 8) | azL;
	HAL_Delay(100);
	
	HAL_TIM_Base_Start_IT(&htim1);
  //HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_3);

  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  //HAL_TIM_Base_Start(&htim7);
	
  HAL_TIM_Base_Start_IT(&htim12);
//  HCSR04_Init();
	
  // servo_setAngle(Angle[180]);
	
  //__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,45*80/3);
//  HAL_TIM_Base_Stop_IT(&htim12);
//  servo_setAngle(90);//调90度
//DJ_Set_Speed_All(0,0,0);
	  
  while (1)
  {
	   //0:???? 1:?????????
	if(ctr_Flag == 1){
	  mpu_reset_fifo();
	  if(aimAng){
			aimA = aimAng;
		az_j=(aimAng-90)*808.0;
		aimAng = 0;
	  }
	  row = (az_j-z_add)/200.0;
	  DJ_Set_Mode(2,1);
	  DJ_Set_Mode(3,0);
	  DJ_Set_Mode(1,0);
	  DJ_Set_Speed_All(100,100-row/2.0,100);
	  DJ_Step();
	}	
	else{
		row = (az_j-z_add)/200.0;
	  DJ_Set_Mode(2,0);
	  DJ_Set_Mode(3,0);
	  DJ_Set_Mode(1,0);
		if(speed1==0 && speed2==0 && speed3==0){
			DJ_Set_Speed_All(row/2,row/2,row/2);
		}else{
			DJ_Set_Speed_All(speed1,speed2+row/2.0, speed3);
		}
	  
	  DJ_Step();
	}
	  
//		if(k==10){
//			 mpu_reset_fifo();
//			HAL_Delay(4);
			//while(bug = mpu_dmp_get_data(&pitch, &roll, &yaw));			//必须要用while等待，才能读取成功
			//MPU_Get_Accelerometer(&aacx,&aacy, &aacz);		//得到加速度传感器数据
//      row = (z_add-az_j)/1000.0;

//      DJ_Set_Speed_All(speed1, speed2, speed3+row);
			
		// DJ_Set_Speed(3, 200 + z_add/500.0);
			//temp=MPU_Get_Temperature();						//得到温度信息
		
//		DJ_Step();
/*start
	  //在运动过程中，每次超声波检测到障碍物，将目标方向指向障碍物方向
	  //
	  红外光遥控进入此程序
	  int main()
	  {
		初始化
		打开舵机定时器的时钟
		while(!getAim());//等待超声波获得障碍物方向
		while(1)
		{
			mpu_reset_fifo();
			if(!getAim())
			{
				az_j=getaim(); 
			}
			DJ_Set_Speed_All(speed1,speed2,row);
			row=(z_add-az_j)/1000.0;	
			DJ_Step();
		}
	  }
end*/	  
    
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t NEC_Start = 0, NEC_Mode=0;
uint16_t CR1=0, CR2=0;
uint16_t time1=0;
uint32_t time2=0;
uint8_t NEC_Data_P=0;
uint32_t NEC_Data=0, NEC_Last_Data=0;
int x2, y2;
int nowAng = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim->Instance == TIM1){
	  if(NEC_Mode){
      NEC_Start ++;
      // if(NEC_Start>=2){
      //   NEC_Mode = 0;
      // }
      if(NEC_Mode==1 && NEC_Start==2){
        NEC_Mode = 0;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
      }
      if((NEC_Mode==2||NEC_Mode==3) && NEC_Start==7){
        NEC_Mode = 0;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
      }
    }
	  
    MPU_Read_Fast(0x47, &azH);
    MPU_Read_Fast(0x48, &azL);
    az = (azH << 8) | azL;
    if(az - az_i>5|| az - az_i<-5){
      z_add += az - az_i;
    }
    
    
  }
  if(htim == (&htim12)){
		//LCD_ClearRect(5, 144, 120, 86);
		
    LCD_SetBackColor(LCD_BLACK);
		LCD_SetColor(LCD_WHITE);
		//distance=HCSR04_GetValue();
		LCD_DisplayNumber(1,1,aimA,3);
		LCD_DisplayNumber(5, 120+24, getSpeed(1), 4);
		LCD_DisplayNumber(5, 120+48, getSpeed(2), 4);
		LCD_DisplayNumber(5, 120+72, getSpeed(3), 4);
		
		LCD_DisplayNumber(80, 120+24, (int)z_add, 4);
		LCD_DisplayNumber(80, 120+48, (int)row, 4);
		LCD_DisplayNumber(80, 120+72, (int)aimAng, 3);
		

		LCD_DisplayNumber(160, 120+24, speed1, 4);
		LCD_DisplayNumber(160, 120+48, speed2, 4);
		LCD_DisplayNumber(160, 120+72, speed3, 4);
		
			//if(Angle[ids]>=39) Angle[ids] = 39;
      

		// LCD_DisplayNumber(100, 120+48, (int)Pitch, 4);
		// LCD_DisplayNumber(100, 120+72, (int)Roll, 4);
	//	LCD_DisplayNumber(100, 120+86, (int)temp, 4);
		
////    LCD_DisplayText(5, 120+48, sp2);
////    LCD_DisplayText(5, 120+72, sp3);
    
    //LCD_DrawHalfCircle(120, 120, 120, 1);
    if(nowAng >=0){
      distance = HCSR04_GetValue();
			if(distance >120)
				distance=120;
			Angle[nowAng] = distance;
			LCD_DrawPoint(120-cos((nowAng)*3.1415926/180)*distance, 120-sin((nowAng)*3.1415926/180)*distance, LCD_WHITE);
//			if(nowAng<180){
//				LCD_SetColor(LCD_BLACK);
//				LCD_DrawLine(120, 120, x2, y2);
//				if(nowAng){
//					x2 = 120-cos((nowAng)*3.1415926/180)*120;
//					y2 = 120-sin((nowAng)*3.1415926/180)*120;
//					LCD_DrawPoint(120-cos((nowAng-1)*3.1415926/180)*Angle[nowAng-1], 120-sin((nowAng-1)*3.1415926/180)*Angle[nowAng-1], LCD_WHITE);
//				}else{
//					LCD_DrawPoint(120-cos((180)*3.1415926/180)*Angle[179], 120-sin((180)*3.1415926/180)*Angle[179], LCD_WHITE);
//				}
//				LCD_SetColor(LCD_GREEN);
//				LCD_DrawLine(120, 120, x2, y2);
//			}
						
//			LCD_DrawPoint(120-cos((nowAng-1)*3.1415926/180)*distance, 120-sin((nowAng-1)*3.1415926/180)*distance, LCD_WHITE);
      nowAng++;
      if(nowAng>=180){
				LCD_Clear();
        nowAng = -50;
        servo_setAngle(180);
	   aimAng = getAim();
      }else{
        servo_setAngle(180-nowAng);
      }
    }else{
      nowAng++;
    }
    /*start

    uint8_t start=0 end=0;
    for(uint8_t i=0;i<180;i++)
    {
		servo_setAngle(i);
		distance=HCSR04_GetValue();
		if(!start)
		{
		    if(distance>=10&&distance<=60)
		    start=i;
		}
		if(distance>=10&&distance<=60)
		{
			end=i;
		}
    }
    AimAngle=(start+end)/2;
    end*/
  }
}
void ctr_mode(uint32_t sw_data){
    if(sw_data == 0xEA15FF00){  //妯″紡1
        ctr_Flag = 0;
    }

    else if(sw_data == 0xF609FF00){  //妯″紡2
        ctr_Flag = 1;
			z_add = 0;
    }

    if(ctr_Flag == 0){
        if(sw_data == 0xAD52FF00){ //2 鍓嶈繘
            speed1 =  -100;
            speed2 =  100  ;
            speed3 =  -100 ;
        }
        else if(sw_data == 0xE718FF00){  //8 鍚庨��
            speed1 = 100 ;
            speed2 = -100 ;
            speed3 = 100  ;
        }
        else if(sw_data == 0xA55AFF00){  //4 宸﹁浆 
            az_j+=10000;
        } 
        else if(sw_data == 0xF708FF00){  //6 鍙宠浆
            az_j-=10000;
        }else if(sw_data == 0xE31CFF00){
			speed1 = 0;
			speed2 = 0;
			speed3 = 0;
		}
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
  if(htim->Instance == TIM1){
    if(NEC_Mode==0){
      CR1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      NEC_Mode=1;
      NEC_Start=0;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
    }else if(NEC_Mode==1){
      CR2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      time1 = CR2 - CR1 + NEC_Start*20000;
      if(time1 > 8000 && time1 < 10000){
        NEC_Mode = 2;  // 2卤韸韭柯寂惵断惻号结姼
        NEC_Data=0;
        NEC_Data_P=0;
      }else{
        NEC_Mode = 0;  // 袇潞糯韼陈Ｂ结姼
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
      }
    }else if(NEC_Mode==2){
      CR1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      NEC_Start = 0;
      NEC_Mode=3;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }else if(NEC_Mode==3){
      CR2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      time2 = CR2 - CR1 + NEC_Start*20000;
      if(time2<800){
        NEC_Data &= ~(((uint32_t)1)<<NEC_Data_P);
        NEC_Data_P++;
      }else if(time2<6000){
        NEC_Data |= (((uint32_t)1)<<NEC_Data_P);
        NEC_Data_P++;
      }else if(time2>90000){
        NEC_Mode = 0;//闀挎寜
        NEC_Data_P = 0;
        ctr_mode(NEC_Last_Data);
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
      }else{
				NEC_Mode = 0;
        NEC_Data_P = 0;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
      if(NEC_Data_P>31){
         NEC_Last_Data = NEC_Data;//鐭寜
        //sw(NEC_Data);
        NEC_Mode = 0;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
      }else{
        NEC_Mode=2;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      }
    }
	}
}

		// if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_SET && NEC_Mode==1){  // 计算拉低时间
    //   CR2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    //   time1 = CR2 - CR1 + NEC_Start*20000;  //1us
    //   if(time1 > 8000 && time1 < 10000){
    //     NEC_Mode = 2;  // 2表示开始判断信号结束
    //     NEC_Data=0;
    //   }else{
    //     NEC_Mode = 0;  // 信号错误，结束
    //   }
    // }else if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_SET && (NEC_Mode==2 || NEC_Mode==3)){  // 计算拉低时间
    //   CR2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    //   NEC_Mode = 3;
    // }

    // if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_RESET && NEC_Mode==0){  // 拉低触发
    //   NEC_Mode = 1;  // 1表示开始判断信号起始 9usL, 4.5usH;
    //   NEC_Start = 0;
    //   NEC_Data_P = 0;
    //   CR1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    // }else if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_RESET && (NEC_Mode==2 || NEC_Mode==3)){
    //   if(NEC_Mode==3){
    //     CR3 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    //     time1 = CR2 - CR1 + NEC_Start*20000;  //1us
    //     time2 = CR3 - CR1 + NEC_Start*20000;  //1us
    //     time2 -= time1;
    //     // 500-620<-560  1500-1860<-1680
    //     if(time1<800 && time1>300){
    //       if(time2<800 && time2>300){
    //         NEC_Data &= ~(((uint32_t)1)<<NEC_Data_P);
    //         NEC_Data_P++;
    //       }else if(time2<2000&&time2>1300){
    //         NEC_Data |= (((uint32_t)1)<<NEC_Data_P);
    //         NEC_Data_P++;
    //       }else{
    //         NEC_Mode = 0;
    //         NEC_Data_P=0;
    //       }
    //       if(NEC_Data_P>31){
    //         NEC_Mode = 0;
    //         NEC_Data_P=0;
    //       }
    //     }else{
    //       NEC_Mode = 0;
    //       NEC_Data_P=0;
    //     }
    //   }
    //   NEC_Start=0;
    //   CR1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    // }
//void HAL_Delay_us(uint32_t nus)
//{
//	_nop_();
//}
void _nop_(){
	
}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
