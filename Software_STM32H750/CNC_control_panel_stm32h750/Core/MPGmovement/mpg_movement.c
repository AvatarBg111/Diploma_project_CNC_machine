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
#include "stm32h7xx_it.h"


/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern grbl_data_t grbl_data;
extern pendant_data_t pendant_data;
extern pendant_control_t pendant_control;

extern UART_HandleTypeDef huart3;
extern uint8_t uart3_tx_buf[100];


/* Private variables ---------------------------------------------------------*/
mpg_settings_t mpg_settings = {
	.x_axis_multiplicity = {X_AXIS_MULTIPLICITY_DEFAULT_0 ,X_AXIS_MULTIPLICITY_DEFAULT_1, X_AXIS_MULTIPLICITY_DEFAULT_2},
	.yz_axis_multiplicity = {YZ_AXIS_MULTIPLICITY_DEFAULT_0 ,YZ_AXIS_MULTIPLICITY_DEFAULT_1, YZ_AXIS_MULTIPLICITY_DEFAULT_2},
	.jog_speeds = {JOG_SPEED_DEFAULT_SLOW, JOG_SPEED_DEFAULT_MEDIUM, JOG_SPEED_DEFAULT_FAST},
	.spindle_speed = SPINDLE_SPEED_DEFAULT,
	.spindle_mode = SPINDLE_MODE_OFF,
};
const double X_AXIS_MULTIPLICITY_DIFF[3] = {0.01, 0.1, 0.1};
const double YZ_AXIS_MULTIPLICITY_DIFF[3] = {0.01, 0.1, 0.1};
const double JOG_SPEED_DIFF[3] = {10, 50, 100};


/* Private function prototypes -----------------------------------------------*/
void pendant_data_to_action(pendant_action_t);
void process_pendant_data_to_action(void);


/* Private user code -----------------------------------------------*/
/**
  * @brief Delete a given sub-menu
  */
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
	}else if(!grbl_data.spindle.state.on && data.spindle_mode == SPINDLE_MODE_CW){
		spindle_mode = 3;
	}else if(!grbl_data.spindle.state.on && data.spindle_mode == SPINDLE_MODE_CCW){
		spindle_mode = 4;
	}
	if(spindle_mode){
		sprintf(grbl_movement_str, "M%dS%d\n", spindle_mode, mpg_settings.spindle_speed);
		grbl_send_packet((uint8_t*)grbl_movement_str, strlen(grbl_movement_str));
		memset(grbl_movement_str, 0, strlen(grbl_movement_str));
	}

	// Set/Reset flood coolant
	grbl_movement_str[1] = '\n';
	if(data.flood != grbl_data.coolant.flood){
		grbl_movement_str[0] = CMD_OVERRIDE_COOLANT_FLOOD_TOGGLE;
		grbl_send_packet((uint8_t*)grbl_movement_str, 2);
	}
	if(data.mist != grbl_data.coolant.mist){
		grbl_movement_str[0] = CMD_OVERRIDE_COOLANT_MIST_TOGGLE;
		grbl_send_packet((uint8_t*)grbl_movement_str, 2);
	}
	grbl_movement_str[0] = 0;
	grbl_movement_str[1] = 0;

	// System status (HOLD, CYCLE_START) TODO
	if(pendant_data.system_status == Idle && grbl_data.grbl.state != Idle && \
	   grbl_data.grbl.state != Run && grbl_data.grbl.state != Jog){
		grbl_movement_str[0] = CMD_CYCLE_START;
		grbl_send_packet((uint8_t*)grbl_movement_str, 1);
	}else if(pendant_data.system_status == Hold && grbl_data.grbl.state != Hold && \
			 grbl_data.grbl.state != Alarm && grbl_data.grbl.state != Check && \
			 grbl_data.grbl.state != Door && grbl_data.grbl.state != Sleep){
		grbl_movement_str[0] = CMD_FEED_HOLD;
		grbl_send_packet((uint8_t*)grbl_movement_str, 1);
	}
	grbl_movement_str[0] = 0;
}

/**
  * @brief Process the received pendant data and do the
  * according actions (via sending messages to GRBL)
  */
