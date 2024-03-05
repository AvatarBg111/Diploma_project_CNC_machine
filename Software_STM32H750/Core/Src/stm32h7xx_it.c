/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ft5436.h"
#include "buttons.h"
#include "grbl.h"
#include "mpg_pendant.h"
#include "mpg_movement.h"
#include "../grblHALComm/parser.h"
#include "systick_timer.h"
#include "r61529_screen_menu.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GRBL_BUF_SIZE 256
#define PENDANT_BUF_SIZE 32
#define TIM1_DELAY 10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// Touch IC communication variables and buffers
uint8_t touch_detected = 0x00;
uint8_t i2c3_dma_itr_sync = 0x00;

// GRBL UART communication variables and buffers
uint8_t uart2_rx_buffer[GRBL_BUF_SIZE] = {0};
uint16_t uart2_rx_buf_size = 0;
uint8_t grbl_buffer[GRBL_BUF_SIZE] = {0};
uint16_t grbl_buffer_size = 0;
const char initial_grbl_message[] = "GrblHAL 1.1f ['$' or '$HELP' for help]";

// MPG pendant communication variables and buffers
uint8_t uart4_rx_buf[PENDANT_BUF_SIZE] = {0};
uint8_t uart4_tx_buf[PENDANT_BUF_SIZE] = {0};
bool pendant_connecting_procedure_started = false;
bool pendant_disconnecting_procedure_started = false;

// Debug UART communication variables and buffers
uint8_t uart3_tx_buf[100] = {0};

// Buzzer control variables and buffers
bool buzzer_program_occupied = false;
uint16_t buzzer_update_event_cnt = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void start_grbl_uart_reception(void);
bool is_initial_grbl_message(uint8_t*);
void start_pendant_connection(void);
void start_pendant_reception(void);
bool start_buzzer(uint16_t);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_i2c3_rx;
extern DMA_HandleTypeDef hdma_i2c3_tx;
extern I2C_HandleTypeDef hi2c3;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */
extern DMA_HandleTypeDef hdma_uart4_rx;
extern menu_controller_t menu_controller;
extern pendant_control_t pendant_control;
extern pendant_data_t pendant_data;
extern grbl_data_t grbl_data;

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
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
  * @brief This function handles Pre-fetch fault, memory access fault.
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
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */
	if(__HAL_DMA_STREAM_GET_IT_SOURCE(&hdma_i2c3_rx, DMA_IT_TC)){
		i2c3_dma_itr_sync |= 0x02;
	}

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_i2c3_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */
  if(i2c3_dma_itr_sync & 0x02){
	i2c3_dma_itr_sync &= ~0x02;
	reception_complete_callback(&hdma_i2c3_rx);
  }

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */
	if(__HAL_DMA_STREAM_GET_IT_SOURCE(&hdma_i2c3_tx, DMA_IT_TC)){
		i2c3_dma_itr_sync |= 0x01;
	}

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_i2c3_tx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */
  if(i2c3_dma_itr_sync & 0x01){
	i2c3_dma_itr_sync &= ~0x01;
	transmission_complete_callback(&hdma_i2c3_tx);
  }

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

  /* USER CODE END DMA1_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */

  /* USER CODE END DMA1_Stream4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
  /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

  /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

  /* USER CODE END DMA1_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
  touch_detected = 0x01;
  request_available_points();

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt.
  */
void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_IRQn 0 */

  /* USER CODE END TIM1_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_IRQn 1 */
  	// Call function for updating button channel statuses of de-bouncer
	update_button_statuses();

	// The TIM1 interrupt occurs every 10m, so each time increment a counter
	// and when it reaches (ENTER_MENU_DELAY_TIME / TIM1_DELAY) times, set
	// a flag that allows the program to enter/run the menu logic function one cycle
	if(++menu_controller.menu_cnt > (ENTER_MENU_DELAY_TIME / TIM1_DELAY)){
		menu_controller.menu_cnt = 0;
		menu_controller.enter_menu = true;
	}

	// The code inside this if statement is ran
	// when the system has entered manual mode
	if(menu_controller.entered_manual_mode){
		// If MPG "turn off" procedure has been initiated,
		// disable the MPG functionality of the MPG pendant
		if(pendant_control.turn_off_mpg){
			pendant_control.mpg_status = MPG_STATUS_DISABLED;
		}

		// The TIM1 interrupt occurs every 10m, so each time
		// increment a counter and when it reaches DATA_REQUEST_CNT,
		// send a message to the MPG pendant in order to receive data from it
		if(++pendant_control.counter == DATA_REQUEST_CNT){
			request_pendant_data();
		}else if(pendant_control.counter >= CHECK_DATA_RECEIVED_CNT){
			// If he counter has reached CHECK_DATA_RECEIVED_CNT,
			// check if a response has been received
			if(!pendant_control.request_answered){ // Response has NOT been received
				// Increment a second "timeout" counter
				// If it has reached REQUEST_TIMEOUT_CNT, restart the process flow
				if(++pendant_control.comm_cnt == REQUEST_TIMEOUT_CNT){
					pendant_control.counter = 0;
					pendant_control.comm_cnt = 0;
				}
			}else{ // Response has been received
				// Restart process flow and stop waiting for response
				pendant_control.request_answered = false;
				pendant_control.counter = 0;
				pendant_control.comm_cnt = 0;
			}
		}
	}else{ // System is not in manual mode
		// Reset data request and timeout counters
		pendant_control.counter = 0;
		pendant_control.comm_cnt = 0;
	}

  /* USER CODE END TIM1_UP_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
	if(__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_IT_CC3) == SET){
		if(--buzzer_update_event_cnt == 0){
			buzzer_program_occupied = false;
		}
	}

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */
  // If flag is reset, stop buzzer wave generator
  if(buzzer_program_occupied == false){
	  HAL_TIM_PWM_Stop_IT(&htim4, TIM_CHANNEL_3);
  }

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

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */

  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles I2C3 event interrupt.
  */
