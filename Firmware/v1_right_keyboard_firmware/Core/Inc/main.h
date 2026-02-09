/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define KEY_C7_Pin GPIO_PIN_13
#define KEY_C7_GPIO_Port GPIOC
#define KEY_B8_Pin GPIO_PIN_14
#define KEY_B8_GPIO_Port GPIOC
#define KEY_A9_Pin GPIO_PIN_15
#define KEY_A9_GPIO_Port GPIOC
#define KEY_A10_Pin GPIO_PIN_0
#define KEY_A10_GPIO_Port GPIOF
#define KEY_A11_Pin GPIO_PIN_1
#define KEY_A11_GPIO_Port GPIOF
#define KEY_A12_Pin GPIO_PIN_0
#define KEY_A12_GPIO_Port GPIOA
#define KEY_B9_Pin GPIO_PIN_1
#define KEY_B9_GPIO_Port GPIOA
#define KEY_C8_Pin GPIO_PIN_2
#define KEY_C8_GPIO_Port GPIOA
#define KEY_D7_Pin GPIO_PIN_3
#define KEY_D7_GPIO_Port GPIOA
#define KEY_B10_Pin GPIO_PIN_4
#define KEY_B10_GPIO_Port GPIOA
#define KEY_B11_Pin GPIO_PIN_5
#define KEY_B11_GPIO_Port GPIOA
#define KEY_B12_Pin GPIO_PIN_6
#define KEY_B12_GPIO_Port GPIOA
#define KEY_C9_Pin GPIO_PIN_7
#define KEY_C9_GPIO_Port GPIOA
#define KEY_C10_Pin GPIO_PIN_0
#define KEY_C10_GPIO_Port GPIOB
#define KEY_C11_Pin GPIO_PIN_1
#define KEY_C11_GPIO_Port GPIOB
#define KEY_C12_Pin GPIO_PIN_2
#define KEY_C12_GPIO_Port GPIOB
#define KEY_D8_Pin GPIO_PIN_10
#define KEY_D8_GPIO_Port GPIOB
#define KEY_D9_Pin GPIO_PIN_11
#define KEY_D9_GPIO_Port GPIOB
#define KEY_D10_Pin GPIO_PIN_12
#define KEY_D10_GPIO_Port GPIOB
#define KEY_D12_Pin GPIO_PIN_13
#define KEY_D12_GPIO_Port GPIOB
#define KEY_D11_Pin GPIO_PIN_14
#define KEY_D11_GPIO_Port GPIOB
#define KEY_E6_Pin GPIO_PIN_9
#define KEY_E6_GPIO_Port GPIOA
#define KEY_E5_Pin GPIO_PIN_6
#define KEY_E5_GPIO_Port GPIOC
#define KEY_E4_Pin GPIO_PIN_7
#define KEY_E4_GPIO_Port GPIOC
#define NEOPIXEL_Pin GPIO_PIN_10
#define NEOPIXEL_GPIO_Port GPIOA
#define ENC_SW_Pin GPIO_PIN_1
#define ENC_SW_GPIO_Port GPIOD
#define STAT2_LED_Pin GPIO_PIN_3
#define STAT2_LED_GPIO_Port GPIOD
#define STAT1_LED_Pin GPIO_PIN_4
#define STAT1_LED_GPIO_Port GPIOB
#define KEY_B7_Pin GPIO_PIN_5
#define KEY_B7_GPIO_Port GPIOB
#define KEY_A7_Pin GPIO_PIN_6
#define KEY_A7_GPIO_Port GPIOB
#define KEY_A8_Pin GPIO_PIN_7
#define KEY_A8_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