void process_pendant_data_to_action(void){
	static pendant_data_t data = {0};
	pendant_data_t current_data;
	pendant_action_t action = {0};

	// Copy current pendant data into local structure
	memcpy(&current_data, &pendant_data, sizeof(pendant_data_t));

	// Encoder X
	if((current_data.encoder1_val > data.encoder1_val) && (current_data.encoder1_val - data.encoder1_val) < 300){
		action.encoder1_val_diff = current_data.encoder1_val - data.encoder1_val;
	}else if(current_data.encoder1_val > data.encoder1_val){
		action.encoder1_val_diff = -((0xFFFF - current_data.encoder1_val) + data.encoder1_val);
	}else if((current_data.encoder1_val < data.encoder1_val) && (data.encoder1_val - current_data.encoder1_val) < 300){
		action.encoder1_val_diff = current_data.encoder1_val - data.encoder1_val;
	}else if(current_data.encoder1_val < data.encoder1_val){
		action.encoder1_val_diff = -((0xFFFF - data.encoder1_val) + current_data.encoder1_val);
	}

	// Encoder YZ
	if((current_data.encoder2_val > data.encoder2_val) && (current_data.encoder2_val - data.encoder2_val) < 300){
		action.encoder2_val_diff = current_data.encoder2_val - data.encoder2_val;
	}else if(current_data.encoder2_val > data.encoder2_val){
		action.encoder2_val_diff = -((0xFFFF - current_data.encoder2_val) + data.encoder2_val);
	}else if((current_data.encoder2_val < data.encoder2_val) && (data.encoder2_val - current_data.encoder2_val) < 300){
		action.encoder2_val_diff = current_data.encoder2_val - data.encoder2_val;
	}else if(current_data.encoder2_val < data.encoder2_val){
		action.encoder2_val_diff = -((0xFFFF - data.encoder2_val) + current_data.encoder2_val);
	}

	// Copy the rest of the data
	action.x_axis_multiplicity = current_data.x_axis_multiplicity;
	action.yz_axis_multiplicity = current_data.yz_axis_multiplicity;
	action.y_or_z = current_data.y_or_z;
	action.spindle_mode = current_data.spindle_mode;
	action.jog_mode = current_data.jog_mode;
	action.system_status = current_data.system_status;
	action.flood = current_data.flood;
	action.mist = current_data.mist;

	/* Logic for pendant MPG functionality control */
	// The code inside this if statement will be ran
	// when the pendant responds with a flag for requesting
	// the control panel to accept the enabling of it's MPG functionality
	if(current_data.mpg){
		// The program will enter this if statement, if
		// GRBL MPG mode is turned off and the control panel
		// is not trying to enable it
		if(!grbl_data.mpgMode && !grbl_data.mpg_enable_ongoing){
			// Re/set flags and variables in
			// order to try to enable GRBL MPG mode
			pendant_control.mpg_status = MPG_STATUS_ENABLE_ONGOING;
			pendant_data.mpg = false;
			pendant_data.encoder1_val = 0;
			pendant_data.encoder2_val = 0;
			current_data.mpg = false;
			current_data.encoder1_val = 0;
			current_data.encoder2_val = 0;
			action.encoder1_val_diff = 0;
			action.encoder2_val_diff = 0;

			// Call a function that sends a message via
			// UART for entering GRBL into MPG mode
			enable_mpg();
		}else if(grbl_data.mpgMode){
			// Enable pendant MPG functionality
			pendant_control.mpg_status = MPG_STATUS_ENABLED;
		}
	}else{ // The pendant MPG functionality is logically turned off
		pendant_control.mpg_status = MPG_STATUS_DISABLED;
		pendant_data.encoder1_val = 0;
		pendant_data.encoder2_val = 0;
		current_data.encoder1_val = 0;
		current_data.encoder2_val = 0;
		action.encoder1_val_diff = 0;
		action.encoder2_val_diff = 0;
	}
	action.mpg = current_data.mpg;

	// The program will enter this if statement, if the pendant
	// is enabled and it's MPG functionality is turned on
	if(pendant_control.status && pendant_control.mpg_status == MPG_STATUS_ENABLED){
		// Call function that translates the received
		// pendant data into the according actions
		pendant_data_to_action(action);
	}

	// Save current data by copying it in
	// a local structure of the same type
	memcpy(&data, &current_data, sizeof(pendant_data_t));
}

