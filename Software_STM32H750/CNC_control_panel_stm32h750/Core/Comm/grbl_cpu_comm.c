/*
 * grbl_cpu_comm.c
 *
 *  Created on: Sep 13, 2023
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include "grbl_cpu_comm.h"
#include "parser.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"
#include<string.h>


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TX_BUFFER_SIZE 32


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t tx_buffer[TX_BUFFER_SIZE] = {0};
uint8_t pending_command = NONE;
bool mpg_state = false;


/* Private function prototypes -----------------------------------------------*/
HAL_StatusTypeDef grbl_send_msg(uint8_t*, uint8_t);
HAL_StatusTypeDef enable_mpg(void);
HAL_StatusTypeDef disable_mpg(void);
uint8_t get_pending_command(void);
void reset_pending_command(void);
void set_mpg_state(bool);
bool get_mpg_state(void);


/* Private user code ---------------------------------------------------------*/
/**
  * @brief Send message to MCU with GRBL firmware
  */
HAL_StatusTypeDef grbl_send_msg(uint8_t *msg, uint8_t len){
	return HAL_UART_Transmit_DMA(&huart2, msg, len);
}

/**
  * @brief Receive message from MCU with GRBL firmware
  */
/*
HAL_StatusTypeDef grbl_recv_pckt(uint8_t *buf, uint8_t *len){
	start_uart1_reception(buf, len);
	while(!uart1_reception_done());
	return HAL_OK;
}
*/

/**
  * @brief Enable MPG mode
  */
HAL_StatusTypeDef enable_mpg(void){
	sprintf((char*)tx_buffer, "%c", (char)_ENABLE_MPG);

	if(grbl_send_msg(tx_buffer, 1) == HAL_OK){
		pending_command = _ENABLE_MPG;
	}else{
		pending_command = NONE;
		return HAL_ERROR;
	}

	return HAL_OK;
}

/**
  * @brief Disable MPG mode
  */
HAL_StatusTypeDef disable_mpg(void){
	sprintf((char*)tx_buffer, "%c", (char)_DISABLE_MPG);

	if(grbl_send_msg(tx_buffer, 1) == HAL_OK){
		pending_command = _DISABLE_MPG;
	}else{
		pending_command = NONE;
		return HAL_ERROR;
	}

	return HAL_OK;
}

/**
  * @brief Get the pending command
  */
uint8_t get_pending_command(void){
	return pending_command;
}

/**
  * @brief Pull pending command
  */
void reset_pending_command(void){
	pending_command = NONE;
}

/**
  * @brief Set MPG state
  */
void set_mpg_state(bool state){
	mpg_state = state;
}

/**
  * @brief Get MPG state
  */
bool get_mpg_state(void){
	return mpg_state;
}
