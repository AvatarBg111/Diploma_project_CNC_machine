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
#define DATA_REQUEST_CNT 9
#define CHECK_DATA_RECEIVED_CNT 10
#define REQUEST_TIMEOUT_CNT 10

#define MPG_STATUS_DISABLED 0
#define MPG_STATUS_ENABLED 1
#define MPG_STATUS_ENABLE_ONGOING 2


/* Exported types ------------------------------------------------------------*/
typedef struct{
	uint8_t counter;
	uint8_t comm_cnt;
	uint8_t mpg_status;
	bool status;
	bool turn_off_mpg;
	bool request_answered;
}pendant_control_t;

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
}pendant_data_t;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void request_pendant_data(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPG_PENDANT_H */
