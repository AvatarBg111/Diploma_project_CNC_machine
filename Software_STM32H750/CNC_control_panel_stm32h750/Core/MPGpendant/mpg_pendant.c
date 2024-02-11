/*
 * mpg_pendant.c
 *
 *  Created on: Feb 10, 2024
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include "mpg_pendant.h"
#include "systick_timer.h"
#include "stm32h7xx_it.h"
#include "stm32h7xx_hal.h"


/* Private define ------------------------------------------------------------*/
extern UART_HandleTypeDef huart4;
#define PENDANT_UART huart4

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
pendant_control_t pendant_control = {0};
pendant_data_t pendant_data = {0};


/* Private function prototypes -----------------------------------------------*/
void connect_pendant(void);
void disconnect_pendant(void);
void request_pendant_data(void);
uint8_t get_pendant_status(void);
void set_pendant_status(uint8_t);


/* Private user code -----------------------------------------------*/
void connect_pendant(void){
	if(pendant_control.status == PENDANT_DISCONNECTED || pendant_control.status == PENDANT_ERROR){
		pendant_control.status = PENDANT_CONNECTING;
		start_pendant_connection();
	}
}

void disconnect_pendant(void){
	if(pendant_control.status == PENDANT_CONNECTED || pendant_control.status == PENDANT_ERROR){
		pendant_control.status = PENDANT_DISCONNECTING;
		start_pendant_disconnection();
	}
}

void request_pendant_data(void){
	if(pendant_control.status == PENDANT_CONNECTED){
		pendant_control.status = PENDANT_AWAITING_DATA;
		HAL_UART_Transmit_DMA(&PENDANT_UART, (uint8_t*)"GET_DATA", 8);
		start_pendant_reception();
	}
}

uint8_t get_pendant_status(void){
	return pendant_control.status;
}

void set_pendant_status(uint8_t status){
	if(status >= 0 && status <= 6){
		pendant_control.status = status;
	}
}
