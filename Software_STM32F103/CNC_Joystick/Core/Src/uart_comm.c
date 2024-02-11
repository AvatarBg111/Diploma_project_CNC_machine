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
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


/* Private define ------------------------------------------------------------*/
#define ENCODER_1_TIM htim2
#define ENCODER_2_TIM htim3


/* Private variables ---------------------------------------------------------*/
pendant_data_t pendant_data = {0};
uint8_t uart2_tx_buf[UART_BUF_SIZE] = {0};


/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void send_system_connected(void){
	memset(uart2_tx_buf, 0, UART_BUF_SIZE);
	strcpy((char*)uart2_tx_buf, "CONNECTED");
	HAL_UART_Transmit_DMA(&huart2, uart2_tx_buf, strlen((char*)uart2_tx_buf));
}

void send_system_disconnected(void){
	memset(uart2_tx_buf, 0, UART_BUF_SIZE);
	strcpy((char*)uart2_tx_buf, "DISCONNECTED");
	HAL_UART_Transmit_DMA(&huart2, uart2_tx_buf, strlen((char*)uart2_tx_buf));
}

void send_system_error(void){
	memset(uart2_tx_buf, 0, UART_BUF_SIZE);
	strcpy((char*)uart2_tx_buf, "ERROR");
	HAL_UART_Transmit_DMA(&huart2, uart2_tx_buf, strlen((char*)uart2_tx_buf));
}

void send_system_data(void){
	pendant_data.encoder1_val = ENCODER_1_TIM.Instance->CNT;
	pendant_data.encoder2_val = ENCODER_2_TIM.Instance->CNT;

	//TODO
	pendant_data.x_axis_multiplicity = 0;
	pendant_data.yz_axis_multiplicity = 0;
	pendant_data.y_or_z = 0;
	pendant_data.spindle_mode = 0;
	pendant_data.jog_mode = get_3pos_sw_state();
	pendant_data.buttons = 0x0004 | 0x0008;	//FLOOD and MIST on

	memset(uart2_tx_buf, 0, UART_BUF_SIZE);
	memcpy(uart2_tx_buf, &pendant_data, sizeof(pendant_data_t));
	HAL_UART_Transmit_DMA(&huart2, uart2_tx_buf, sizeof(pendant_data_t));
}
