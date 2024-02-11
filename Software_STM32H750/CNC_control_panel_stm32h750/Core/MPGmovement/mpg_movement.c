/*
 * mpg_movement.c
 *
 *  Created on: Feb 11, 2024
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include <mpg_movement.h>
#include "mpg_pendant.h"
#include "../grblHALComm/parser.h"
#include "../grblHALComm/sender.h"
#include "systick_timer.h"


/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern grbl_data_t grbl_data;


/* Private variables ---------------------------------------------------------*/
mpg_settings_t mpg_settings = {
	.x_axis_multiplicity = {X_AXIS_MULTIPLICITY_DEFAULT_0 ,X_AXIS_MULTIPLICITY_DEFAULT_1, X_AXIS_MULTIPLICITY_DEFAULT_2},
	.yz_axis_multiplicity = {YZ_AXIS_MULTIPLICITY_DEFAULT_0 ,YZ_AXIS_MULTIPLICITY_DEFAULT_1, YZ_AXIS_MULTIPLICITY_DEFAULT_2},
	.jog_speeds = {JOG_SPEED_DEFAULT_SLOW, JOG_SPEED_DEFAULT_MEDIUM, JOG_SPEED_DEFAULT_FAST},
	.spindle_speed = SPINDLE_SPEED_DEFAULT,
	.spindle_mode = SPINDLE_MODE_OFF,
};


/* Private function prototypes -----------------------------------------------*/
void pendant_data_to_action(pendant_action_t);


/* Private user code -----------------------------------------------*/
void pendant_data_to_action(pendant_action_t data){
	double x_multiplicity = mpg_settings.x_axis_multiplicity[data.x_axis_multiplicity];
	double yz_multiplicity = mpg_settings.yz_axis_multiplicity[data.yz_axis_multiplicity];
	double x_movement = data.encoder1_val_diff * x_multiplicity;
	double yz_movement = data.encoder2_val_diff * yz_multiplicity;
	char grbl_movement_str[50] = {0};
	char y_or_z = 0;
	uint8_t spindle_mode = 0;

	// Send movement packet
	if(x_movement != 0.000 || yz_movement != 0.000){
		if(data.y_or_z == YZ_MOVEMENT_Y){
			y_or_z = 'Y';
		}else{
			y_or_z = 'Z';
		}
		sprintf(grbl_movement_str, "$J=G91G21X%f%c%fF%d\n", x_movement, y_or_z, yz_movement, mpg_settings.jog_speeds[data.jog_mode]);
		grbl_send_packet((uint8_t*)grbl_movement_str, strlen(grbl_movement_str));
		memset(grbl_movement_str, 0, strlen(grbl_movement_str));
	}

	// Send spindle packet
	if(grbl_data.spindle.state.on && data.spindle_mode == SPINDLE_MODE_OFF){
		spindle_mode = 5;
	}else if(grbl_data.spindle.state.ccw && data.spindle_mode == SPINDLE_MODE_CW){
		spindle_mode = 3;
	}else if(!grbl_data.spindle.state.ccw && data.spindle_mode == SPINDLE_MODE_CCW){
		spindle_mode = 4;
	}
	if(spindle_mode){
		sprintf(grbl_movement_str, "$J=M%dS%d\n", spindle_mode, mpg_settings.spindle_speed);	//M03/M04/M05	//SPINDLE
		grbl_send_packet((uint8_t*)grbl_movement_str, strlen(grbl_movement_str));
		memset(grbl_movement_str, 0, strlen(grbl_movement_str));
	}

	// Send coolant packet
	if(FLOOD_ON(data.buttons) && MIST_ON(data.buttons) && !(grbl_data.coolant.flood || grbl_data.coolant.mist)){
		strcpy(grbl_movement_str, "$J=M7\n$J=M8\n");
	}else if(FLOOD_ON(data.buttons) && !grbl_data.coolant.flood){
		strcpy(grbl_movement_str, "$J=M9\n$J=M7\n");
	}else if(MIST_ON(data.buttons) && !grbl_data.coolant.mist){
		strcpy(grbl_movement_str, "$J=M9\n$J=M8\n");
	}else if(grbl_data.coolant.flood && grbl_data.coolant.mist){
		strcpy(grbl_movement_str, "$J=M9\n");
	}
	grbl_send_packet((uint8_t*)grbl_movement_str, strlen(grbl_movement_str));
}
