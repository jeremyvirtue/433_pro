/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_CS_Pin GPIO_PIN_4
#define OLED_CS_GPIO_Port GPIOA
#define OLED_D0_Pin GPIO_PIN_5
#define OLED_D0_GPIO_Port GPIOA
#define OLED_D1_Pin GPIO_PIN_7
#define OLED_D1_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_0
#define OLED_RES_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_1
#define OLED_DC_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_12
#define LED0_GPIO_Port GPIOB
#define tx_433_Pin GPIO_PIN_13
#define tx_433_GPIO_Port GPIOB
#define rx_433_Pin GPIO_PIN_14
#define rx_433_GPIO_Port GPIOB
#define key1_Pin GPIO_PIN_7
#define key1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define LED0_SET        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET)
#define LED0_RESET      HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET)
#define LED0_Tog        HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin)

#define tx_433_Pin_SET       HAL_GPIO_WritePin(tx_433_GPIO_Port,tx_433_Pin,GPIO_PIN_SET)
#define tx_433_Pin_RESET     HAL_GPIO_WritePin(tx_433_GPIO_Port,tx_433_Pin,GPIO_PIN_RESET)

#define PING_ASK		 HAL_GPIO_ReadPin(rx_433_GPIO_Port  , rx_433_Pin)			//	14

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
