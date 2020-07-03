/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Tx433Lin.h"
#include "Control.h"
#include "oled.h"
#include "Tx433Remote.h"
#include "keeloq_lock.h"
#include "bmp.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/*printf*/
#if 1
int _write(int fd, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 0xFFFF);
    return len;
}
#endif
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
    u8 t;
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
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
//    OLED_Init();
//    OLED_ColorTurn(0);//0æ­£å¸¸æ˜¾ç¤ºï¼?1 åè‰²æ˜¾ç¤º
//    OLED_DisplayTurn(0);//0æ­£å¸¸æ˜¾ç¤º 1 å±å¹•ç¿»è½¬æ˜¾ç¤º
//    t=' ';
    NLF_DEFINE();
    AskInit();
    Tx433IrInit();
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


    KeyMenu();
   // HAL_UART_Transmit(&huart1,"\r\nabc\r\n",7,10);
//    printf(" 123456qwe !@#$\r\n");
//    HAL_Delay(500);
/*      OLED_ShowPicture(0,0,128,8,BMP1);
      HAL_Delay(500);
      OLED_Clear();
      OLED_ShowChinese(0,0,0,16);//ä¸?
      OLED_ShowChinese(18,0,1,16);//æ™?
      OLED_ShowChinese(36,0,2,16);//å›?
      OLED_ShowChinese(54,0,3,16);//ç”?
      OLED_ShowChinese(72,0,4,16);//å­?
      OLED_ShowChinese(90,0,5,16);//ç§?
      OLED_ShowChinese(108,0,6,16);//æŠ?
      OLED_ShowString(8,16,"ZHONGJINGYUAN",16);
      OLED_ShowString(20,32,"2014/05/01",16);
      OLED_ShowString(0,48,"ASCII:",16);
      OLED_ShowString(63,48,"CODE:",16);
      OLED_ShowChar(48,48,t,16);//æ˜¾ç¤ºASCIIå­—ç¬¦
      t++;
      if(t>'~')t=' ';
      OLED_ShowNum(103,48,t,3,16);
      OLED_Refresh();
      HAL_Delay(500);
      OLED_Clear();
      OLED_ShowChinese(0,0,0,16);  //16*16 ä¸?
      OLED_ShowChinese(16,0,0,24); //24*24 ä¸?
      OLED_ShowChinese(24,20,0,32);//32*32 ä¸?
      OLED_ShowChinese(64,0,0,64); //64*64 ä¸?
      OLED_Refresh();
      HAL_Delay(500);
      OLED_Clear();
      OLED_ShowString(0,0,"ABC",12);//6*12 â€œABCâ€?
      OLED_ShowString(0,12,"ABC",16);//8*16 â€œABCâ€?
      OLED_ShowString(0,28,"ABC",24);//12*24 â€œABCâ€?
      OLED_Refresh();
      HAL_Delay(500);
      OLED_ScrollDisplay(11,4);*/
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

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
