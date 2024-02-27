/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buttons.h"
#include "uart_comm.h"
#include "systick_timer.h"
#include "stm32f1xx_it.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Y_OR_Z_BUT_DELAY_CH 0
#define MPG_BUT_DELAY_CH 1
#define FLOOD_BUT_DELAY_CH 2
#define MIST_BUT_DELAY_CH 3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
bool process_inputs_flag = false;
bool pendant_status = false;
uint8_t pendant_mpg_status = MPG_STATUS_DISABLED;

uint8_t button_action_delay_cnt_ch[16] = {0};
const uint8_t button_delay_action_cnt = BUTTON_DELAY_ACTION_TIME / PROCESS_INPUTS_DELAY;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void Init_interfaces(void);
void turn_off_leds(void);
void process_inputs(void);
uint8_t get_spindle_mode(void);
uint8_t get_x_axis_multiplier(void);
uint8_t get_yz_axis_multiplier(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define ENCODER_1_TIM htim2
#define ENCODER_2_TIM htim3

extern pendant_data_t pendant_data;

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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  // Initialize interfaces
  Init_interfaces();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1)
  {
	// The program will enter this if statement when the flag
	// has been set which happens inside TIM4 interrupt handler
	if(process_inputs_flag && pendant_status){
		// Call function for processing system inputs
		process_inputs();

		// Reset the entry flag
		process_inputs_flag = false;
	}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 320;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000 - 1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 200000;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, FLOOD_LED_Pin|MIST_LED_Pin|CYCLE_START_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MPG_ON_LED_Pin|AXIS_X_MULTIPLIER_LED_1_Pin|AXIS_X_MULTIPLIER_LED_2_Pin|AXIS_X_MULTIPLIER_LED_3_Pin
                          |AXIS_YZ_MULTIPLIER_LED_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, AXIS_YZ_MULTIPLIER_LED_2_Pin|AXIS_YZ_MULTIPLIER_LED_3_Pin|YZ_SW_LED_Y_Pin|YZ_SW_LED_Z_Pin
                          |HOLD_LED_Pin|SPINDLE_CW_LED_Pin|SPINDLE_CCW_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : FLOOD_LED_Pin MIST_LED_Pin CYCLE_START_LED_Pin */
  GPIO_InitStruct.Pin = FLOOD_LED_Pin|MIST_LED_Pin|CYCLE_START_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SPINDLE_MODE_SW_NC_1_Pin SPINDLE_MODE_SW_NC_2_Pin MIST_Pin LOCK_X_AXIS_SW_Pin
                           LOCK_YZ_AXIS_SW_Pin */
  GPIO_InitStruct.Pin = SPINDLE_MODE_SW_NC_1_Pin|SPINDLE_MODE_SW_NC_2_Pin|MIST_Pin|LOCK_X_AXIS_SW_Pin
                          |LOCK_YZ_AXIS_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : AXIS_X_MULTIPLIER_SW_NC_1_Pin AXIS_X_MULTIPLIER_SW_NC_2_Pin AXIS_YZ_MULTIPLIER_SW_NC_1_Pin AXIS_YZ_MULTIPLIER_SW_NC_2_Pin
                           YZ_SW_Pin MPG_SW_Pin HOLD_Pin CYCLE_START_Pin
                           FLOOD_Pin */
  GPIO_InitStruct.Pin = AXIS_X_MULTIPLIER_SW_NC_1_Pin|AXIS_X_MULTIPLIER_SW_NC_2_Pin|AXIS_YZ_MULTIPLIER_SW_NC_1_Pin|AXIS_YZ_MULTIPLIER_SW_NC_2_Pin
                          |YZ_SW_Pin|MPG_SW_Pin|HOLD_Pin|CYCLE_START_Pin
                          |FLOOD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MPG_ON_LED_Pin AXIS_X_MULTIPLIER_LED_1_Pin AXIS_X_MULTIPLIER_LED_2_Pin AXIS_X_MULTIPLIER_LED_3_Pin
                           AXIS_YZ_MULTIPLIER_LED_1_Pin */
  GPIO_InitStruct.Pin = MPG_ON_LED_Pin|AXIS_X_MULTIPLIER_LED_1_Pin|AXIS_X_MULTIPLIER_LED_2_Pin|AXIS_X_MULTIPLIER_LED_3_Pin
                          |AXIS_YZ_MULTIPLIER_LED_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : AXIS_YZ_MULTIPLIER_LED_2_Pin AXIS_YZ_MULTIPLIER_LED_3_Pin YZ_SW_LED_Y_Pin YZ_SW_LED_Z_Pin
                           HOLD_LED_Pin SPINDLE_CW_LED_Pin SPINDLE_CCW_LED_Pin */
  GPIO_InitStruct.Pin = AXIS_YZ_MULTIPLIER_LED_2_Pin|AXIS_YZ_MULTIPLIER_LED_3_Pin|YZ_SW_LED_Y_Pin|YZ_SW_LED_Z_Pin
                          |HOLD_LED_Pin|SPINDLE_CW_LED_Pin|SPINDLE_CCW_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief Initialize interfaces
  */
void Init_interfaces(void){
	// Start encoder timers
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

	// Start 3 position switch LED PWM timer
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	// Start TIM4 for button de-bouncer
	HAL_TIM_Base_Start_IT(&htim4);

	// Assign buttons/switches a de-bouncer channel
	set_button_channel(SPINDLE_MODE_SW_NC_1_GPIO_Port, SPINDLE_MODE_SW_NC_1_Pin, SPINDLE_3POS_SW_NC_1_CH);
	set_button_channel(SPINDLE_MODE_SW_NC_2_GPIO_Port, SPINDLE_MODE_SW_NC_2_Pin, SPINDLE_3POS_SW_NC_2_CH);
	set_button_channel(AXIS_X_MULTIPLIER_SW_NC_1_GPIO_Port, AXIS_X_MULTIPLIER_SW_NC_1_Pin, AXIS_X_MULTIPLIER_SW_NC_1_CH);
	set_button_channel(AXIS_X_MULTIPLIER_SW_NC_2_GPIO_Port, AXIS_X_MULTIPLIER_SW_NC_2_Pin, AXIS_X_MULTIPLIER_SW_NC_2_CH);
	set_button_channel(AXIS_YZ_MULTIPLIER_SW_NC_1_GPIO_Port, AXIS_YZ_MULTIPLIER_SW_NC_1_Pin, AXIS_YZ_MULTIPLIER_SW_NC_1_CH);
	set_button_channel(AXIS_YZ_MULTIPLIER_SW_NC_2_GPIO_Port, AXIS_YZ_MULTIPLIER_SW_NC_2_Pin, AXIS_YZ_MULTIPLIER_SW_NC_2_CH);
	set_button_channel(YZ_SW_GPIO_Port, YZ_SW_Pin, YZ_SW_CH);
	set_button_channel(MPG_SW_GPIO_Port, MPG_SW_Pin, MPG_BUT_CH);
	set_button_channel(HOLD_GPIO_Port, HOLD_Pin, HOLD_BUT_CH);
	set_button_channel(CYCLE_START_GPIO_Port, CYCLE_START_Pin, CYCLE_START_BUT_CH);
	set_button_channel(FLOOD_GPIO_Port, FLOOD_Pin, FLOOD_SW_CH);
	set_button_channel(MIST_GPIO_Port, MIST_Pin, MIST_SW_CH);
	set_button_channel(LOCK_X_AXIS_SW_GPIO_Port, LOCK_X_AXIS_SW_Pin, LOCK_X_AXIS_SW_CH);
	set_button_channel(LOCK_YZ_AXIS_SW_GPIO_Port, LOCK_YZ_AXIS_SW_Pin, LOCK_YZ_AXIS_SW_CH);

	// Turn off all LEDs
	turn_off_leds();

	// Start UART2 reception
	start_uart2_reception();
}

/**
  * @brief Turn off all system LEDs
  */
void turn_off_leds(void){
	HAL_GPIO_WritePin(FLOOD_LED_GPIO_Port, FLOOD_LED_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(MIST_LED_GPIO_Port, MIST_LED_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(CYCLE_START_LED_GPIO_Port, CYCLE_START_LED_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(HOLD_LED_GPIO_Port, HOLD_LED_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(MPG_ON_LED_GPIO_Port, MPG_ON_LED_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(YZ_SW_LED_Y_GPIO_Port, YZ_SW_LED_Y_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(YZ_SW_LED_Z_GPIO_Port, YZ_SW_LED_Z_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(SPINDLE_CW_LED_GPIO_Port, SPINDLE_CW_LED_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(SPINDLE_CCW_LED_GPIO_Port, SPINDLE_CCW_LED_Pin, LED_STATE_OFF);

	HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_1_GPIO_Port, AXIS_X_MULTIPLIER_LED_1_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_2_GPIO_Port, AXIS_X_MULTIPLIER_LED_2_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_3_GPIO_Port, AXIS_X_MULTIPLIER_LED_3_Pin, LED_STATE_OFF);

	HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_1_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_1_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_2_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_2_Pin, LED_STATE_OFF);
	HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_3_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_3_Pin, LED_STATE_OFF);
}


/**
  * @brief Process system inputs
  */
void process_inputs(void){
	static bool y_or_z_last_state = false;
	uint8_t lock_x_axis = get_button_state(LOCK_X_AXIS_SW_CH);	// Get X axis lock state
	uint8_t lock_yz_axis = get_button_state(LOCK_YZ_AXIS_SW_CH); // Get Y/Z axes lock state

	// Check MPG functionality switch input (it disables/enables MPG functionality)
	if(get_button_state(MPG_BUT_CH) == GPIO_PIN_RESET){
		if(++button_action_delay_cnt_ch[MPG_BUT_DELAY_CH] >= button_delay_action_cnt || \
		   button_action_delay_cnt_ch[MPG_BUT_DELAY_CH] == 1){
			if(button_action_delay_cnt_ch[MPG_BUT_DELAY_CH] != 1 && button_delay_action_cnt != 1){
				button_action_delay_cnt_ch[MPG_BUT_DELAY_CH] = 0;
			}

			if(pendant_mpg_status != MPG_STATUS_DISABLED){
				pendant_mpg_status = MPG_STATUS_DISABLED;
			}else{
				pendant_mpg_status = MPG_STATUS_ENABLE_ONGOING;
			}
		}
	}else{
		button_action_delay_cnt_ch[MPG_BUT_DELAY_CH] = 0;
	}

	// The program will enter this if statement, if the system
	// is enabled and it's MPg functionality is turned on
	if(pendant_status && pendant_data.mpg){
		// Keep or "discard" axis movement data based on locks states
		if(lock_x_axis == GPIO_PIN_RESET){
			pendant_data.encoder1_val = ENCODER_1_TIM.Instance->CNT;
		}else{
			ENCODER_1_TIM.Instance->CNT = pendant_data.encoder1_val;
		}
		if(lock_yz_axis == GPIO_PIN_RESET){
			pendant_data.encoder2_val = ENCODER_2_TIM.Instance->CNT;
		}else{
			ENCODER_2_TIM.Instance->CNT = pendant_data.encoder2_val;
		}
	}else{
		// Discard encoder inputs
		pendant_data.encoder1_val = 0;
		pendant_data.encoder2_val = 0;
		ENCODER_1_TIM.Instance->CNT = 0;
		ENCODER_2_TIM.Instance->CNT = 0;
	}

	// If pendant's MPG functionality is enabled,
	// execute the following code inside the if statement
	if(pendant_mpg_status == MPG_STATUS_ENABLED){
		pendant_data.spindle_mode = get_spindle_mode(); // Get spindle mode
		pendant_data.jog_mode = 0;	//TODO: need to have an input for jogging mode switching
		pendant_data.x_axis_multiplicity = get_x_axis_multiplier(); // Get X axis multiplier
		pendant_data.yz_axis_multiplicity = get_yz_axis_multiplier(); // Get Y/Z axes multiplier

		// Check Y/Z axes switch input
		if(get_button_state(YZ_SW_CH) == GPIO_PIN_RESET){
			if(!y_or_z_last_state){
				y_or_z_last_state = true;
				button_action_delay_cnt_ch[Y_OR_Z_BUT_DELAY_CH] = 0;
			}
			if(++button_action_delay_cnt_ch[Y_OR_Z_BUT_DELAY_CH] >= button_delay_action_cnt){
				button_action_delay_cnt_ch[Y_OR_Z_BUT_DELAY_CH] = 0;
				pendant_data.y_or_z = YZ_MOVEMENT_Z;	// Set to Z axis
			}
		}else{
			if(y_or_z_last_state){
				y_or_z_last_state = false;
				button_action_delay_cnt_ch[Y_OR_Z_BUT_DELAY_CH] = 0;
			}
			if(++button_action_delay_cnt_ch[Y_OR_Z_BUT_DELAY_CH] >= button_delay_action_cnt){
				button_action_delay_cnt_ch[Y_OR_Z_BUT_DELAY_CH] = 0;
				pendant_data.y_or_z = YZ_MOVEMENT_Y;	// Set to Z axis
			}
		}

		// Check FLOOD coolant switch input
		if(get_button_state(FLOOD_SW_CH) == GPIO_PIN_RESET){
			if(++button_action_delay_cnt_ch[FLOOD_BUT_DELAY_CH] >= button_delay_action_cnt){
				button_action_delay_cnt_ch[FLOOD_BUT_DELAY_CH] = 0;
				pendant_data.flood = (pendant_data.flood ? false : true);
			}
		}else{
			button_action_delay_cnt_ch[FLOOD_BUT_DELAY_CH] = 0;
		}

		// Check MIST coolant switch input
		if(get_button_state(MIST_SW_CH) == GPIO_PIN_RESET){
			if(++button_action_delay_cnt_ch[MIST_BUT_DELAY_CH] >= button_delay_action_cnt){
				button_action_delay_cnt_ch[MIST_BUT_DELAY_CH] = 0;
				pendant_data.mist = (pendant_data.mist ? false : true);
			}
		}else{
			button_action_delay_cnt_ch[MIST_BUT_DELAY_CH] = 0;
		}

		// Check HOLD and Cycle start button input
		if(get_button_state(HOLD_BUT_CH) == GPIO_PIN_RESET){	// System HOLD status
			pendant_data.system_status = Hold;		// Set system status to Hold
		}else if(get_button_state(CYCLE_START_BUT_CH) == GPIO_PIN_RESET){	// System CYCLE START status
			pendant_data.system_status = Idle;		// Set system status to Idle
		}

		// Set MPG status LED
		HAL_GPIO_WritePin(MPG_ON_LED_GPIO_Port, MPG_ON_LED_Pin, LED_STATE_ON);

		// Set / Reset LED for which axis between Y and Z is selected
		if(pendant_data.y_or_z == YZ_MOVEMENT_Z){
			HAL_GPIO_WritePin(YZ_SW_LED_Y_GPIO_Port, YZ_SW_LED_Y_Pin, LED_STATE_OFF);
			HAL_GPIO_WritePin(YZ_SW_LED_Z_GPIO_Port, YZ_SW_LED_Z_Pin, LED_STATE_ON);
		}else if(pendant_data.y_or_z == YZ_MOVEMENT_Y){
			HAL_GPIO_WritePin(YZ_SW_LED_Y_GPIO_Port, YZ_SW_LED_Y_Pin, LED_STATE_ON);
			HAL_GPIO_WritePin(YZ_SW_LED_Z_GPIO_Port, YZ_SW_LED_Z_Pin, LED_STATE_OFF);
		}

		// Set / Reset flood coolant LED
		if(pendant_data.flood){
			HAL_GPIO_WritePin(FLOOD_LED_GPIO_Port, FLOOD_LED_Pin, LED_STATE_ON);
		}else{
			HAL_GPIO_WritePin(FLOOD_LED_GPIO_Port, FLOOD_LED_Pin, LED_STATE_OFF);
		}

		// Set / Reset mist coolant LED
		if(pendant_data.mist){
			HAL_GPIO_WritePin(MIST_LED_GPIO_Port, MIST_LED_Pin, LED_STATE_ON);
		}else{
			HAL_GPIO_WritePin(MIST_LED_GPIO_Port, MIST_LED_Pin, LED_STATE_OFF);
		}

		// Set / Reset X axis multiplier LEDs
		switch(pendant_data.x_axis_multiplicity){
			case 0:
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_1_GPIO_Port, AXIS_X_MULTIPLIER_LED_1_Pin, LED_STATE_ON);
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_2_GPIO_Port, AXIS_X_MULTIPLIER_LED_2_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_3_GPIO_Port, AXIS_X_MULTIPLIER_LED_3_Pin, LED_STATE_OFF);
				break;
			case 1:
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_1_GPIO_Port, AXIS_X_MULTIPLIER_LED_1_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_2_GPIO_Port, AXIS_X_MULTIPLIER_LED_2_Pin, LED_STATE_ON);
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_3_GPIO_Port, AXIS_X_MULTIPLIER_LED_3_Pin, LED_STATE_OFF);
				break;
			case 2:
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_1_GPIO_Port, AXIS_X_MULTIPLIER_LED_1_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_2_GPIO_Port, AXIS_X_MULTIPLIER_LED_2_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(AXIS_X_MULTIPLIER_LED_3_GPIO_Port, AXIS_X_MULTIPLIER_LED_3_Pin, LED_STATE_ON);
				break;
			default:
				break;
		}

		// Set / Reset YZ axis multiplier LEDs
		switch(pendant_data.x_axis_multiplicity){
			case 0:
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_1_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_1_Pin, LED_STATE_ON);
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_2_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_2_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_3_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_3_Pin, LED_STATE_OFF);
				break;
			case 1:
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_1_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_1_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_2_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_2_Pin, LED_STATE_ON);
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_3_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_3_Pin, LED_STATE_OFF);
				break;
			case 2:
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_1_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_1_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_2_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_2_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(AXIS_YZ_MULTIPLIER_LED_3_GPIO_Port, AXIS_YZ_MULTIPLIER_LED_3_Pin, LED_STATE_ON);
				break;
			default:
				break;
		}

		// Set / Reset spindle mode LEDs
		switch(pendant_data.spindle_mode){
			case SPINDLE_MODE_CW:
				HAL_GPIO_WritePin(SPINDLE_CW_LED_GPIO_Port, SPINDLE_CW_LED_Pin, LED_STATE_ON);
				HAL_GPIO_WritePin(SPINDLE_CCW_LED_GPIO_Port, SPINDLE_CCW_LED_Pin, LED_STATE_OFF);
				break;
			case SPINDLE_MODE_CCW:
				HAL_GPIO_WritePin(SPINDLE_CW_LED_GPIO_Port, SPINDLE_CW_LED_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(SPINDLE_CCW_LED_GPIO_Port, SPINDLE_CCW_LED_Pin, LED_STATE_ON);
				break;
			case SPINDLE_MODE_OFF:
				HAL_GPIO_WritePin(SPINDLE_CW_LED_GPIO_Port, SPINDLE_CW_LED_Pin, LED_STATE_OFF);
				HAL_GPIO_WritePin(SPINDLE_CCW_LED_GPIO_Port, SPINDLE_CCW_LED_Pin, LED_STATE_OFF);
				break;
			default:
				break;
		}

		// Set / Reset machine status LEDs
		if(pendant_data.system_status == Idle){
			HAL_GPIO_WritePin(CYCLE_START_LED_GPIO_Port, CYCLE_START_LED_Pin, LED_STATE_ON);
			HAL_GPIO_WritePin(HOLD_LED_GPIO_Port, HOLD_LED_Pin, LED_STATE_OFF);
		}else if(pendant_data.system_status == Hold){
			HAL_GPIO_WritePin(CYCLE_START_LED_GPIO_Port, CYCLE_START_LED_Pin, LED_STATE_OFF);
			HAL_GPIO_WritePin(HOLD_LED_GPIO_Port, HOLD_LED_Pin, LED_STATE_ON);
		}
	}else if(pendant_mpg_status == MPG_STATUS_DISABLED){
		// Turn off system LEDs
		turn_off_leds();
	}
}

