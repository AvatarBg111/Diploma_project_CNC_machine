/*
 * mpg_movement.h
 *
 *  Created on: Feb 11, 2024
 *      Author: AvatarBg111
 */

#ifndef __MPG_MOVEMENT_H
#define __MPG_MOVEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"


/* Private defines -----------------------------------------------------------*/
#define X_AXIS_MULTIPLICITY_DEFAULT_PRECISE 0.01
#define X_AXIS_MULTIPLICITY_DEFAULT_NORMAL 0.1
#define X_AXIS_MULTIPLICITY_DEFAULT_COARSE 1

#define YZ_AXIS_MULTIPLICITY_DEFAULT_PRECISE 0.01
#define YZ_AXIS_MULTIPLICITY_DEFAULT_NORMAL 0.1
#define YZ_AXIS_MULTIPLICITY_DEFAULT_COARSE 1

#define YZ_MOVEMENT_Y 0
#define YZ_MOVEMENT_Z 1

#define JOG_SPEED_DEFAULT_SLOW 500
#define JOG_SPEED_DEFAULT_NORMAL 750
#define JOG_SPEED_DEFAULT_FAST 1000

#define SPINDLE_MODE_OFF 0
#define SPINDLE_MODE_CW 1
#define SPINDLE_MODE_CCW 2
#define SPINDLE_SPEED_DEFAULT 1000
#define SPINDLE_SPEED_MAX 5000
#define SPINDLE_SPEED_MIN 800
#define SPINDLE_SPEED_DIFF 100


/* Exported types ------------------------------------------------------------*/
typedef struct{
	double x_axis_multiplicity[3];
	double yz_axis_multiplicity[3];
	uint16_t jog_speeds[3];
	int16_t spindle_speed;
	uint8_t spindle_mode;
}mpg_settings_t;

typedef struct{
	int16_t encoder1_val_diff;
	int16_t encoder2_val_diff;
	uint8_t x_axis_multiplicity;
	uint8_t yz_axis_multiplicity;
	bool y_or_z;
	uint8_t jog_mode;
	uint8_t spindle_mode;
	uint8_t system_status;
	bool mpg;
	bool flood;
	bool mist;
}pendant_action_t;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void pendant_data_to_action(pendant_action_t);
void process_pendant_data_to_action(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPG_MOVEMENT_H */
