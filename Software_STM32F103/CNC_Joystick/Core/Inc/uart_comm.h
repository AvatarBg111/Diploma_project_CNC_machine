/*
 * uart_comm.h
 *
 *  Created on: Feb 9, 2024
 *      Author: AvatarBg111
 */

#ifndef __UART_COMM_H
#define __UART_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"


/* Exported types ------------------------------------------------------------*/
typedef struct{
	uint32_t encoder1_val;
	uint32_t encoder2_val;
	uint8_t x_axis_multiplicity;
	uint8_t yz_axis_multiplicity;
	uint8_t y_or_z;
	uint8_t spindle_mode;
	uint8_t jog_mode;
	uint16_t buttons;
}pendant_data_t;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void send_system_connected(void);
void send_system_disconnected(void);
void send_system_error(void);
void send_system_data(void);


/* Private defines -----------------------------------------------------------*/
#define UART_BUF_SIZE 32


#ifdef __cplusplus
}
#endif

#endif /* __BUTTONS_H */
