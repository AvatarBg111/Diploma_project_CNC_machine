/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buttons.h"
#include "uart_comm.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// UART2 variables and buffers
uint8_t uart2_rx_buf[UART_BUF_SIZE] = {0};

// Other variables
uint8_t inputs_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void start_uart2_reception(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern bool process_inputs_flag;
extern bool pendant_status;
extern uint8_t pendant_mpg_status;
extern pendant_data_t pendant_data;

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel6 global interrupt.
  */
void DMA1_Channel6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel6_IRQn 0 */

  /* USER CODE END DMA1_Channel6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Channel6_IRQn 1 */

  /* USER CODE END DMA1_Channel6_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel7 global interrupt.
  */
void DMA1_Channel7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel7_IRQn 0 */

  /* USER CODE END DMA1_Channel7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
  /* USER CODE BEGIN DMA1_Channel7_IRQn 1 */

  /* USER CODE END DMA1_Channel7_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

	// Call function that implements the updating
	// of button channel statuses in a de-bouncer
	update_button_statuses();

	// Increment a counter and when it reaches TIM4_INT_ENTRIES
	// set a flag that shows whether the main program should
	// process the system inputs
	if(++inputs_counter >= TIM4_INT_ENTRIES){
		inputs_counter = 0;
		process_inputs_flag = true;
	}

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */
// CALLBACKS
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance == USART2){ // Events for USART2
		HAL_UART_RxEventTypeTypeDef eventId = HAL_UARTEx_GetRxEventType(huart);

		if(eventId == HAL_UART_RXEVENT_IDLE || eventId == HAL_UART_RXEVENT_TC){
			// The program will enter this if statement,
			// if the received message has "GET_DATA" in it
			if(strstr((char*)uart2_rx_buf, "GET_DATA")){
				// The program will enter this if statement, if
				// the received message's size is more than 13
				if(Size >= 13){
					//uint16_t spindle_sw_delay_action = atoi(uart2_rx_buf + 13);	//TODO
					if(uart2_rx_buf[9] == '1'){ // Received system status is ON
						// Turn on system
						pendant_status = true;

						// Logic for enabling/disabling the MPG functionality
						if(uart2_rx_buf[11] == ('0' + MPG_STATUS_DISABLED) && pendant_mpg_status == MPG_STATUS_ENABLE_ONGOING && !pendant_data.mpg){
							pendant_data.mpg = true;
						}else if(uart2_rx_buf[11] == ('0' + MPG_STATUS_ENABLED) && pendant_mpg_status == MPG_STATUS_ENABLE_ONGOING){
							pendant_data.mpg = true;
							pendant_mpg_status = MPG_STATUS_ENABLED;
						}else if(uart2_rx_buf[11] == ('0' + MPG_STATUS_ENABLED) && pendant_mpg_status == MPG_STATUS_DISABLED){
							pendant_data.mpg = false;
						}else if(uart2_rx_buf[11] == ('0' + MPG_STATUS_DISABLED) && pendant_mpg_status == MPG_STATUS_ENABLED && pendant_data.mpg){
							pendant_data.mpg = false;
						}
					}else if(uart2_rx_buf[9] == '0'){ // Received system status is OFF
						// Turn off system
						pendant_status = false;

						// Turn off system MPG functionality
						pendant_mpg_status = MPG_STATUS_DISABLED;

						// Reset pendant data MPG functionality flag
						pendant_data.mpg = false;
					}
				}

				// Call function that sends message
				// with system data via UART
				send_system_data();
			}

			// Clear UART2 transmit buffer
			// from index 0 to index "Size"
			memset(uart2_rx_buf, 0, Size);

			// Call function that initiates DMA reception transfer on UART2
			start_uart2_reception();
		}
	}
}


// CUSTOM FUCNTIONS
void start_uart2_reception(void){
	if(HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_buf, UART_BUF_SIZE) == HAL_OK){
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}
}

/* USER CODE END 1 */
