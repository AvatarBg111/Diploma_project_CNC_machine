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
#include "r61529_screen_menu.h"


/* Private define ------------------------------------------------------------*/
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
#define PENDANT_UART huart4

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern menu_controller_t menu_controller;


/* Private variables ---------------------------------------------------------*/
pendant_control_t pendant_control = {0};
pendant_data_t pendant_data = {0};
char request_body[30] = {0};


/* Private function prototypes -----------------------------------------------*/
void request_pendant_data(void);


/* Private user code -----------------------------------------------*/
/**
  * @brief Send a data request to the MPG pendant
  */
void request_pendant_data(void){
	// Clear reqeust buffer
	memset(request_body, 0, 30);

	// Prepare buffer and send it to the pendant via UART
	sprintf(request_body, "GET_DATA|%d|%d|%d", pendant_control.status, pendant_control.mpg_status, 5000);
	HAL_UART_Transmit_DMA(&PENDANT_UART, (uint8_t*)request_body, strlen(request_body));

	// Initiate a DMA reception transfer on the
	// UART port associated with the MPG pendant
	start_pendant_reception();
}
