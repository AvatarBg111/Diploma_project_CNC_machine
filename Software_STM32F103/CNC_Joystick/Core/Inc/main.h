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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void process_inputs(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FLOOD_LED_Pin GPIO_PIN_13
#define FLOOD_LED_GPIO_Port GPIOC
#define MIST_LED_Pin GPIO_PIN_14
#define MIST_LED_GPIO_Port GPIOC
#define CYCLE_START_LED_Pin GPIO_PIN_15
#define CYCLE_START_LED_GPIO_Port GPIOC
#define SPINDLE_MODE_SW_NC_1_Pin GPIO_PIN_4
#define SPINDLE_MODE_SW_NC_1_GPIO_Port GPIOA
#define SPINDLE_MODE_SW_NC_2_Pin GPIO_PIN_5
#define SPINDLE_MODE_SW_NC_2_GPIO_Port GPIOA
#define AXIS_X_MULTIPLIER_SW_NC_1_Pin GPIO_PIN_0
#define AXIS_X_MULTIPLIER_SW_NC_1_GPIO_Port GPIOB
#define AXIS_X_MULTIPLIER_SW_NC_2_Pin GPIO_PIN_1
#define AXIS_X_MULTIPLIER_SW_NC_2_GPIO_Port GPIOB
#define AXIS_YZ_MULTIPLIER_SW_NC_1_Pin GPIO_PIN_2
#define AXIS_YZ_MULTIPLIER_SW_NC_1_GPIO_Port GPIOB
#define AXIS_YZ_MULTIPLIER_SW_NC_2_Pin GPIO_PIN_10
#define AXIS_YZ_MULTIPLIER_SW_NC_2_GPIO_Port GPIOB
#define YZ_SW_Pin GPIO_PIN_11
#define YZ_SW_GPIO_Port GPIOB
#define MPG_SW_Pin GPIO_PIN_12
#define MPG_SW_GPIO_Port GPIOB
#define HOLD_Pin GPIO_PIN_13
#define HOLD_GPIO_Port GPIOB
#define CYCLE_START_Pin GPIO_PIN_14
#define CYCLE_START_GPIO_Port GPIOB
#define FLOOD_Pin GPIO_PIN_15
#define FLOOD_GPIO_Port GPIOB
#define MIST_Pin GPIO_PIN_8
#define MIST_GPIO_Port GPIOA
#define MPG_ON_LED_Pin GPIO_PIN_9
#define MPG_ON_LED_GPIO_Port GPIOA
#define AXIS_X_MULTIPLIER_LED_1_Pin GPIO_PIN_10
#define AXIS_X_MULTIPLIER_LED_1_GPIO_Port GPIOA
#define AXIS_X_MULTIPLIER_LED_2_Pin GPIO_PIN_11
#define AXIS_X_MULTIPLIER_LED_2_GPIO_Port GPIOA
#define AXIS_X_MULTIPLIER_LED_3_Pin GPIO_PIN_12
#define AXIS_X_MULTIPLIER_LED_3_GPIO_Port GPIOA
#define LOCK_X_AXIS_SW_Pin GPIO_PIN_13
#define LOCK_X_AXIS_SW_GPIO_Port GPIOA
#define LOCK_YZ_AXIS_SW_Pin GPIO_PIN_14
#define LOCK_YZ_AXIS_SW_GPIO_Port GPIOA
#define AXIS_YZ_MULTIPLIER_LED_1_Pin GPIO_PIN_15
#define AXIS_YZ_MULTIPLIER_LED_1_GPIO_Port GPIOA
#define AXIS_YZ_MULTIPLIER_LED_2_Pin GPIO_PIN_3
#define AXIS_YZ_MULTIPLIER_LED_2_GPIO_Port GPIOB
#define AXIS_YZ_MULTIPLIER_LED_3_Pin GPIO_PIN_4
#define AXIS_YZ_MULTIPLIER_LED_3_GPIO_Port GPIOB
#define YZ_SW_LED_Y_Pin GPIO_PIN_5
#define YZ_SW_LED_Y_GPIO_Port GPIOB
#define YZ_SW_LED_Z_Pin GPIO_PIN_6
#define YZ_SW_LED_Z_GPIO_Port GPIOB
#define HOLD_LED_Pin GPIO_PIN_7
#define HOLD_LED_GPIO_Port GPIOB
#define SPINDLE_CW_LED_Pin GPIO_PIN_8
#define SPINDLE_CW_LED_GPIO_Port GPIOB
#define SPINDLE_CCW_LED_Pin GPIO_PIN_9
#define SPINDLE_CCW_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define SPINDLE_3POS_SW_NC_1_CH 0
#define SPINDLE_3POS_SW_NC_2_CH 1
#define AXIS_X_MULTIPLIER_SW_NC_1_CH 2
#define AXIS_X_MULTIPLIER_SW_NC_2_CH 3
#define AXIS_YZ_MULTIPLIER_SW_NC_1_CH 4
#define AXIS_YZ_MULTIPLIER_SW_NC_2_CH 5
#define YZ_SW_CH 6
#define MPG_BUT_CH 7
#define HOLD_BUT_CH 8
#define CYCLE_START_BUT_CH 9
#define FLOOD_SW_CH 10
#define MIST_SW_CH 11
#define LOCK_X_AXIS_SW_CH 12
#define LOCK_YZ_AXIS_SW_CH 13

#define BUTTON_DELAY_ACTION_TIME 200
#define TIM1_INT_TIME 10
#define PROCESS_INPUTS_DELAY 50
#define TIM1_INT_ENTRIES (PROCESS_INPUTS_DELAY / TIM1_INT_TIME)

#if(1)
	#define LED_STATE_OFF GPIO_PIN_SET
	#define LED_STATE_ON GPIO_PIN_RESET
#else
	#define LED_STATE_OFF GPIO_PIN_RESET
	#define LED_STATE_ON GPIO_PIN_SET
#endif

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
