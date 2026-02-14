/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_ucpd.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_dma.h"

#include "stm32g0xx_ll_exti.h"

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
#define KEY_E1_Pin GPIO_PIN_13
#define KEY_E1_GPIO_Port GPIOC
#define KEY_D1_Pin GPIO_PIN_14
#define KEY_D1_GPIO_Port GPIOC
#define KEY_D2_Pin GPIO_PIN_15
#define KEY_D2_GPIO_Port GPIOC
#define KEY_D3_Pin GPIO_PIN_0
#define KEY_D3_GPIO_Port GPIOF
#define KEY_D4_Pin GPIO_PIN_1
#define KEY_D4_GPIO_Port GPIOF
#define KEY_C1_Pin GPIO_PIN_0
#define KEY_C1_GPIO_Port GPIOA
#define KEY_C2_Pin GPIO_PIN_1
#define KEY_C2_GPIO_Port GPIOA
#define KEY_C3_Pin GPIO_PIN_2
#define KEY_C3_GPIO_Port GPIOA
#define KEY_C4_Pin GPIO_PIN_3
#define KEY_C4_GPIO_Port GPIOA
#define KEY_D5_Pin GPIO_PIN_4
#define KEY_D5_GPIO_Port GPIOA
#define KEY_B1_Pin GPIO_PIN_5
#define KEY_B1_GPIO_Port GPIOA
#define KEY_B2_Pin GPIO_PIN_6
#define KEY_B2_GPIO_Port GPIOA
#define KEY_B3_Pin GPIO_PIN_7
#define KEY_B3_GPIO_Port GPIOA
#define KEY_B4_Pin GPIO_PIN_0
#define KEY_B4_GPIO_Port GPIOB
#define KEY_A1_Pin GPIO_PIN_1
#define KEY_A1_GPIO_Port GPIOB
#define KEY_C5_Pin GPIO_PIN_2
#define KEY_C5_GPIO_Port GPIOB
#define KEY_A2_Pin GPIO_PIN_10
#define KEY_A2_GPIO_Port GPIOB
#define KEY_B5_Pin GPIO_PIN_11
#define KEY_B5_GPIO_Port GPIOB
#define KEY_A3_Pin GPIO_PIN_12
#define KEY_A3_GPIO_Port GPIOB
#define KEY_D6_Pin GPIO_PIN_9
#define KEY_D6_GPIO_Port GPIOA
#define KEY_A4_Pin GPIO_PIN_6
#define KEY_A4_GPIO_Port GPIOC
#define KEY_A5_Pin GPIO_PIN_7
#define KEY_A5_GPIO_Port GPIOC
#define KEY_A6_Pin GPIO_PIN_10
#define KEY_A6_GPIO_Port GPIOA
#define KEY_B6_Pin GPIO_PIN_15
#define KEY_B6_GPIO_Port GPIOA
#define KEY_C6_Pin GPIO_PIN_1
#define KEY_C6_GPIO_Port GPIOD
#define STAT1_LED_Pin GPIO_PIN_3
#define STAT1_LED_GPIO_Port GPIOD
#define STAT2_LED_Pin GPIO_PIN_3
#define STAT2_LED_GPIO_Port GPIOB
#define ENC_A_Pin GPIO_PIN_4
#define ENC_A_GPIO_Port GPIOB
#define ENC_B_Pin GPIO_PIN_5
#define ENC_B_GPIO_Port GPIOB
#define ENC_SW_Pin GPIO_PIN_6
#define ENC_SW_GPIO_Port GPIOB
#define NEOPIXEL_Pin GPIO_PIN_7
#define NEOPIXEL_GPIO_Port GPIOB
#define KEY_E3_Pin GPIO_PIN_8
#define KEY_E3_GPIO_Port GPIOB
#define KEY_E2_Pin GPIO_PIN_9
#define KEY_E2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
