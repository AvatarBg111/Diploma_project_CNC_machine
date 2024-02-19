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
	bool y_or_z;
	uint8_t jog_mode;
	uint8_t spindle_mode;
	uint8_t system_status;
	bool mpg;
	bool flood;
	bool mist;
	uint8_t signals;
}pendant_data_t;

typedef struct{
	uint32_t hello;
	uint8_t hello2;
}test_struct;

typedef enum{
    Unknown = 0,
    Idle,
    Run,
    Jog,
    Hold,
    Alarm,
    Check,
    Door,
    Tool,
    Home,
    Sleep
}grbl_state_t;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void send_system_data(void);


/* Private defines -----------------------------------------------------------*/
#define UART_BUF_SIZE 32
#define YZ_MOVEMENT_Y 0
#define YZ_MOVEMENT_Z 1
#define SPINDLE_MODE_OFF 0
#define SPINDLE_MODE_CW 1
#define SPINDLE_MODE_CCW 2

#define MPG_STATUS_DISABLED 0
#define MPG_STATUS_ENABLED 1
#define MPG_STATUS_ENABLE_ONGOING 2

#ifdef __cplusplus
}
#endif

#endif /* __BUTTONS_H */
