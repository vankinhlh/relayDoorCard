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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
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
#define IN1_Pin GPIO_PIN_3
#define IN1_GPIO_Port GPIOA
#define IN2_Pin GPIO_PIN_4
#define IN2_GPIO_Port GPIOA
#define IN3_Pin GPIO_PIN_5
#define IN3_GPIO_Port GPIOA
#define IN4_Pin GPIO_PIN_6
#define IN4_GPIO_Port GPIOA
#define Button1_Pin GPIO_PIN_13
#define Button1_GPIO_Port GPIOB
#define Button2_Pin GPIO_PIN_14
#define Button2_GPIO_Port GPIOB
#define ledRun_Pin GPIO_PIN_8
#define ledRun_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_2
#define D3_GPIO_Port GPIOD
#define D2_Pin GPIO_PIN_3
#define D2_GPIO_Port GPIOB
#define D1_Pin GPIO_PIN_4
#define D1_GPIO_Port GPIOB
#define D0_Pin GPIO_PIN_5
#define D0_GPIO_Port GPIOB
#define Relay2_Pin GPIO_PIN_8
#define Relay2_GPIO_Port GPIOB
#define Relay1_Pin GPIO_PIN_9
#define Relay1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define OTHER_APPLICATION 1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
