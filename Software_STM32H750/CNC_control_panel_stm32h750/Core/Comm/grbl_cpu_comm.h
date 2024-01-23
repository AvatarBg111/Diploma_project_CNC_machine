/*
 * grbl_cpu_comm.h
 *
 *  Created on: Sep 13, 2023
 *      Author: AvatarBg111
 */

#ifndef __COMM_GRBL_CPU_COMM_H
#define __COMM_GRBL_CPU_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"


/* Exported types ------------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
enum grbl_commands{
	NONE = 0x00,
	_ENABLE_MPG = 0x8B,
	_DISABLE_MPG = 0x85
};


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
HAL_StatusTypeDef enable_mpg(void);
HAL_StatusTypeDef disable_mpg(void);
uint8_t get_pending_command(void);
void reset_pending_command(void);
void set_mpg_state(bool);
bool get_mpg_state(void);


/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __COMM_GRBL_CPU_COMM_H */