/**
  * @brief Analyze spindle mode switch input and return spindle mode
  */
uint8_t get_spindle_mode(void){
	uint8_t button_states = 0x00;

	button_states |= (~get_button_state(SPINDLE_3POS_SW_NC_1_CH)) & 0x01;
	button_states |= (((~get_button_state(SPINDLE_3POS_SW_NC_2_CH)) & 0x01) << 1);

	if(button_states == 0x03){
		return SPINDLE_MODE_OFF;
	}else if(button_states == 0x01){
		return SPINDLE_MODE_CW;
	}else if(button_states == 0x02){
		return SPINDLE_MODE_CCW;
	}

	return SPINDLE_MODE_OFF;
}

/**
  * @brief Analyze X axis multiplier selector input and return the selected multiplier
  */
uint8_t get_x_axis_multiplier(void){
	uint8_t button_states = 0x00;

	button_states |= (~get_button_state(AXIS_X_MULTIPLIER_SW_NC_1_CH)) & 0x01;
	button_states |= (((~get_button_state(AXIS_X_MULTIPLIER_SW_NC_2_CH)) & 0x01) << 1);

	if(button_states == 0x03){
		return 0;
	}else if(button_states == 0x01){
		return 1;
	}else if(button_states == 0x02){
		return 2;
	}

	return 0;
}

/**
  * @brief Analyze Y/Z axes multiplier selector input and return the selected multiplier
  */
uint8_t get_yz_axis_multiplier(void){
	uint8_t button_states = 0x00;

	button_states |= (~get_button_state(AXIS_YZ_MULTIPLIER_SW_NC_1_CH) & 0x01);
	button_states |= (((~get_button_state(AXIS_YZ_MULTIPLIER_SW_NC_2_CH)) & 0x01) << 1);

	if(button_states == 0x03){
		return 0;
	}else if(button_states == 0x01){
		return 1;
	}else if(button_states == 0x02){
		return 2;
	}

	return 0;
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
