/*
 * grbl_cpu_comm.c
 *
 *  Created on: Sep 13, 2023
 *      Author: AvatarBg111
 */

// Includes
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"
#include "grbl_cpu_comm.h"
#include "parser.h"
#include<string.h>


// Private macros and typedefs
#define RX_BUF_SIZE 160


// Private variables
bool mpg_mode = false;
uint8_t cmd_mpg_mode_toggle[] = {0x8B, 0x0D};


// Private function declarations
HAL_StatusTypeDef grbl_send_msg(uint8_t,uint8_t*,uint8_t);
HAL_StatusTypeDef grbl_recv_pckt(uint8_t,uint8_t*,uint8_t*);
HAL_StatusTypeDef enable_mpg(void);
HAL_StatusTypeDef disable_mpg(void);


// Private function definitions
/**
  * @brief Send message to GRBL controller
  */
HAL_StatusTypeDef grbl_send_msg(uint8_t msg_channel, uint8_t *msg, uint8_t len){
	switch(msg_channel){
		case UART_Gcode_channel:
			return HAL_UART_Transmit(&GCODE_UART_H, msg, len, 100);
		case UART_MPG_channel:
			return HAL_UART_Transmit(&MPG_UART_H, msg, len, 100);
		default:
			break;
	}

	return HAL_ERROR;
}

/**
  * @brief Receive message from GRBL controller
  */
HAL_StatusTypeDef grbl_recv_pckt(uint8_t msg_channel, uint8_t *buf, uint8_t *len){
	switch(msg_channel){
		case UART_Gcode_channel:
			start_uart1_reception(buf, len);
			while(!uart1_reception_done());
			return HAL_OK;
		case UART_MPG_channel:
			//start_mpg_uart_reception();
			return HAL_TIMEOUT;
		default:
			break;
	}

	return HAL_ERROR;
}

/**
  * @brief Enable MPG mode
  */
HAL_StatusTypeDef enable_mpg(void){
	uint8_t buf[RX_BUF_SIZE] = {0}, len = 0;

	grbl_send_msg(UART_Gcode_channel, cmd_mpg_mode_toggle, strlen((char*)cmd_mpg_mode_toggle));
	grbl_recv_pckt(UART_Gcode_channel, buf, &len);

	if(mpg_enabled(buf, len)){
		mpg_mode = true;
		return HAL_OK;
	}

	mpg_mode = false;
	return HAL_ERROR;
}

/**
  * @brief Disable MPG mode
  */
HAL_StatusTypeDef disable_mpg(void){
	uint8_t buf[RX_BUF_SIZE] = {0}, len = 0;

	grbl_send_msg(UART_Gcode_channel, cmd_mpg_mode_toggle, strlen((char*)cmd_mpg_mode_toggle));
	grbl_recv_pckt(UART_Gcode_channel, buf, &len);

	if(mpg_disabled(buf, len)){
		mpg_mode = false;
		return HAL_OK;
	}

	//grbl_send_msg(UART_MPG_channel, buf, len);
	mpg_mode = true;
	return HAL_ERROR;
}

/**
  * @brief Get MPG mode state
  */
bool get_mpg_state(){
	return mpg_mode;
}
