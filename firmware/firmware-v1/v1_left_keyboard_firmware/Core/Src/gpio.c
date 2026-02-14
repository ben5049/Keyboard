/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STAT1_LED_GPIO_Port, STAT1_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STAT2_LED_GPIO_Port, STAT2_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : KEY_E1_Pin KEY_D1_Pin KEY_D2_Pin KEY_A4_Pin
                           KEY_A5_Pin */
  GPIO_InitStruct.Pin = KEY_E1_Pin|KEY_D1_Pin|KEY_D2_Pin|KEY_A4_Pin
                          |KEY_A5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_D3_Pin KEY_D4_Pin */
  GPIO_InitStruct.Pin = KEY_D3_Pin|KEY_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_C1_Pin KEY_C2_Pin KEY_C3_Pin KEY_C4_Pin
                           KEY_D5_Pin KEY_B1_Pin KEY_B2_Pin KEY_B3_Pin
                           KEY_D6_Pin KEY_A6_Pin KEY_B6_Pin */
  GPIO_InitStruct.Pin = KEY_C1_Pin|KEY_C2_Pin|KEY_C3_Pin|KEY_C4_Pin
                          |KEY_D5_Pin|KEY_B1_Pin|KEY_B2_Pin|KEY_B3_Pin
                          |KEY_D6_Pin|KEY_A6_Pin|KEY_B6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_B4_Pin KEY_A1_Pin KEY_C5_Pin KEY_A2_Pin
                           KEY_B5_Pin KEY_A3_Pin ENC_SW_Pin KEY_E3_Pin
                           KEY_E2_Pin */
  GPIO_InitStruct.Pin = KEY_B4_Pin|KEY_A1_Pin|KEY_C5_Pin|KEY_A2_Pin
                          |KEY_B5_Pin|KEY_A3_Pin|ENC_SW_Pin|KEY_E3_Pin
                          |KEY_E2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_C6_Pin */
  GPIO_InitStruct.Pin = KEY_C6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_C6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STAT1_LED_Pin */
  GPIO_InitStruct.Pin = STAT1_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STAT1_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STAT2_LED_Pin */
  GPIO_InitStruct.Pin = STAT2_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STAT2_LED_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
