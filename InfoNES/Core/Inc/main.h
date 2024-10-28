/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define BAT_ADC_Pin GPIO_PIN_0
#define BAT_ADC_GPIO_Port GPIOC
#define KEY_A_Pin GPIO_PIN_0
#define KEY_A_GPIO_Port GPIOA
#define KEY_B_Pin GPIO_PIN_1
#define KEY_B_GPIO_Port GPIOA
#define KEY_SEL_Pin GPIO_PIN_2
#define KEY_SEL_GPIO_Port GPIOA
#define KEY_STR_Pin GPIO_PIN_3
#define KEY_STR_GPIO_Port GPIOA
#define BUZZ_Pin GPIO_PIN_10
#define BUZZ_GPIO_Port GPIOB
#define F_CS_Pin GPIO_PIN_14
#define F_CS_GPIO_Port GPIOB
#define KEY_RIGH_Pin GPIO_PIN_8
#define KEY_RIGH_GPIO_Port GPIOD
#define KEY_DOWN_Pin GPIO_PIN_9
#define KEY_DOWN_GPIO_Port GPIOD
#define KEY_LEFT_Pin GPIO_PIN_10
#define KEY_LEFT_GPIO_Port GPIOD
#define KEY_UP_Pin GPIO_PIN_11
#define KEY_UP_GPIO_Port GPIOD
#define KEY_BACK_Pin GPIO_PIN_12
#define KEY_BACK_GPIO_Port GPIOD
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOD
#define LCD_DC_Pin GPIO_PIN_1
#define LCD_DC_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_2
#define LCD_RST_GPIO_Port GPIOD
#define LCD_LED_Pin GPIO_PIN_3
#define LCD_LED_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
