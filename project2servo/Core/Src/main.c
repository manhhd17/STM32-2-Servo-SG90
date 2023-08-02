/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
char rota1[20] = {0};
char rota2[20] = {0};
int PC_rota1 = 0;
int PC_rota2 = 45;
// int a = 25;
// int b = 60;
// int PC_rota1 = 0;
// int PC_rota2 = 60;
enum
{
  Mode_auto,
  Mode_manual
};

volatile int Mode = Mode_auto;

uint8_t Rx_data;
char buffer_data[50] = {0};
volatile int Rx_index = 0;
volatile int Rx_complete = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void log_data(char *buf)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)buf, strlen(buf), 100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
    if (Rx_data == 13)
    {
      buffer_data[Rx_index] = '\0';
      Rx_index = 0;
      Rx_complete = 1;
      if (strcmp(buffer_data, "manual") == 0)
      {
        Mode = Mode_manual;
      }

      if (strcmp(buffer_data, "auto") == 0)
      {
        Mode = Mode_auto;
      }
    }
    else
    {
      buffer_data[Rx_index] = (char)Rx_data;
      Rx_index++;
      // log_data((char*)Rx_data);
    }
    HAL_UART_Receive_IT(&huart1, &Rx_data, 1);
  }
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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  log_data("duma");
  HAL_UART_Receive_IT(&huart1, &Rx_data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    switch (Mode)
    {
    case Mode_auto:
      // if (strcmp(buffer_data, "stop") == 0)
      // {
      //   TIM1->CCR1 = a;
      //   TIM2->CCR2 = b;
      //   break;
      // }
      while (PC_rota1 <= 180)
      {
        if (strcmp(buffer_data, "stop") == 0)
        {
          TIM1->CCR1 = (int)((PC_rota1 + 45) / 1.8);
          TIM2->CCR2 = (int)((PC_rota2 + 45) / 1.8);
          HAL_Delay(100);
          break;
        }
        TIM1->CCR1 = (int)((PC_rota1 + 45) / 1.8);
        TIM2->CCR2 = (int)((PC_rota2 + 45) / 1.8);
        HAL_Delay(10);
        PC_rota1++;
        if (PC_rota1 > 180)
        {
          PC_rota2 += 10;
          break;
        }
      }

      while (PC_rota1 >= 0)
      {
        if (strcmp(buffer_data, "stop") == 0)
        {
          TIM1->CCR1 = (int)((PC_rota1 + 45) / 1.8);
          TIM2->CCR2 = (int)((PC_rota2 + 45) / 1.8);
          HAL_Delay(100);
          break;
        }
        TIM1->CCR1 = (int)((PC_rota1 + 45) / 1.8);
        TIM2->CCR2 = (int)((PC_rota2 + 45) / 1.8);
        HAL_Delay(10);
        PC_rota1 -= 1;
        if (PC_rota1 < 0)
        {
          PC_rota2 += 10;
          break;
        }
      }
      if (PC_rota2 >= 135)
        PC_rota2 = 45;
      //   if (strcmp(buffer_data, "stop") == 0) {
      //     break;
      // }

    case Mode_manual:
      // if (strcmp(buffer_data, "stop") == 0)
      // {
      //   TIM1->CCR1 = a;
      //   TIM2->CCR2 = b;
      // }
      if (Rx_complete)
      {
        Rx_complete = 0;
        log_data("\noke");
        // if (strcmp(buffer_data, "30") == 0)
        // {
        // Parse string
        // sscanf(buffer_data, "%*[^0-9]%d", &PC_rota);

        // char *token = strtok(buffer_data, "&");
        int PC_rota1 = atoi(strtok(buffer_data, "&"));

        int PC_rota2 = atoi(strtok(NULL, "/"));
        // control Servo
        TIM1->CCR1 = (int)((PC_rota1 + 45) / 1.8);
        // sprintf(rota1, "Rota: %d  ", PC_rota1);
        HAL_Delay(100);
        TIM2->CCR2 = (int)((PC_rota2 + 45) / 1.8);
        // sprintf(rota2, "Rota: %d  ", PC_rota2);
        // memset(buffer_data, '\0', sizeof(buffer_data));
      }
    }
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

#ifdef USE_FULL_ASSERT
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
