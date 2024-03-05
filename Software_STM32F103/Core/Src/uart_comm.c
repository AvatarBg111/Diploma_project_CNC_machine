/*
 * uart_comm.c
 *
 *  Created on: Sep 9, 2024
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include "uart_comm.h"


/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;


/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
pendant_data_t pendant_data = {0};
uint8_t uart2_tx_buf[UART_BUF_SIZE] = {0};


/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
/**
  * @brief Send system data packet/message via UART
  */
void send_system_data(void){
	// Clear UART2 transmit buffer
	memset(uart2_tx_buf, 0, UART_BUF_SIZE);

	// Copy pendant data global structure into buffer
	memcpy(uart2_tx_buf, &pendant_data, sizeof(pendant_data_t));

	// Transmit buffer via UART
	HAL_UART_Transmit_DMA(&huart2, uart2_tx_buf, sizeof(pendant_data_t));
}
