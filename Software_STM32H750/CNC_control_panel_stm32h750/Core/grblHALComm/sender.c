/*
 * sender.c
 *
 *  Created on: Feb 8, 2024
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include "../grblHALComm/sender.h"
#include "../grblHALComm/parser.h"
#include "grbl.h"
#include "systick_timer.h"
#include "stm32h7xx_hal.h"

/* Private define ------------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
#define MPG_UART huart2

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern grbl_data_t grbl_data;


/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
bool enable_mpg(void);
bool disable_mpg(void);
uint8_t grbl_send_packet(uint8_t*,uint16_t);
bool request_report(void);


/* Private user code -----------------------------------------------*/
bool enable_mpg(void){
    uint8_t buffer[2] = {0};

    // Check if MPG is already enabled
    if(grbl_data.mpgMode == true){
    	return true;
    }

    // Send command to enable MPG mode
    buffer[0] = CMD_MPG_MODE_TOGGLE;
    if(HAL_UART_Transmit(&MPG_UART, buffer, 1, 2) != HAL_OK){
    	return false;
    }

    // Wait for MPG mode ON to be acknowledged
    while(grbl_data.mpgMode == false){
    	if(wait_ms_ch(3, 200)){
    		return false;
    	}
    }
    reset_wait_ms_ch(3);

    return true;
}

bool disable_mpg(void){
    uint8_t buffer[2] = {0};

    // Check if MPG is already disabled
    if(grbl_data.mpgMode == false){
    	return true;
    }

    // Send command to disable MPG mode
    buffer[0] = CMD_MPG_MODE_TOGGLE;
    if(HAL_UART_Transmit(&MPG_UART, buffer, 1, 2) != HAL_OK){
    	return false;
    }

    // Wait for MPG mode OFF to be acknowledged
    while(grbl_data.mpgMode == true){
    	if(wait_ms_ch(3, 200)){
    		return false;
    	}
    }
    reset_wait_ms_ch(3);

    return true;
}

uint8_t grbl_send_packet(uint8_t *packet, uint16_t packet_size){
	if(packet_size && HAL_UART_Transmit(&MPG_UART, packet, packet_size, 20) != HAL_OK){
    	return 1;
    }

	return 0;
}

bool request_report(void){
	if(grbl_send_packet((uint8_t*)"?\n", 2)){
    	return false;
    }

	return true;
}
