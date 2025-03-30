/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "oled.h"
#include "font.h"
#include "key_led.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t pulseWidth = 50;   //脉宽
uint8_t dirInc = 1;         //脉宽变化方向�??????????1表示递增�??????????0表示递减
char str[10];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
char *NumToStr(int16_t num,char *str);
  /* USER CODE END 1 */

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
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
OLED_Init();
OLED_NewFrame();
OLED_PrintString(0,0,"PWM Output:",&font16x16,OLED_COLOR_NORMAL);
OLED_PrintString(0,16,"Width=",&font16x16,OLED_COLOR_NORMAL);
OLED_PrintString(0,32,"Pulse=",&font16x16,OLED_COLOR_NORMAL);
//OLED_PrintInt(48,48,-100,&afont16x8,OLED_COLOR_NORMAL);
//OLED_PrintString(0,48,"yeah!",&font16x16,OLED_COLOR_NORMAL);
OLED_ShowFrame();

HAL_TIM_Base_Start_IT(&htim1);
HAL_TIM_Base_Start(&htim3);
HAL_TIM_PWM_Start_IT(&htim1,TIM_CHANNEL_1);   //启动TIM1_CH1，生成PWM
HAL_TIM_OC_Start(&htim1,TIM_CHANNEL_2);       //启动TIM1_CH2的输出比较功�??
HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);    //启动TIM2_CH1的IC(输入)功能
HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);    //启动TIM2_CH2的IC(输入)功能

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    OLED_PrintString(0,48,"OK?",&font16x16,OLED_COLOR_NORMAL);
    KEYS curKey = ScanPressedKey(KEY_WAIT_ALWAYS);                //有问题：必须在TIM3_CH1未有输入时，才能成功扫描到按键状�??
    OLED_PrintString(24,48,"OK!",&font16x16,OLED_COLOR_NORMAL);
    uint32_t CCR = __HAL_TIM_GET_COMPARE(&htim1,TIM_CHANNEL_1);
    //对CCR限幅
    if (CCR <= 5)
    { CCR = 10;}
    else if (CCR >= 495)
    { CCR = 495;}

    if (curKey == KEY_DOWN)
    {
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,CCR - 5);
      OLED_PrintString(48,48,"KEY_DOWN",&font16x16,OLED_COLOR_NORMAL);
    }
    else if (curKey == KEY_UP)
    {
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,CCR + 5);
      OLED_PrintString(48,48,"KEY_UP",&font16x16,OLED_COLOR_NORMAL);
    }
    else if (curKey == KEY_NONE)
    {
      OLED_PrintString(48,48,"KEY_NONE",&font16x16,OLED_COLOR_NORMAL);
    }
    HAL_Delay(300);   //消除按键后抖动影�??
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
char *NumToStr(int16_t num,char *str)
{
  int8_t i = 0;
  int16_t temp = num;
  if (num < 0) {return NULL;}   //先不管负数的处理
  while(temp > 0)
  {
    str[i] = temp%10;
    temp /= 10;
    i++;
  }
  str[i] = '\0';
  return str;
}

// void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)   //使用该回调函数要启用Tim1 update interrupt
// {
//   //OLED_PrintString(0,48,"go into PWM_CB",&font16x16,OLED_COLOR_NORMAL);
//   if (htim->Instance == TIM1)
//   {
//     if (dirInc == 1)      //脉宽递增
//     {
//       //OLED_PrintString(0,48,"go into dirInc=1",&font16x16,OLED_COLOR_NORMAL);
//       pulseWidth++;
//       if (pulseWidth >= 195)
//       {
//         pulseWidth = 195;
//         dirInc = 0;
//       }
//     }
//     else                  //脉宽递减
//     {
//       //OLED_PrintString(0,48,"go into dirInc=0",&font16x16,OLED_COLOR_NORMAL);
//       pulseWidth--;
//       if (pulseWidth <= 5)
//       {
//         pulseWidth = 5;
//         dirInc = 1;
//       }
//     }
//     __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pulseWidth);
//     OLED_PrintASCIIChar(88,32,pulseWidth/100+'0',&afont16x8,OLED_COLOR_NORMAL);
//     OLED_PrintASCIIChar(96,32,pulseWidth/10%10+'0',&afont16x8,OLED_COLOR_NORMAL);
//     OLED_PrintASCIIChar(104,32,pulseWidth%10+'0',&afont16x8,OLED_COLOR_NORMAL);
//     OLED_ShowFrame();
//   }
// }

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  uint16_t IC1_Width = __HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_1);
  uint16_t IC2_Pulse = __HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_2);
  if ((IC1_Width == 0) || (IC2_Pulse == 0))
  { return; }

  OLED_PrintASCIIChar(48,16,IC1_Width/100+'0',&afont16x8,OLED_COLOR_NORMAL);
  OLED_PrintASCIIChar(56,16,IC1_Width/10%10+'0',&afont16x8,OLED_COLOR_NORMAL);
  OLED_PrintASCIIChar(64,16,IC1_Width%10+'0',&afont16x8,OLED_COLOR_NORMAL);

  OLED_PrintASCIIChar(48,32,IC2_Pulse/100+'0',&afont16x8,OLED_COLOR_NORMAL);
  OLED_PrintASCIIChar(56,32,IC2_Pulse/10%10+'0',&afont16x8,OLED_COLOR_NORMAL);
  OLED_PrintASCIIChar(64,32,IC2_Pulse%10+'0',&afont16x8,OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}
/* USER CODE END 4 */

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