void I2C3_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C3_EV_IRQn 0 */

  /* USER CODE END I2C3_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hi2c3);
  /* USER CODE BEGIN I2C3_EV_IRQn 1 */

  /* USER CODE END I2C3_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C3 error interrupt.
  */
void I2C3_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C3_ER_IRQn 0 */

  /* USER CODE END I2C3_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c3);
  /* USER CODE BEGIN I2C3_ER_IRQn 1 */

  /* USER CODE END I2C3_ER_IRQn 1 */
}

/* USER CODE BEGIN 1 */
// CALLBACKS
/**
  * @brief This callback function is mainly used for
  * reading and analyzing received data on GRBL and
  * MPG pendant UART ports
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance == USART2){ // Event on USART2
		HAL_UART_RxEventTypeTypeDef eventId = HAL_UARTEx_GetRxEventType(huart);

		if(eventId == HAL_UART_RXEVENT_IDLE || eventId == HAL_UART_RXEVENT_TC){
			uart2_rx_buf_size = Size;

			// Copy received message into GRBL global buffer
			memcpy(grbl_buffer, uart2_rx_buffer, uart2_rx_buf_size);

			// Clear GRBL buffer from index "Size" to the end of the buffer
			memset(grbl_buffer + uart2_rx_buf_size, 0, GRBL_BUF_SIZE - uart2_rx_buf_size);

			// Clear UART2 message buffer
			memset(uart2_rx_buffer, 0, uart2_rx_buf_size);

			// Parse GRBL data from received message
			parseData((char*)grbl_buffer);
			// Parse GRBL settings from received message
			parseSettings((char*)grbl_buffer);
			// Parse GRBL info from received message
			parseInfo((char*)grbl_buffer);

			// Call function that initiates DMA reception transfer on UART2
			start_grbl_uart_reception();
		}
	}else if(huart->Instance == UART4){ // Event on UART4
		// Copy received message into global structure for pendant data
		memcpy(&pendant_data, uart4_rx_buf, sizeof(pendant_data_t));

		// The code inside this if statement is ran
		// when the system has entered manual mode
		if(menu_controller.entered_manual_mode){
			if(pendant_control.counter >= DATA_REQUEST_CNT){
				// Set flag that shows that message
				// is received form MPG pendant
				pendant_control.request_answered = true;

				// The code inside this if statement is ran when the system has initiated
				// a procedure to turn off the MPG functionality of the pendant
				if(pendant_control.turn_off_mpg){
					// If the MPG functionality of the pendant is
					// already turned off, reset the procedure's flag
					if(!pendant_data.mpg){
						pendant_control.turn_off_mpg = false;
					}

					pendant_data.mpg = false;
					pendant_control.mpg_status = MPG_STATUS_DISABLED;
					pendant_data.system_status = Idle;
					pendant_control.counter = DATA_REQUEST_CNT - 1;
				}else{
					// If pendant MPG functionality is not scheduled to be turned off,
					// process the received message and do the according actions, based on it
					process_pendant_data_to_action();
				}
			}
		}

		// Clear the UART4 message buffer
		memset(uart4_rx_buf, 0, PENDANT_BUF_SIZE);
	}
}


// CUSTOM FUCNTIONS
/**
  * @brief Initiate UART reception on GRBL UART port
  */
void start_grbl_uart_reception(void){
	if(HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_buffer, GRBL_BUF_SIZE) == HAL_OK){
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}
}

/**
  * @brief Check whether given message is GRBL initial global message
  */
bool is_initial_grbl_message(uint8_t*){
	uint8_t message_length = strlen(initial_grbl_message);
	for(uint8_t i = 0; i < message_length; i++){
		if(grbl_buffer[i] != (uint8_t)initial_grbl_message[i]){
			return false;
		}
	}
	return true;
}

/**
  * @brief Start buzzer and keep it on for a given amount of time
  */
bool start_buzzer(uint16_t cnt){
	if(cnt == 0 || buzzer_program_occupied){
		return false;
	}

	buzzer_program_occupied = true;
	buzzer_update_event_cnt = cnt;

	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_3);

	return true;
}

/**
  * @brief Similar to start_grbl_uart_reception() function
  * It is used to start reception on MPG pendant UART port
  */
void start_pendant_reception(void){
	if(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, uart4_rx_buf, PENDANT_BUF_SIZE) == HAL_OK){
		__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
	}
}

/* USER CODE END 1 */
