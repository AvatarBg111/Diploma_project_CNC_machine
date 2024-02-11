/*
 * mpg_pendant.h
 *
 *  Created on: Feb 10, 2024
 *      Author: AvatarBg111
 */

#ifndef __MPG_PENDANT_H
#define __MPG_PENDANT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"


/* Private defines -----------------------------------------------------------*/
#define CONNECTION_CNT 300

#define PENDANT_DISCONNECTED 0
#define PENDANT_CONNECTED 1
#define PENDANT_CONNECTING 2
#define PENDANT_DISCONNECTING 3
#define PENDANT_ERROR 4
#define PENDANT_AWAITING_DATA 5
#define PENDANT_DATA_RECEIVED 6


/* Exported types ------------------------------------------------------------*/
typedef struct{
	uint8_t status;
	uint16_t counter;
}pendant_control_t;

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
void connect_pendant(void);
void disconnect_pendant(void);
void request_pendant_data(void);
uint8_t get_pendant_status(void);
void set_pendant_status(uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* __MPG_PENDANT_H */
