/*
 * grbl_cpu_comm.h
 *
 *  Created on: Sep 13, 2023
 *      Author: AvatarBg111
 */

#ifndef COMM_GRBL_CPU_COMM_H_
#define COMM_GRBL_CPU_COMM_H_

// Includes
#include "main.h"


// Exported macros and typedefs
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
#define GCODE_UART_H huart1
#define MPG_UART_H huart2


typedef uint8_t grbl_comm_stat_t;

enum msg_channel_t{
	UART_Gcode_channel = 0,
	UART_MPG_channel
};

enum grbl_comm_stat_t{
	grbl_cpu_comm_ready = 0,
	grbl_cpu_comm_busy = 1
};

/**
  * @brief Enable MPG mode
  */
HAL_StatusTypeDef enable_mpg(void);

/**
  * @brief Disable MPG mode
  */
HAL_StatusTypeDef disable_mpg(void);

/**
  * @brief Get MPG mode status
  */
bool get_mpg_state(void);

#endif /* COMM_GRBL_CPU_COMM_H_ */
