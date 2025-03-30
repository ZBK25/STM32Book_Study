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
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "font.h"
#include <stdio.h>    //用到sprintf()
#include <string.h>   //用到了strcpy()
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
uint16_t triggerCntA = 0;   //闹钟A中断次数
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
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim1);
  OLED_Init();
  OLED_NewFrame();
  OLED_PrintASCIIString(0,0,"Date=",&afont16x8,OLED_COLOR_NORMAL);
  OLED_PrintASCIIString(0,16,"Day=",&afont16x8,OLED_COLOR_NORMAL);
  OLED_PrintASCIIString(0,32,"Time=",&afont16x8,OLED_COLOR_NORMAL);
  OLED_PrintASCIIString(0,48,"AlarmA times:0",&afont16x8,OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  triggerCntA++;
  OLED_PrintInt(104,48,triggerCntA,&afont16x8,OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}

// void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
// {
//   RTC_TimeTypeDef sTime;
//   RTC_DateTypeDef sDate;

//   if (HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK)
//   {
//     HAL_RTC_GetDate(hrtc,&sDate,RTC_FORMAT_BIN);
//     //显示日期：年-�?-�?
//     char str[40];
//     sprintf(str,"Date=%4d-%2d-%2d",2000+sDate.Year,sDate.Month,sDate.Date);
//     OLED_PrintASCIIString(0,0,str,&afont16x8,OLED_COLOR_NORMAL);
//     //显示时间：hh:mm:ss
//     sprintf(str,"Time=%2d:%2d:%2d",sTime.Hours,sTime.Minutes,sTime.Seconds);
//     OLED_PrintASCIIString(0,32,str,&afont16x8,OLED_COLOR_NORMAL);
//     OLED_ShowFrame();
//   }
// }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1)
  {
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    //读取时间和日期，必须都读取出来，否则无法解锁，就不能连续读取
    if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK)
    {
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      //显示日期；年-月-日
      char str[40];
      sprintf(str, "Date=%4d-%02d-%02d", 2000+sDate.Year,sDate.Month,sDate.Date);
      OLED_PrintASCIIString(0,0,str,&afont16x8,OLED_COLOR_NORMAL);
      //显示时间：hh:mm:ss
      sprintf(str, "Time=%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
      OLED_PrintASCIIString(0,32,str,&afont16x8,OLED_COLOR_NORMAL);
      //显示星期
      char weekday[15];
      switch (sDate.WeekDay)
      {
      case RTC_WEEKDAY_SUNDAY:
        strcpy(weekday,"Sunday");
        break;
      case RTC_WEEKDAY_MONDAY:
        strcpy(weekday,"Monday");
        break;
      case RTC_WEEKDAY_TUESDAY:
        strcpy(weekday,"Tuesday");
        break;
      case RTC_WEEKDAY_WEDNESDAY:
        strcpy(weekday,"Wednesday");
        break;
      case RTC_WEEKDAY_THURSDAY:
        strcpy(weekday,"Thursday");
        break;
      case RTC_WEEKDAY_FRIDAY:
        strcpy(weekday,"Friday");
        break;
      case RTC_WEEKDAY_SATURDAY:
        strcpy(weekday,"Saturday");
        break;
      default:
        break;
      }
      sprintf(str, "Day=%s",weekday);
      OLED_PrintASCIIString(0,16,str,&afont16x8,OLED_COLOR_NORMAL);

      OLED_ShowFrame();
    }
  }
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
