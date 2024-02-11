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
#include "mpg_pendant.h"
#include "../grblHALComm/parser.h"
#include "systick_timer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GRBL_BUF_SIZE 256
#define PENDANT_BUF_SIZE 32

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// Touch IC communication
uint8_t touch_detected = 0x00;
uint8_t i2c3_dma_itr_sync = 0x00;

// GRBL UART communication
uint8_t uart2_rx_buffer[GRBL_BUF_SIZE] = {0};
uint16_t uart2_rx_buf_size = 0;

uint8_t grbl_buffer[GRBL_BUF_SIZE] = {0};
uint16_t grbl_buffer_size = 0;
const char initial_grbl_message[] = "GrblHAL 1.1f ['$' or '$HELP' for help]";

// MPG pendant communication
uint8_t uart4_rx_buf[PENDANT_BUF_SIZE] = {0};
uint8_t uart4_tx_buf[PENDANT_BUF_SIZE] = {0};
bool pendant_connecting_procedure_started = false;
bool pendant_disconnecting_procedure_started = false;

// Debug UART communication
uint8_t uart3_tx_buf[50] = {0};

// Buzzer control
bool buzzer_program_occupied = false;
uint16_t buzzer_update_event_cnt = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void grbl_uart_start_reception_stream(void);
bool is_initial_grbl_message(uint8_t*);
void start_pendant_connection(void);
void start_pendant_reception(void);
bool turn_on_buzzer(uint16_t);

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
extern pendant_control_t pendant_control;
extern pendant_data_t pendant_data;

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
	update_button_status();

	if(pendant_connecting_procedure_started){
		if(get_pendant_status() == PENDANT_CONNECTED){
			pendant_connecting_procedure_started = false;
			pendant_control.counter = 0;
			HAL_UART_DMAStop(&huart4);
		}

		if(pendant_control.counter == 0){
			strcpy((char*)uart4_tx_buf, "CONNECT");
			uart4_tx_buf[7] = 0;
		}else if(pendant_control.counter >= CONNECTION_CNT){
			memset(uart4_tx_buf, 0, 7);
		}

		if(++pendant_control.counter >= CONNECTION_CNT){
			pendant_connecting_procedure_started = false;
			pendant_control.counter = 0;

			HAL_UART_DMAStop(&huart4);
			memset(uart4_rx_buf, 0, PENDANT_BUF_SIZE);
			set_pendant_status(PENDANT_ERROR);
		}else if(pendant_control.counter == 1){
			if(HAL_UART_Transmit_DMA(&huart4, uart4_tx_buf, 7) != HAL_OK){
				HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_RESET);
			}
			start_pendant_reception();
		}
	}else if(pendant_disconnecting_procedure_started){
		if(get_pendant_status() == PENDANT_DISCONNECTED){
			pendant_disconnecting_procedure_started = false;
			pendant_control.counter = 0;
			HAL_UART_DMAStop(&huart4);
		}

		if(pendant_control.counter == 0){
			strcpy((char*)uart4_tx_buf, "DISCONNECT");
			uart4_tx_buf[10] = 0;
		}else if(pendant_control.counter >= CONNECTION_CNT){
			memset(uart4_tx_buf, 0, 10);
		}

		if(++pendant_control.counter >= CONNECTION_CNT){
			pendant_disconnecting_procedure_started = false;
			pendant_control.counter = 0;

			HAL_UART_DMAStop(&huart4);
			memset(uart4_rx_buf, 0, PENDANT_BUF_SIZE);
			set_pendant_status(PENDANT_ERROR);
		}else if(pendant_control.counter == 1){
			if(HAL_UART_Transmit_DMA(&huart4, uart4_tx_buf, 10) != HAL_OK){
				HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_RESET);
			}

			start_pendant_reception();
		}
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
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance == USART2){
		HAL_UART_RxEventTypeTypeDef eventId = HAL_UARTEx_GetRxEventType(huart);

		if(eventId == HAL_UART_RXEVENT_IDLE || eventId == HAL_UART_RXEVENT_TC){
			start_grbl_uart_reception();

			uart2_rx_buf_size = Size;
			memcpy(grbl_buffer, uart2_rx_buffer, uart2_rx_buf_size);
			memset(grbl_buffer + uart2_rx_buf_size, 0, GRBL_BUF_SIZE - uart2_rx_buf_size);
			memset(uart2_rx_buffer, 0, uart2_rx_buf_size);

			parseSettings((char*)grbl_buffer);
			parseInfo((char*)grbl_buffer);
			parseData((char*)grbl_buffer);
		}
	}else if(huart->Instance == UART4){
		if(!strcmp((char*)uart4_rx_buf, "CONNECTED")){
			pendant_connecting_procedure_started = false;
			pendant_control.counter = 0;
			set_pendant_status(PENDANT_CONNECTED);
		}else if(!strcmp((char*)uart4_rx_buf, "DISCONNECTED")){
			pendant_disconnecting_procedure_started = false;
			pendant_control.counter = 0;
			set_pendant_status(PENDANT_DISCONNECTED);
		}else if(get_pendant_status() == PENDANT_AWAITING_DATA){
			set_pendant_status(PENDANT_DATA_RECEIVED);
			memcpy(&pendant_data, uart4_rx_buf, sizeof(pendant_data_t));
		}
		memset(uart4_rx_buf, 0, PENDANT_BUF_SIZE);
	}
}


// CUSTOM FUCNTIONS
void start_grbl_uart_reception(void){
	if(HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_buffer, GRBL_BUF_SIZE) == HAL_OK){
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}
}

bool is_initial_grbl_message(uint8_t*){
	uint8_t message_length = strlen(initial_grbl_message);
	for(uint8_t i = 0; i < message_length; i++){
		if(grbl_buffer[i] != (uint8_t)initial_grbl_message[i]){
			return false;
		}
	}
	return true;
}

bool turn_on_buzzer(uint16_t cnt){
	if(cnt == 0 || buzzer_program_occupied){
		return false;
	}

	buzzer_program_occupied = true;
	buzzer_update_event_cnt = cnt;

	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_3);

	return true;
}

void start_pendant_connection(void){
	HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_RESET);
	if(!pendant_disconnecting_procedure_started){
		pendant_connecting_procedure_started = true;
	}
}

void start_pendant_disconnection(void){
	HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_RESET);
	if(!pendant_connecting_procedure_started){
		pendant_disconnecting_procedure_started = true;
	}
}

void start_pendant_reception(void){
	if(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, uart4_rx_buf, PENDANT_BUF_SIZE) != HAL_OK){
		HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_RESET);
	}else{
		__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
	}
}
/* USER CODE END 1 */
