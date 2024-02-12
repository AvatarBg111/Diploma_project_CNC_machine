/*
 * screen_menu_r61529.c
 *
 *  Created on: Oct 17, 2023
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include <mpg_movement.h>
#include "r61529_screen_menu.h"
#include "r61529.h"
#include "ft5436.h"
#include "buttons.h"
#include "sound_fx.h"
#include "mpg_pendant.h"
#include "mpg_movement.h"
#include "../grblHALComm/parser.h"
#include "../grblHALComm/sender.h"
#include "systick_timer.h"
#include "stm32h7xx_it.h"
#include "stm32h7xx_hal.h"


/* Private define ------------------------------------------------------------*/
#define LEVELS 4
#define MAXIMUM_BRIGHTNESS	100
#define MINIMUM_BRIGHTNESS	10

#define SUBMENUS 3
#define SUBMENU_BLOCK_WIDTH 150
#define SUBMENU_BLOCK_HEIGHT 60
#define SUBMENU_BLOCK_X_OFFSET (480 - (SUBMENU_BLOCK_WIDTH * SUBMENUS)) / (SUBMENUS + 1)
#define SUBMENU_BLOCK_X_MARGIN SUBMENU_BLOCK_X_OFFSET
#define SUBMENU_BLOCK_Y_MARGIN 30

#define SETTINGS_SUBMENUS 3
#define SETTING_BLOCK_WIDTH 300
#define SETTING_BLOCK_HEIGHT 40
#define SETTING_BLOCK_X_MARGIN 20
#define SETTING_BLOCK_Y_MARGIN 60
#define SETTING_BLOCK_Y_OFFSET 20

#define MPG_AXES_SETTINGS 3
#define MPG_AXES_SETTING_BLOCK_WIDTH 300
#define MPG_AXES_SETTING_BLOCK_HEIGHT 30
#define MPG_AXES_SETTING_BLOCK_X_MARGIN 20
#define MPG_AXES_SETTING_BLOCK_Y_MARGIN 80
#define MPG_AXES_SETTING_BLOCK_Y_OFFSET 20

#define X_AXIS_DIST_SETTINGS 3
#define X_AXIS_DIST_SETTING_BLOCK_WIDTH 300
#define X_AXIS_DIST_SETTING_BLOCK_HEIGHT 50
#define X_AXIS_DIST_SETTING_X_MARGIN 40
#define X_AXIS_DIST_SETTING_Y_MARGIN 80
#define X_AXIS_DIST_SETTING_Y_OFFSET 20

#define YZ_AXIS_DIST_SETTINGS 3
#define YZ_AXIS_DIST_SETTING_BLOCK_WIDTH 300
#define YZ_AXIS_DIST_SETTING_BLOCK_HEIGHT 50
#define YZ_AXIS_DIST_SETTING_X_MARGIN 40
#define YZ_AXIS_DIST_SETTING_Y_MARGIN 80
#define YZ_AXIS_DIST_SETTING_Y_OFFSET 20

#define JOG_FEED_SETTINGS 3
#define JOG_FEED_SETTING_BLOCK_WIDTH 300
#define JOG_FEED_SETTING_BLOCK_HEIGHT 50
#define JOG_FEED_SETTING_X_MARGIN 40
#define JOG_FEED_SETTING_Y_MARGIN 80
#define JOG_FEED_SETTING_Y_OFFSET 20

#define MPG_SPINDLE_SETTINGS 1
#define MPG_SPINDLE_SETTING_BLOCK_WIDTH 250
#define MPG_SPINDLE_SETTING_BLOCK_HEIGHT 40
#define MPG_SPINDLE_SETTING_BLOCK_X_MARGIN 20
#define MPG_SPINDLE_SETTING_BLOCK_Y_MARGIN 80
#define MPG_SPINDLE_SETTING_BLOCK_Y_OFFSET 20

#define SPINDLE_SPEED_SETTING_SLIDER_BASE_X	40
#define SPINDLE_SPEED_SETTING_SLIDER_BASE_Y 80
#define SPINDLE_SPEED_SETTING_SLIDER_WIDTH	300
#define SPINDLE_SPEED_SETTING_SLIDER_HEIGHT	30
#define SPINDLE_SPEED_SETTING_SLIDER_END_X	(SPINDLE_SPEED_SETTING_SLIDER_WIDTH + SPINDLE_SPEED_SETTING_SLIDER_BASE_X)
#define SPINDLE_SPEED_SETTING_SLIDER_END_Y	(SPINDLE_SPEED_SETTING_SLIDER_HEIGHT + SPINDLE_SPEED_SETTING_SLIDER_BASE_Y)

#define SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X	20
#define SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y 60
#define SCREEN_BRIGHTNESS_SETTING_SLIDER_WIDTH	300
#define SCREEN_BRIGHTNESS_SETTING_SLIDER_HEIGHT	30
#define SCREEN_BRIGHTNESS_SETTING_SLIDER_END_X	(SCREEN_BRIGHTNESS_SETTING_SLIDER_WIDTH + SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X)
#define SCREEN_BRIGHTNESS_SETTING_SLIDER_END_Y	(SCREEN_BRIGHTNESS_SETTING_SLIDER_HEIGHT + SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y)

#define BUTTONS 6
#define BUTTON_UP_CH 0
#define BUTTON_DOWN_CH 1
#define BUTTON_LEFT_CH 2
#define BUTTON_RIGHT_CH 3
#define BUTTON_ENTER_CH 4
#define BUTTON_BACK_CH 5

#define INPUT_DELAY_TIME 20
#define DEFAULT_DELAY 200


/* Private typedef -----------------------------------------------------------*/
typedef struct{
	uint8_t level;
	uint16_t level_indexes[LEVELS];
	uint8_t button_delay;
	uint8_t button_delay_cnts[BUTTONS];

	int8_t brightness_value;
	int8_t buzzer_volume;
	uint8_t touch_point_cnt;
	int16_t touch_points[FT_REG_NUMTOUCHES][2];
}_menu_controller;


/* Private macro -------------------------------------------------------------*/
#define GET_SUBMENU_BLOCK_BASE_X(index) (SUBMENU_BLOCK_X_MARGIN + (index * SUBMENU_BLOCK_X_OFFSET) + (index * SUBMENU_BLOCK_WIDTH))
#define GET_SUBMENU_BLOCK_BASE_Y SUBMENU_BLOCK_Y_MARGIN
#define GET_SUBMENU_BLOCK_TEXT_BASE_X(index) (GET_SUBMENU_BLOCK_BASE_X(index) + ((SUBMENU_BLOCK_WIDTH - (strlen(main_menu_options[index]) * 11)) / 2))
#define GET_SUBMENU_BLOCK_TEXT_BASE_Y (SUBMENU_BLOCK_Y_MARGIN + ((SUBMENU_BLOCK_HEIGHT - 18) / 2))

#define GET_SETTING_BLOCK_BASE_X SETTING_BLOCK_X_MARGIN
#define GET_SETTING_BLOCK_BASE_Y(index) (SETTING_BLOCK_Y_MARGIN + (index * SETTING_BLOCK_Y_OFFSET) + (index * SETTING_BLOCK_HEIGHT))
#define GET_SETTING_BLOCK_TEXT_BASE_X (GET_SETTING_BLOCK_BASE_X + ((SETTING_BLOCK_WIDTH - (strlen(settings_options[index]) * 11)) / 2))
#define GET_SETTING_BLOCK_TEXT_BASE_Y(index) (GET_SETTING_BLOCK_BASE_Y(index) + ((SETTING_BLOCK_HEIGHT - 18) / 2))

#define GET_MPG_AXES_SETTING_BLOCK_BASE_X MPG_AXES_SETTING_BLOCK_X_MARGIN
#define GET_MPG_AXES_SETTING_BLOCK_BASE_Y(index) (MPG_AXES_SETTING_BLOCK_Y_MARGIN + (index * MPG_AXES_SETTING_BLOCK_Y_OFFSET) + (index * MPG_AXES_SETTING_BLOCK_HEIGHT))
#define GET_MPG_AXES_SETTING_BLOCK_TEXT_BASE_X (GET_MPG_AXES_SETTING_BLOCK_BASE_X + ((MPG_AXES_SETTING_BLOCK_WIDTH - (strlen(mpg_axes_settings[index]) * 11)) / 2))
#define GET_MPG_AXES_SETTING_BLOCK_TEXT_BASE_Y(index) (GET_MPG_AXES_SETTING_BLOCK_BASE_Y(index) + ((MPG_AXES_SETTING_BLOCK_HEIGHT - 18) / 2))

#define GET_X_AXIS_DIST_SETTING_BLOCK_BASE_X X_AXIS_DIST_SETTING_X_MARGIN
#define GET_X_AXIS_DIST_SETTING_BLOCK_BASE_Y(index) (X_AXIS_DIST_SETTING_Y_MARGIN + (index * X_AXIS_DIST_SETTING_Y_OFFSET) + (index * X_AXIS_DIST_SETTING_BLOCK_HEIGHT))
#define GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X (GET_X_AXIS_DIST_SETTING_BLOCK_BASE_X + 10)
#define GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(index) (GET_X_AXIS_DIST_SETTING_BLOCK_BASE_Y(index) + ((X_AXIS_DIST_SETTING_BLOCK_HEIGHT - 26) / 2))

#define GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_X YZ_AXIS_DIST_SETTING_X_MARGIN
#define GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_Y(index) (YZ_AXIS_DIST_SETTING_Y_MARGIN + (index * YZ_AXIS_DIST_SETTING_Y_OFFSET) + (index * YZ_AXIS_DIST_SETTING_BLOCK_HEIGHT))
#define GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X (GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_X + 10)
#define GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(index) (GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_Y(index) + ((YZ_AXIS_DIST_SETTING_BLOCK_HEIGHT - 26) / 2))

#define GET_JOG_FEED_SETTING_BLOCK_BASE_X JOG_FEED_SETTING_X_MARGIN
#define GET_JOG_FEED_SETTING_BLOCK_BASE_Y(index) (JOG_FEED_SETTING_Y_MARGIN + (index * JOG_FEED_SETTING_Y_OFFSET) + (index * JOG_FEED_SETTING_BLOCK_HEIGHT))
#define GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_X (GET_JOG_FEED_SETTING_BLOCK_BASE_X + 10)
#define GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_Y(index) (GET_JOG_FEED_SETTING_BLOCK_BASE_Y(index) + ((JOG_FEED_SETTING_BLOCK_HEIGHT - 26) / 2))

#define GET_MPG_SPINDLE_SETTING_BLOCK_BASE_X MPG_SPINDLE_SETTING_BLOCK_X_MARGIN
#define GET_MPG_SPINDLE_SETTING_BLOCK_BASE_Y(index) (MPG_SPINDLE_SETTING_BLOCK_Y_MARGIN + (index * MPG_SPINDLE_SETTING_BLOCK_Y_OFFSET) + (index * MPG_SPINDLE_SETTING_BLOCK_HEIGHT))
#define GET_MPG_SPINDLE_SETTING_BLOCK_TEXT_BASE_X (GET_MPG_SPINDLE_SETTING_BLOCK_BASE_X + ((MPG_SPINDLE_SETTING_BLOCK_WIDTH - (strlen(mpg_spindle_settings[index]) * 11)) / 2))
#define GET_MPG_SPINDLE_SETTING_BLOCK_TEXT_BASE_Y(index) (GET_MPG_SPINDLE_SETTING_BLOCK_BASE_Y(index) + ((MPG_SPINDLE_SETTING_BLOCK_HEIGHT - 18) / 2))


/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern uint8_t touch_detected;
extern uint16_t touchX[FT_REG_NUMTOUCHES];
extern uint16_t touchY[FT_REG_NUMTOUCHES];

extern grbl_data_t grbl_data;
extern settings_t settings;

extern mpg_settings_t mpg_settings;
extern const double X_AXIS_MULTIPLICITY_DIFF[3];
extern const double YZ_AXIS_MULTIPLICITY_DIFF[3];
extern const double JOG_SPEED_DIFF[3];

extern pendant_data_t pendant_data;


/* Private variables ---------------------------------------------------------*/
_menu_controller menu_controller = {0};
static const char *main_menu_options[SUBMENUS] = {
	"Auto mode",
	"Manual mode",
	"Settings",
};
static const char *settings_options[SETTINGS_SUBMENUS] = {
	"Screen brightness",
	"MPG axes settings",
	"MPG spindle settings",
};
static const char *mpg_axes_settings[MPG_AXES_SETTINGS] = {
	"X axis jogging distances",
	"Y/Z axes jogging distances",
	"Jogging feed rates",
};
static const char *mpg_spindle_settings[MPG_SPINDLE_SETTINGS] = {
	"Jogging spindle speed",
};
static const char *x_axis_dist_settings[X_AXIS_DIST_SETTINGS] = {
	"SLOW",
	"MEDIUM",
	"FAST",
};
static const char *yz_axis_dist_settings[YZ_AXIS_DIST_SETTINGS] = {
	"SLOW",
	"MEDIUM",
	"FAST",
};
static const char *jog_feed_settings[JOG_FEED_SETTINGS] = {
	"SLOW",
	"MEDIUM",
	"FAST",
};


//Auto and Manual mode variables
static grbl_state_t grbl_state = -1;
static double grbl_position[3] = {-1,-1,-1};
static double grbl_offset[3] = {-1,-1,-1};
static bool grbl_coolant_status[3] = {false, false, false};
static double grbl_rpm = -1;
static uint8_t mpg_state = -1;
static uint8_t pendant_status = -1;
static uint8_t pendant_jog_mode = -1;


/* Private function prototypes -----------------------------------------------*/
// Button functions
void init_button_inputs(void);
void clear_button_channel_cnts(void);

// Menu logic functions
int8_t touch_select_submenu(uint8_t);
bool touch_exit_submenu(uint8_t);
void process_user_input(void);
void r61529_screen_menu(void);

// Menu drawing functions
void draw_menu(void);
void delete_menu(void);
void draw_loading_screen(void);

// Sub-menu drawing functions
void delete_submenu(uint8_t);
void select_submenu(uint8_t);
void unselect_submenu(uint8_t);

// Setting drawing functions
void delete_setting(uint8_t);
void select_setting(uint8_t);
void unselect_setting(uint8_t);

// MPG axes setting drawing functions
void mpg_axes_delete_setting(uint8_t);
void mpg_axes_select_setting(uint8_t);
void mpg_axes_unselect_setting(uint8_t);

// MPG spindle setting drawing functions
void mpg_spindle_delete_setting(uint8_t);
void mpg_spindle_select_setting(uint8_t);
void mpg_spindle_unselect_setting(uint8_t);

// Level 1 menu special functions
void auto_mode_update_screen(void);
void manual_mode_update_screen(void);

// Level 2 menu special functions
void screen_brightness_setting_update_screen(void);

// Level 4 menu special functions
void delete_x_axis_dist_setting(uint8_t);
void select_x_axis_dist_setting(uint8_t);
void unselect_x_axis_dist_setting(uint8_t);
void delete_yz_axis_dist_setting(uint8_t);
void select_yz_axis_dist_setting(uint8_t);
void unselect_yz_axis_dist_setting(uint8_t);
void delete_jog_feed_setting(uint8_t);
void select_jog_feed_setting(uint8_t);
void unselect_jog_feed_setting(uint8_t);
void mpg_axes_update_settings(void);
void mpg_spindle_update_setting(void);

// Other functions
void process_pendant_data_to_action(void);


/* Private user code -----------------------------------------------*/
//// Button functions ////
/**
  * @brief Initialize button inputs
  * Button channels (physical to logical representation)
  *     [0]		A
  *  [2]   [3]	C,B
  *     [1]		D
  *  [4]   [5]	F,E
  *
  * 0 - UP, 1 - DOWN, 2 - LEFT, 3 - RIGHT, 4 - ENTER, 5 - BACK
  */
void init_button_inputs(void){
	set_button_channel(USER_BUT_A_GPIO_Port, USER_BUT_A_Pin, BUTTON_RIGHT_CH);
	set_button_channel(USER_BUT_B_GPIO_Port, USER_BUT_B_Pin, BUTTON_UP_CH);
	set_button_channel(USER_BUT_C_GPIO_Port, USER_BUT_C_Pin, BUTTON_LEFT_CH);
	set_button_channel(USER_BUT_D_GPIO_Port, USER_BUT_D_Pin, BUTTON_DOWN_CH);
	set_button_channel(USER_BUT_E_GPIO_Port, USER_BUT_E_Pin, BUTTON_BACK_CH);
	set_button_channel(USER_BUT_F_GPIO_Port, USER_BUT_F_Pin, BUTTON_ENTER_CH);

	menu_controller.button_delay = (uint8_t)(DEFAULT_DELAY / INPUT_DELAY_TIME);
	for(uint8_t i = 0; i < BUTTONS; i++)
		menu_controller.button_delay_cnts[i] = 0;
}

/**
  * @brief Clear the counters of the button channels
  * (the counters are used for slowing down the effect of the buttons)
  */
void clear_button_channel_cnts(void){
	for(uint8_t i = 0; i < 6; i++){
		if(get_button_state(i) == GPIO_PIN_SET)
			menu_controller.button_delay_cnts[i] = 0;
	}
}





//// Menu logic functions////
/**
  * @brief Return if sub-menu is selected using touch screen
  */
int8_t touch_select_submenu(uint8_t level){
	int8_t submenu = -1;

	if(menu_controller.touch_point_cnt){
		uint16_t x = menu_controller.touch_points[0][0];
		uint16_t y = menu_controller.touch_points[0][1];
		switch(level){
			case 0:	// Main menu
				for(uint8_t i = 0; i < SUBMENUS; i++){
					uint16_t base_x = GET_SUBMENU_BLOCK_BASE_X(i);
					uint16_t base_y = GET_SUBMENU_BLOCK_BASE_Y;

					if((x > base_x && x < (base_x + SUBMENU_BLOCK_WIDTH)) && (y > base_y && y < (base_y + SUBMENU_BLOCK_HEIGHT))){
						submenu = i;
						break;
					}
				}
				break;
			case 1:
				switch(((menu_controller.level_indexes[0] & 0xFF00) >> 8)){
					case 2: // Settings
						for(uint8_t i = 0; i < SUBMENUS; i++){
							uint16_t base_x = GET_SETTING_BLOCK_BASE_X;
							uint16_t base_y = GET_SETTING_BLOCK_BASE_Y(i);

							if((x > base_x && x < (base_x + SETTING_BLOCK_WIDTH)) && (y > base_y && y < (base_y + SETTING_BLOCK_HEIGHT))){
								submenu = i;
								break;
							}
						}
						break;
					default:
						break;
				}
				break;
			case 2:
				if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 2){
					switch(((menu_controller.level_indexes[1] & 0xFF00) >> 8)){
						case 1: // MPG axes settings
							for(uint8_t i = 0; i < MPG_AXES_SETTINGS; i++){
								uint16_t base_x = GET_MPG_AXES_SETTING_BLOCK_BASE_X;
								uint16_t base_y = GET_MPG_AXES_SETTING_BLOCK_BASE_Y(i);

								if((x > base_x && x < (base_x + MPG_AXES_SETTING_BLOCK_WIDTH)) && (y > base_y && y < (base_y + MPG_AXES_SETTING_BLOCK_HEIGHT))){
									submenu = i;
									break;
								}
							}
							break;
						case 2: // MPG spindle settings
							for(uint8_t i = 0; i < MPG_SPINDLE_SETTINGS; i++){
								uint16_t base_x = GET_MPG_SPINDLE_SETTING_BLOCK_BASE_X;
								uint16_t base_y = GET_MPG_SPINDLE_SETTING_BLOCK_BASE_Y(i);

								if((x > base_x && x < (base_x + MPG_SPINDLE_SETTING_BLOCK_WIDTH)) && (y > base_y && y < (base_y + MPG_SPINDLE_SETTING_BLOCK_HEIGHT))){
									submenu = i;
									break;
								}
							}
							break;
						default:
							break;
					}
				}
				break;
			default:
				break;
		}
	}

	return submenu;
}

/**
  * @brief Exit sub-menu if touch screen detects touch in top right corner of screen
  */
bool touch_exit_submenu(uint8_t level){
	if(menu_controller.touch_point_cnt){
		uint16_t x = menu_controller.touch_points[0][0];
		uint16_t y = menu_controller.touch_points[0][1];

		switch(level){
			case 1:
				if(x > 410 && y < 40){
					return true;
				}
			case 2:
				if(x > 410 && y < 40){
					return true;
				}
				break;
			case 3:
				if(x > 410 && y < 40){
					return true;
				}
				break;
			default:
				break;
		}
	}

	return false;
}

/**
  * @brief Function that implements graphical menu of the system
  */
void r61529_screen_menu(void){
	if(wait_ms_ch(0, INPUT_DELAY_TIME)){
		int8_t touch_selected_submenu;
		int8_t index;

		// Check for screen touch data
		menu_controller.touch_point_cnt = touch_detected;
		for(uint8_t i = 0; (i < FT_REG_NUMTOUCHES) && touch_detected; i++){
			if(i >= menu_controller.touch_point_cnt){
				menu_controller.touch_points[i][0] = -1;					//Touch point not available
				menu_controller.touch_points[i][1] = -1;					//Touch point not available
			}else{
				menu_controller.touch_points[i][0] = touchY[i];				//This is because of screen rotation
				menu_controller.touch_points[i][1] = (320 - touchX[i]);		//This is because of screen rotation
			}
		}

		// Clear the counters of the button channels
		clear_button_channel_cnts();

		// Set LEDs
		if(grbl_data.grbl.state == Run || grbl_data.grbl.state == Jog){
			HAL_GPIO_WritePin(USER_LED_B_GPIO_Port, USER_LED_B_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(USER_LED_C_GPIO_Port, USER_LED_C_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(USER_LED_D_GPIO_Port, USER_LED_D_Pin, GPIO_PIN_RESET);
		}else if(grbl_data.grbl.state == Hold){
			HAL_GPIO_WritePin(USER_LED_B_GPIO_Port, USER_LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(USER_LED_C_GPIO_Port, USER_LED_C_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(USER_LED_D_GPIO_Port, USER_LED_D_Pin, GPIO_PIN_RESET);
		}else if(grbl_data.grbl.state == Alarm || grbl_data.grbl.state == Check || grbl_data.grbl.state == Door){
			HAL_GPIO_WritePin(USER_LED_B_GPIO_Port, USER_LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(USER_LED_C_GPIO_Port, USER_LED_C_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(USER_LED_D_GPIO_Port, USER_LED_D_Pin, GPIO_PIN_SET);
		}else if(grbl_data.grbl.state == Idle){
			HAL_GPIO_WritePin(USER_LED_B_GPIO_Port, USER_LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(USER_LED_C_GPIO_Port, USER_LED_C_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(USER_LED_D_GPIO_Port, USER_LED_D_Pin, GPIO_PIN_RESET);
		}
		if(grbl_data.coolant.flood || grbl_data.coolant.mist || grbl_data.coolant.shower || grbl_data.coolant.trough_spindle){
			HAL_GPIO_WritePin(USER_LED_A_GPIO_Port, USER_LED_A_Pin, GPIO_PIN_SET);
		}else{
			HAL_GPIO_WritePin(USER_LED_A_GPIO_Port, USER_LED_A_Pin, GPIO_PIN_RESET);
		}

		// Go to a given sub-menu based on touch screen data
		touch_selected_submenu = touch_select_submenu(menu_controller.level);
		if(touch_selected_submenu != -1){
			delete_menu();
			menu_controller.level_indexes[menu_controller.level] = touch_selected_submenu | (touch_selected_submenu << 8);
			menu_controller.level++;
			draw_menu();
			return;
		}

		switch(menu_controller.level){
			case 0:		//Main menu
				index = (int8_t)menu_controller.level_indexes[0] & 0x00FF;
				if(get_button_state(BUTTON_RIGHT_CH) == GPIO_PIN_RESET){
					if(++menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == 1){
						if(menu_controller.button_delay_cnts[BUTTON_RIGHT_CH]++ != 1){
							menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] = 0;
						}

						if(++index >= SUBMENUS)	index = 0;
					}
				}else if(get_button_state(BUTTON_LEFT_CH) == GPIO_PIN_RESET){
					if(++menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == 1){
						if(menu_controller.button_delay_cnts[BUTTON_LEFT_CH]++ != 1){
							menu_controller.button_delay_cnts[BUTTON_LEFT_CH] = 0;
						}

						if(--index < 0)	index = SUBMENUS - 1;
					}
				}else if(get_button_state(BUTTON_ENTER_CH) == GPIO_PIN_RESET){
					delete_menu();
					menu_controller.level_indexes[0] &= 0x00FF;
					menu_controller.level_indexes[0] |= (menu_controller.level_indexes[0] << 8);
					menu_controller.level = 1;
					draw_menu();
					break;
				}

				if(index != (menu_controller.level_indexes[0] & 0x00FF)){
					unselect_submenu((menu_controller.level_indexes[0] & 0x00FF));
					menu_controller.level_indexes[0] &= 0x0000;
					menu_controller.level_indexes[0] |= index;
					select_submenu((menu_controller.level_indexes[0] & 0x00FF));
					buzzer_short_ring(2500, 100);
					//buzzer_short_ring(1250, (uint16_t)(menu_controller.button_delay * INPUT_DELAY_TIME));
				}
				break;
			case 1:
				if(touch_exit_submenu(1)){
					delete_menu();
					menu_controller.level = 0;
					menu_controller.level_indexes[1] = 0;
					menu_controller.level_indexes[0] &= 0x00FF;
					draw_menu();
					break;
				}else if(get_button_state(BUTTON_BACK_CH) == GPIO_PIN_RESET){
					delete_menu();
					menu_controller.level = 0;
					menu_controller.level_indexes[1] = 0;
					menu_controller.level_indexes[0] &= 0x00FF;
					draw_menu();
					break;
				}

				if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 0){
					auto_mode_update_screen();
				}else if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 1){
					manual_mode_update_screen();

					if(get_button_state(BUTTON_ENTER_CH) == GPIO_PIN_RESET){
						if(++menu_controller.button_delay_cnts[BUTTON_ENTER_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_ENTER_CH] == 1){
							static uint8_t procedure_id = 0x00;

							if(menu_controller.button_delay_cnts[BUTTON_ENTER_CH]++ != 1){
								menu_controller.button_delay_cnts[BUTTON_ENTER_CH] = 0;
							}

							if(get_pendant_status() == PENDANT_DISCONNECTED || \
							  (get_pendant_status() == PENDANT_ERROR && procedure_id == 0x00)){
								procedure_id = 0x00;
								connect_pendant();
							}else if(get_pendant_status() == PENDANT_CONNECTED || \
							  (get_pendant_status() == PENDANT_ERROR && procedure_id == 0x01)){
								procedure_id = 0x01;
								disconnect_pendant();
							}
						}
					}

					if(get_pendant_status() == PENDANT_CONNECTED && !grbl_data.mpgMode){
						enable_mpg();
					}else if(get_pendant_status() == PENDANT_DISCONNECTED && grbl_data.mpgMode){
						disable_mpg();
					}

					if(grbl_data.mpgMode){
						// Request GRBL data
						if(wait_ms_ch(4, 100)){
							request_report();
						}

						// Request pendant data
						if(wait_ms_ch(5, 100) && get_pendant_status() == PENDANT_CONNECTED){
							request_pendant_data();
						}else if(get_pendant_status() == PENDANT_DATA_RECEIVED){
							reset_wait_ms_ch(5);
							process_pendant_data_to_action();
							set_pendant_status(PENDANT_CONNECTED);
						}

						// Use button presses for machine movement
						if(get_button_state(BUTTON_RIGHT_CH) == GPIO_PIN_RESET){ // X -> +1mm
							if(++menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_RIGHT_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] = 0;
								}

								grbl_send_packet((uint8_t*)"$J=G91G21X1F1000\n", 17);
							}
						}else if(get_button_state(BUTTON_LEFT_CH) == GPIO_PIN_RESET){ // X -> -1mm
							if(++menu_controller.button_delay_cnts[BUTTON_LEFT_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_LEFT_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_LEFT_CH] = 0;
								}

								grbl_send_packet((uint8_t*)"$J=G91G21X-1F1000\n", 18);
							}
						}else if(get_button_state(BUTTON_UP_CH) == GPIO_PIN_RESET){ // Y -> +1mm
							if(++menu_controller.button_delay_cnts[BUTTON_UP_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_UP_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_UP_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_UP_CH] = 0;
								}

								grbl_send_packet((uint8_t*)"$J=G91G21Y1F1000\n", 17);
							}
						}else if(get_button_state(BUTTON_DOWN_CH) == GPIO_PIN_RESET){ // Y -> -1mm
							if(++menu_controller.button_delay_cnts[BUTTON_DOWN_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_DOWN_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_DOWN_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_DOWN_CH] = 0;
								}

								grbl_send_packet((uint8_t*)"$J=G91G21Y-1F1000\n", 18);
							}
						}
					}
				}else if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 2){
					int8_t index = menu_controller.level_indexes[1] & 0x00FF;
					if(get_button_state(BUTTON_DOWN_CH) == GPIO_PIN_RESET){
						if(++menu_controller.button_delay_cnts[BUTTON_DOWN_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_DOWN_CH] == 1){
							if(menu_controller.button_delay_cnts[BUTTON_DOWN_CH]++ != 1){
								menu_controller.button_delay_cnts[BUTTON_DOWN_CH] = 0;
							}

							if(++index == SETTINGS_SUBMENUS)	index = 0;
						}
					}else if(get_button_state(BUTTON_UP_CH) == GPIO_PIN_RESET){
						if(++menu_controller.button_delay_cnts[BUTTON_UP_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_UP_CH] == 1){
							if(menu_controller.button_delay_cnts[BUTTON_UP_CH]++ != 1){
								menu_controller.button_delay_cnts[BUTTON_UP_CH] = 0;
							}

							if(--index < 0)	index = SETTINGS_SUBMENUS - 1;
						}
					}else if(get_button_state(BUTTON_ENTER_CH) == GPIO_PIN_RESET){
						delete_menu();
						menu_controller.level = 2;
						menu_controller.level_indexes[1] &= 0x00FF;
						menu_controller.level_indexes[1] |= menu_controller.level_indexes[1] << 8;
						draw_menu();
						break;
					}

					if(index != (menu_controller.level_indexes[1] & 0x00FF)){
						unselect_setting((menu_controller.level_indexes[1] & 0x00FF));
						menu_controller.level_indexes[1] &= 0x0000;
						menu_controller.level_indexes[1] |= index;
						select_setting((menu_controller.level_indexes[1] & 0x00FF));
						buzzer_short_ring(2500, 100);
						//buzzer_short_ring(1250, (uint16_t)(menu_controller.button_delay * INPUT_DELAY_TIME));
					}
				}
				break;
			case 2:
				if(touch_exit_submenu(2)){
					delete_menu();
					menu_controller.level = 1;
					menu_controller.level_indexes[2] = 0;
					menu_controller.level_indexes[1] &= 0x00FF;
					draw_menu();
					break;
				}else if(get_button_state(BUTTON_BACK_CH) == GPIO_PIN_RESET){
					delete_menu();
					menu_controller.level = 1;
					menu_controller.level_indexes[2] = 0;
					menu_controller.level_indexes[1] &= 0x00FF;
					draw_menu();
					break;
				}

				if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 2){
					if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 0){
						int8_t previous_brightness_value = menu_controller.brightness_value;

						if(get_button_state(BUTTON_RIGHT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_RIGHT_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] = 0;
								}

								if(++previous_brightness_value > MAXIMUM_BRIGHTNESS)	previous_brightness_value = MAXIMUM_BRIGHTNESS;
							}
						}else if(get_button_state(BUTTON_LEFT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_LEFT_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_LEFT_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_LEFT_CH] = 0;
								}

								if(--previous_brightness_value < MINIMUM_BRIGHTNESS)	previous_brightness_value = MINIMUM_BRIGHTNESS;
							}
						}

						if(previous_brightness_value != menu_controller.brightness_value){
							menu_controller.brightness_value = previous_brightness_value;
							htim3.Instance->CCR2 = (menu_controller.brightness_value * htim3.Instance->ARR) / 100;
							screen_brightness_setting_update_screen();
						}
					}else if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 1 || \
							 ((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 2){
						index = menu_controller.level_indexes[2] & 0x00FF;
						if(get_button_state(BUTTON_DOWN_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_DOWN_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_DOWN_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_DOWN_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_DOWN_CH] = 0;
								}

								if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 1){
									if(++index >= MPG_AXES_SETTINGS) index = 0;
								}else{
									if(++index >= MPG_SPINDLE_SETTINGS)	index = 0;
								}
							}
						}else if(get_button_state(BUTTON_UP_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_UP_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_UP_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_UP_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_UP_CH] = 0;
								}

								if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 1){
									if(--index < 0)	index = MPG_AXES_SETTINGS - 1;
								}else{
									if(--index < 0)	index = MPG_SPINDLE_SETTINGS - 1;
								}
							}
						}else if(get_button_state(BUTTON_ENTER_CH) == GPIO_PIN_RESET){
							delete_menu();
							menu_controller.level = 3;
							menu_controller.level_indexes[2] &= 0x00FF;
							menu_controller.level_indexes[2] |= menu_controller.level_indexes[2] << 8;
							draw_menu();
							break;
						}

						if(index != (menu_controller.level_indexes[2] & 0x00FF)){
							if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 1){
								mpg_axes_unselect_setting((menu_controller.level_indexes[2] & 0x00FF));
							}else{
								mpg_spindle_unselect_setting((menu_controller.level_indexes[2] & 0x00FF));
							}

							menu_controller.level_indexes[2] &= 0x0000;
							menu_controller.level_indexes[2] |= index;

							if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 1){
								mpg_axes_select_setting((menu_controller.level_indexes[2] & 0x00FF));
							}else{
								mpg_spindle_select_setting((menu_controller.level_indexes[2] & 0x00FF));
							}

							buzzer_short_ring(2500, 100);
							//buzzer_short_ring(1250, (uint16_t)(menu_controller.button_delay * INPUT_DELAY_TIME));
						}
					}
				}
				break;
			case 3:
				index = menu_controller.level_indexes[3] & 0x00FF;

				if(touch_exit_submenu(3)){
					delete_menu();
					menu_controller.level = 2;
					menu_controller.level_indexes[3] = 0;
					menu_controller.level_indexes[2] &= 0x00FF;
					draw_menu();
					break;
				}else if(get_button_state(BUTTON_BACK_CH) == GPIO_PIN_RESET){
					delete_menu();
					menu_controller.level = 2;
					menu_controller.level_indexes[3] = 0;
					menu_controller.level_indexes[2] &= 0x00FF;
					draw_menu();
					break;
				}

				if((menu_controller.level_indexes[0] & 0x00FF) == 2){
					if((menu_controller.level_indexes[1] & 0x00FF) == 1){
						if(get_button_state(BUTTON_DOWN_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_DOWN_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_DOWN_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_DOWN_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_DOWN_CH] = 0;
								}

								if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 0){
									if(++index >= X_AXIS_DIST_SETTINGS)	index = 0;
								}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 1){
									if(++index >= YZ_AXIS_DIST_SETTINGS) index = 0;
								}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 2){
									if(++index >= JOG_FEED_SETTINGS) index = 0;
								}
							}
						}else if(get_button_state(BUTTON_UP_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_UP_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_UP_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_UP_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_UP_CH] = 0;
								}

								if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 0){
									if(--index < 0)	index = X_AXIS_DIST_SETTINGS - 1;
								}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 1){
									if(--index < 0)	index = YZ_AXIS_DIST_SETTINGS - 1;
								}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 2){
									if(--index < 0) index = JOG_FEED_SETTINGS - 1;
								}
							}
						}else if(get_button_state(BUTTON_RIGHT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_RIGHT_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] = 0;
								}

								if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 0){	//TODO
									switch(menu_controller.level_indexes[3] & 0x00FF){
										case 0:	//Slow jogging x axis distance
											mpg_settings.x_axis_multiplicity[0] += X_AXIS_MULTIPLICITY_DIFF[0];
											if(mpg_settings.x_axis_multiplicity[0] > (X_AXIS_MULTIPLICITY_DEFAULT_0 + X_AXIS_MULTIPLICITY_DIFF[0] * 9))	mpg_settings.x_axis_multiplicity[0] = (X_AXIS_MULTIPLICITY_DEFAULT_0 + X_AXIS_MULTIPLICITY_DIFF[0] * 9);
											break;
										case 1:	//Medium jogging x axis distance
											mpg_settings.x_axis_multiplicity[1] += X_AXIS_MULTIPLICITY_DIFF[1];
											if(mpg_settings.x_axis_multiplicity[1] > (X_AXIS_MULTIPLICITY_DEFAULT_1 + X_AXIS_MULTIPLICITY_DIFF[1] * 9))	mpg_settings.x_axis_multiplicity[1] = (X_AXIS_MULTIPLICITY_DEFAULT_1 + X_AXIS_MULTIPLICITY_DIFF[1] * 9);
											break;
										case 2:	//Fast jogging x axis distance
											mpg_settings.x_axis_multiplicity[2] += X_AXIS_MULTIPLICITY_DIFF[2];
											if(mpg_settings.x_axis_multiplicity[2] > (X_AXIS_MULTIPLICITY_DEFAULT_2 + X_AXIS_MULTIPLICITY_DIFF[2] * 5))	mpg_settings.x_axis_multiplicity[2] = (X_AXIS_MULTIPLICITY_DEFAULT_2 + X_AXIS_MULTIPLICITY_DIFF[2] * 5);
											break;
										default:
											break;
									}
								}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 1){ //TODO
									switch(menu_controller.level_indexes[3] & 0x00FF){
										case 0:	//Slow jogging y/z axis distance
											mpg_settings.yz_axis_multiplicity[0] += YZ_AXIS_MULTIPLICITY_DIFF[0];
											if(mpg_settings.yz_axis_multiplicity[0] > (YZ_AXIS_MULTIPLICITY_DEFAULT_0 + YZ_AXIS_MULTIPLICITY_DIFF[0] * 9))	mpg_settings.yz_axis_multiplicity[0] = (YZ_AXIS_MULTIPLICITY_DEFAULT_0 + YZ_AXIS_MULTIPLICITY_DIFF[0] * 9);
											break;
										case 1:	//Medium jogging y/z axis distance
											mpg_settings.yz_axis_multiplicity[1] += YZ_AXIS_MULTIPLICITY_DIFF[1];
											if(mpg_settings.yz_axis_multiplicity[1] > (YZ_AXIS_MULTIPLICITY_DEFAULT_1 + YZ_AXIS_MULTIPLICITY_DIFF[1] * 9))	mpg_settings.yz_axis_multiplicity[1] = (YZ_AXIS_MULTIPLICITY_DEFAULT_1 + YZ_AXIS_MULTIPLICITY_DIFF[1] * 9);
											break;
										case 2:	//Fast jogging y/z axis distance
											mpg_settings.yz_axis_multiplicity[2] += YZ_AXIS_MULTIPLICITY_DIFF[2];
											if(mpg_settings.yz_axis_multiplicity[2] > (YZ_AXIS_MULTIPLICITY_DEFAULT_2 + YZ_AXIS_MULTIPLICITY_DIFF[2] * 5))	mpg_settings.yz_axis_multiplicity[2] = (YZ_AXIS_MULTIPLICITY_DEFAULT_2 + YZ_AXIS_MULTIPLICITY_DIFF[2] * 5);
											break;
										default:
											break;
									}
								}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 2){	//TODO
									switch(menu_controller.level_indexes[3] & 0x00FF){
										case 0:	//Slow jogging feed rate
											mpg_settings.jog_speeds[0] += JOG_SPEED_DIFF[0];
											if(mpg_settings.jog_speeds[0] > (JOG_SPEED_DEFAULT_SLOW + JOG_SPEED_DIFF[0] * 9))	mpg_settings.jog_speeds[0] = (JOG_SPEED_DEFAULT_SLOW + JOG_SPEED_DIFF[0] * 25);
											break;
										case 1:	//Medium jogging feed rate
											mpg_settings.jog_speeds[1] += JOG_SPEED_DIFF[1];
											if(mpg_settings.jog_speeds[1] > (JOG_SPEED_DEFAULT_MEDIUM + JOG_SPEED_DIFF[1] * 5))	mpg_settings.jog_speeds[1] = (JOG_SPEED_DEFAULT_MEDIUM + JOG_SPEED_DIFF[1] * 5);
											break;
										case 2:	//Fast jogging feed rate
											mpg_settings.jog_speeds[2] += JOG_SPEED_DIFF[2];
											if(mpg_settings.jog_speeds[2] > (JOG_SPEED_DEFAULT_FAST + JOG_SPEED_DIFF[2] * 5))	mpg_settings.jog_speeds[2] = (JOG_SPEED_DEFAULT_FAST + JOG_SPEED_DIFF[2] * 5);
											break;
										default:
											break;
									}
								}
							}
						}else if(get_button_state(BUTTON_LEFT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_LEFT_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_LEFT_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_LEFT_CH] = 0;
								}

								if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 0){
									switch(menu_controller.level_indexes[3] & 0x00FF){
										case 0:	//Slow jogging x axis distance
											mpg_settings.x_axis_multiplicity[0] -= X_AXIS_MULTIPLICITY_DIFF[0];
											if(mpg_settings.x_axis_multiplicity[0] < X_AXIS_MULTIPLICITY_DEFAULT_0)	mpg_settings.x_axis_multiplicity[0] = X_AXIS_MULTIPLICITY_DEFAULT_0;
											break;
										case 1:	//Medium jogging x axis distance
											mpg_settings.x_axis_multiplicity[1] -= X_AXIS_MULTIPLICITY_DIFF[1];
											if(mpg_settings.x_axis_multiplicity[1] < X_AXIS_MULTIPLICITY_DEFAULT_1)	mpg_settings.x_axis_multiplicity[1] = X_AXIS_MULTIPLICITY_DEFAULT_1;
											break;
										case 2:	//Fast jogging x axis distance
											mpg_settings.x_axis_multiplicity[2] -= X_AXIS_MULTIPLICITY_DIFF[2];
											if(mpg_settings.x_axis_multiplicity[2] < X_AXIS_MULTIPLICITY_DEFAULT_2)	mpg_settings.x_axis_multiplicity[2] = X_AXIS_MULTIPLICITY_DEFAULT_2;
											break;
										default:
											break;
									}
								}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 1){
									switch(menu_controller.level_indexes[3] & 0x00FF){
										case 0:	//Slow jogging y/z axis distance
											mpg_settings.yz_axis_multiplicity[0] -= YZ_AXIS_MULTIPLICITY_DIFF[0];
											if(mpg_settings.yz_axis_multiplicity[0] < YZ_AXIS_MULTIPLICITY_DEFAULT_0)	mpg_settings.yz_axis_multiplicity[0] = YZ_AXIS_MULTIPLICITY_DEFAULT_0;
											break;
										case 1:	//Medium jogging y/z axis distance
											mpg_settings.yz_axis_multiplicity[1] -= YZ_AXIS_MULTIPLICITY_DIFF[1];
											if(mpg_settings.yz_axis_multiplicity[1] < YZ_AXIS_MULTIPLICITY_DEFAULT_1)	mpg_settings.yz_axis_multiplicity[1] = YZ_AXIS_MULTIPLICITY_DEFAULT_1;
											break;
										case 2:	//Fast jogging y/z axis distance
											mpg_settings.yz_axis_multiplicity[2] -= YZ_AXIS_MULTIPLICITY_DIFF[2];
											if(mpg_settings.yz_axis_multiplicity[2] < YZ_AXIS_MULTIPLICITY_DEFAULT_2)	mpg_settings.yz_axis_multiplicity[2] = YZ_AXIS_MULTIPLICITY_DEFAULT_2;
											break;
										default:
											break;
									}
								}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 2){
									switch(menu_controller.level_indexes[3] & 0x00FF){
										case 0:	//Slow jogging feed rate
											mpg_settings.jog_speeds[0] -= JOG_SPEED_DIFF[0];
											if(mpg_settings.jog_speeds[0] < JOG_SPEED_DEFAULT_SLOW)	mpg_settings.jog_speeds[0] = JOG_SPEED_DEFAULT_SLOW;
											break;
										case 1:	//Medium jogging feed rate
											mpg_settings.jog_speeds[1] -= JOG_SPEED_DIFF[1];
											if(mpg_settings.jog_speeds[1] < JOG_SPEED_DEFAULT_MEDIUM)	mpg_settings.jog_speeds[1] = JOG_SPEED_DEFAULT_MEDIUM;
											break;
										case 2:	//Fast jogging feed rate
											mpg_settings.jog_speeds[2] -= JOG_SPEED_DIFF[2];
											if(mpg_settings.jog_speeds[2] < JOG_SPEED_DEFAULT_FAST)	mpg_settings.jog_speeds[2] = JOG_SPEED_DEFAULT_FAST;
											break;
										default:
											break;
									}
								}
							}
						}

						mpg_axes_update_settings();

						if(index != (menu_controller.level_indexes[3] & 0x00FF)){
							if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 0){
								unselect_x_axis_dist_setting((menu_controller.level_indexes[3] & 0x00FF));
							}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 1){
								unselect_yz_axis_dist_setting((menu_controller.level_indexes[3] & 0x00FF));
							}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 2){
								unselect_jog_feed_setting((menu_controller.level_indexes[3] & 0x00FF));
							}

							menu_controller.level_indexes[3] &= 0x0000;
							menu_controller.level_indexes[3] |= index;

							if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 0){
								select_x_axis_dist_setting((menu_controller.level_indexes[3] & 0x00FF));
							}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 1){
								select_yz_axis_dist_setting((menu_controller.level_indexes[3] & 0x00FF));
							}else if(((menu_controller.level_indexes[2] & 0xFF00) >> 8) == 2){
								select_jog_feed_setting((menu_controller.level_indexes[3] & 0x00FF));
							}

							buzzer_short_ring(2500, 100);
							//buzzer_short_ring(1250, (uint16_t)(menu_controller.button_delay * INPUT_DELAY_TIME));
						}
					}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 2){		//TODO: MPG spindle settings
						if(get_button_state(BUTTON_RIGHT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_RIGHT_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] = 0;
								}

								if((menu_controller.level_indexes[2] & 0x00FF) == 0){
									mpg_settings.spindle_speed += SPINDLE_SPEED_DIFF;
									if(mpg_settings.spindle_speed > SPINDLE_SPEED_MAX)	mpg_settings.spindle_speed = SPINDLE_SPEED_MAX;
								}
							}
							mpg_spindle_update_setting();
						}else if(get_button_state(BUTTON_LEFT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_LEFT_CH] >= menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == 1){
								if(menu_controller.button_delay_cnts[BUTTON_LEFT_CH]++ != 1){
									menu_controller.button_delay_cnts[BUTTON_LEFT_CH] = 0;
								}

								if((menu_controller.level_indexes[2] & 0x00FF) == 0){
									mpg_settings.spindle_speed -= SPINDLE_SPEED_DIFF;
									if(mpg_settings.spindle_speed < SPINDLE_SPEED_MIN)	mpg_settings.spindle_speed = SPINDLE_SPEED_MIN;
								}
							}
							mpg_spindle_update_setting();
						}
					}
				}
				break;
			default:
				break;
		}
	}
}





//// Menu drawing functions ////
/**
  * @brief Menu draw selector
  */
void draw_menu(void){
	switch(menu_controller.level){
		case 0:
			for(uint8_t i = 0; i < SUBMENUS; i++){
				if(i == menu_controller.level_indexes[0] && 0x00FF){
					select_submenu(i);
				}else{
					unselect_submenu(i);
				}
			}
			break;
		case 1:
			switch((menu_controller.level_indexes[0] & 0xFF00 >> 8)){
				case 0: 	//TODO: Auto mode sub-menu draw
					R61529_WriteString(10, 10, "Auto mode engaged", Font_11x18, PURPLE, BLACK);

					// Position section
					R61529_WriteString(10, 80,  "Position(mm)", Font_11x18, YELLOW, BLACK);
					R61529_WriteString(10, 100, "X:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(10, 130, "Y:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(10, 160, "Z:", Font_16x26, WHITE, BLACK);

					// Offset section
					R61529_WriteString(200, 80,  "Offset(mm)", Font_11x18, WHITE, BLACK);
					R61529_WriteString(200, 100, "X:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(200, 130, "Y:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(200, 160, "Z:", Font_16x26, WHITE, BLACK);

					// Coolant status section
					R61529_WriteString(390, 80,  "Coolant", Font_11x18, CYAN, BLACK);
					R61529_WriteString(390, 100, "MIST", Font_11x18, RED, GRAY);
					R61529_WriteString(390, 130, "FLOOD", Font_11x18, RED, GRAY);
					R61529_WriteString(390, 160, "SPINDLE", Font_11x18, RED, GRAY);

					// RPM
					R61529_WriteString(10, 250,  "RPM:", Font_16x26, WHITE, BLACK);

					// Machine status
					R61529_WriteString(10, 290,  "Status:", Font_11x18, WHITE, BLACK);

					// Show data on screen
					auto_mode_update_screen();
					break;
				case 1: 	//TODO: Manual mode sub-menu draw
					R61529_WriteString(10, 10, "Manual mode engaged", Font_11x18, PURPLE, BLACK);

					// Position section
					R61529_WriteString(10, 80,  "Position(mm)", Font_11x18, YELLOW, BLACK);
					R61529_WriteString(10, 100, "X:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(10, 130, "Y:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(10, 160, "Z:", Font_16x26, WHITE, BLACK);

					// Axes movement steps section
					R61529_WriteString(140, 100 + (26 - 10), "x", Font_7x10, WHITE, BLACK);
					R61529_WriteString(140, 130 + (26 - 10), "x", Font_7x10, WHITE, BLACK);
					R61529_WriteString(140, 160 + (26 - 10), "x", Font_7x10, WHITE, BLACK);

					// Offset section
					R61529_WriteString(250, 80,  "Offset(mm)", Font_11x18, WHITE, BLACK);
					R61529_WriteString(250, 100, "X:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(250, 130, "Y:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(250, 160, "Z:", Font_16x26, WHITE, BLACK);

					// Coolant status section
					R61529_WriteString(390, 80,  "Coolant", Font_11x18, CYAN, BLACK);
					R61529_WriteString(390, 100, "MIST", Font_11x18, RED, GRAY);
					R61529_WriteString(390, 130, "FLOOD", Font_11x18, RED, GRAY);
					R61529_WriteString(390, 160, "SPINDLE", Font_11x18, RED, GRAY);

					// RPM
					R61529_WriteString(10, 250,  "RPM:", Font_16x26, WHITE, BLACK);

					// Machine status
					R61529_WriteString(10, 290,  "Status:", Font_11x18, WHITE, BLACK);

					// Jogging mode
					R61529_WriteString(350, 270,  "Jog:", Font_11x18, WHITE, BLACK);

					// Show data on screen
					manual_mode_update_screen();
					break;
				case 2:		// Settings
					R61529_WriteString(SETTING_BLOCK_X_MARGIN, 10, "Settings", Font_16x26, YELLOW, BLACK);
					for(uint8_t i = 0; i < SETTINGS_SUBMENUS; i++){
						if(i == (menu_controller.level_indexes[1] & 0x00FF)){
							select_setting(i);
						}else{
							unselect_setting(i);
						}
					}
					break;
				default:
					break;
			}
			R61529_WriteString(410, 5, "Back", Font_16x26, WHITE, RED);
			break;
		case 2:
			switch((menu_controller.level_indexes[0] & 0xFF00 >> 8)){
				case 0:		// MPG Auto mode
					break;
				case 1:		// MPG Manual mode
					break;
				case 2:		// Settings
					R61529_WriteString(SETTING_BLOCK_X_MARGIN, 10, settings_options[(menu_controller.level_indexes[1] & 0xFF00) >> 8], Font_16x26, CYAN, BLACK);
					if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 0){ // Screen brightness
						R61529_DrawRect(WHITE, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_Y);
						R61529_WriteString(SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y + SCREEN_BRIGHTNESS_SETTING_SLIDER_HEIGHT + 10, "Brightness - ", Font_11x18, GREEN, BLACK);
						screen_brightness_setting_update_screen();
					}else if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 1){ // MPG axes settings
						for(uint8_t i = 0; i < MPG_AXES_SETTINGS; i++){
							if(i == (menu_controller.level_indexes[2] & 0x00FF)){
								mpg_axes_select_setting(i);
							}else{
								mpg_axes_unselect_setting(i);
							}
						}
					}else if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 2){ // MPG spindle settings
						for(uint8_t i = 0; i < MPG_SPINDLE_SETTINGS; i++){
							if(i == (menu_controller.level_indexes[2] & 0x00FF)){
								mpg_spindle_select_setting(i);
							}else{
								mpg_spindle_unselect_setting(i);
							}
						}
					}
					break;
				default:
					break;
			}
			R61529_WriteString(410, 5, "Back", Font_16x26, WHITE, RED);
			break;
		case 3:
			if((menu_controller.level_indexes[0] & 0xFF00 >> 8) == 2){
				if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 1){
					R61529_WriteString(MPG_AXES_SETTING_BLOCK_X_MARGIN, 10, mpg_axes_settings[(menu_controller.level_indexes[2] & 0xFF00) >> 8], Font_16x26, CYAN, BLACK);
					switch((menu_controller.level_indexes[2] & 0xFF00 >> 8)){
						case 0: // X axis jogging distances
							for(uint8_t i = 0; i < X_AXIS_DIST_SETTINGS; i++){
								if(i == (menu_controller.level_indexes[3] & 0x00FF)){
									select_x_axis_dist_setting(i);
								}else{
									unselect_x_axis_dist_setting(i);
								}
							}
							break;
						case 1:	// Y/Z axes jogging distances
							for(uint8_t i = 0; i < YZ_AXIS_DIST_SETTINGS; i++){
								if(i == (menu_controller.level_indexes[3] & 0x00FF)){
									select_yz_axis_dist_setting(i);
								}else{
									unselect_yz_axis_dist_setting(i);
								}
							}
							break;
						case 2:	// Jogging feed rates
							for(uint8_t i = 0; i < JOG_FEED_SETTINGS; i++){
								if(i == (menu_controller.level_indexes[3] & 0x00FF)){
									select_jog_feed_setting(i);
								}else{
									unselect_jog_feed_setting(i);
								}
							}
							break;
						default:
							break;
					}
				}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 2){
					R61529_WriteString(MPG_SPINDLE_SETTING_BLOCK_X_MARGIN, 10, mpg_spindle_settings[(menu_controller.level_indexes[2] & 0xFF00) >> 8], Font_16x26, CYAN, BLACK);
					switch((menu_controller.level_indexes[2] & 0xFF00) >> 8){
						case 0:		// Jogging spindle speed
							R61529_DrawRect(WHITE, SPINDLE_SPEED_SETTING_SLIDER_BASE_X, SPINDLE_SPEED_SETTING_SLIDER_BASE_Y, SPINDLE_SPEED_SETTING_SLIDER_END_X, SPINDLE_SPEED_SETTING_SLIDER_END_Y);
							R61529_WriteString(SPINDLE_SPEED_SETTING_SLIDER_BASE_X, SPINDLE_SPEED_SETTING_SLIDER_BASE_Y + SPINDLE_SPEED_SETTING_SLIDER_HEIGHT + 10, "Spindle speed: ", Font_11x18, GREEN, BLACK);
							break;
						default:
							break;
					}
					mpg_spindle_update_setting();
				}
			}
			R61529_WriteString(410, 5, "Back", Font_16x26, WHITE, RED);
			break;
		default:
			break;
	}
}

/**
  * @brief Menu delete selector
  */
void delete_menu(void){
	switch(menu_controller.level){
		case 0:
			for(uint8_t i = 0; i < SUBMENUS; i++){
				delete_submenu(i);
			}
			break;
		case 1:
			R61529_WriteString(410, 5, "Back", Font_16x26, BLACK, BLACK);

			grbl_rpm = -1;
			mpg_state = -1;
			grbl_state = -1;
			pendant_status = -1;
			pendant_jog_mode = -1;
			for(uint8_t i = 0; i < 3; i++){
				grbl_offset[i] = -1;
				grbl_position[i] = -1;
				grbl_coolant_status[i] = -1;
			}

			switch((menu_controller.level_indexes[0] & 0xFF00 >> 8)){
				case 0: 	//TODO: Auto mode sub-menu screen deletion
					R61529_FillRect(BLACK, 10, 10, 200, 30);
					R61529_FillRect(BLACK, 10, 80, 200, 190);
					R61529_FillRect(BLACK, 200, 80, 400, 190);
					R61529_FillRect(BLACK, 390, 80, 470, 180);
					R61529_FillRect(BLACK, 10, 250, 165, 280);
					R61529_FillRect(BLACK, 10, 290, 200, 310);
					break;
				case 1: 	//TODO: Manual mode sub-menu screen deletion
					disconnect_pendant();
					while(get_pendant_status() == PENDANT_DISCONNECTING){
						disconnect_pendant();
					}
					if(grbl_data.mpgMode){
						disable_mpg();
					}

					R61529_FillRect(BLACK, 10, 10, 450, 80);
					R61529_FillRect(BLACK, 10, 80, 200, 190);
					R61529_FillRect(BLACK, 200, 80, 400, 190);
					R61529_FillRect(BLACK, 390, 80, 470, 180);
					R61529_FillRect(BLACK, 10, 250, 165, 280);
					R61529_FillRect(BLACK, 10, 290, 200, 310);
					R61529_FillRect(BLACK, 140, 100, 190, 170);
					R61529_FillRect(BLACK, 350, 270, 470, 290);
					break;
				case 2:		//Settings
					R61529_WriteString(SETTING_BLOCK_X_MARGIN, 10, "Settings", Font_16x26, BLACK, BLACK);
					for(uint8_t i = 0; i < SETTINGS_SUBMENUS; i++){
						delete_setting(i);
					}
					break;
				default:
					break;
			}
			break;
		case 2:
			R61529_WriteString(410, 5, "Back", Font_16x26, BLACK, BLACK);
			switch((menu_controller.level_indexes[0] & 0xFF00 >> 8)){
				case 0:		//MPG Auto mode
					break;
				case 1:		//MPG Manual mode
					break;
				case 2:		//Settings
					R61529_WriteString(SETTING_BLOCK_X_MARGIN, 10, settings_options[(menu_controller.level_indexes[1] & 0xFF00) >> 8], Font_16x26, BLACK, BLACK);
					if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 0){				//Screen brightness
						R61529_FillRect(BLACK, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X, 5, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_Y + 30);
					}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 1){		//MPG axes settings
						for(uint8_t i = 0; i < MPG_AXES_SETTINGS; i++){
							mpg_axes_delete_setting(i);
						}
					}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 2){		//MPG spindle settings
						for(uint8_t i = 0; i < MPG_SPINDLE_SETTINGS; i++){
							mpg_spindle_delete_setting(i);
						}
					}
					break;
				default:
					break;
			}
			break;
		case 3:
			R61529_WriteString(410, 5, "Back", Font_16x26, BLACK, BLACK);
			if((menu_controller.level_indexes[0] & 0xFF00 >> 8) == 2){
				if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 1){
					R61529_WriteString(MPG_AXES_SETTING_BLOCK_X_MARGIN, 10, mpg_axes_settings[(menu_controller.level_indexes[2] & 0xFF00) >> 8], Font_16x26, BLACK, BLACK);
					switch((menu_controller.level_indexes[2] & 0xFF00 >> 8)){
						case 0:		// X axis jogging distances
							for(uint8_t i = 0; i < X_AXIS_DIST_SETTINGS; i++){
								delete_x_axis_dist_setting(i);
							}
							break;
						case 1:		// Y/Z axes jogging distances
							for(uint8_t i = 0; i < YZ_AXIS_DIST_SETTINGS; i++){
								delete_yz_axis_dist_setting(i);
							}
							break;
						case 2:		// Jogging feed rates
							for(uint8_t i = 0; i < JOG_FEED_SETTINGS; i++){
								delete_jog_feed_setting(i);
							}
							break;
						default:
							break;
					}
				}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 2){
					R61529_WriteString(MPG_SPINDLE_SETTING_BLOCK_X_MARGIN, 10, mpg_spindle_settings[(menu_controller.level_indexes[2] & 0xFF00) >> 8], Font_16x26, BLACK, BLACK);
					switch((menu_controller.level_indexes[2] & 0xFF00 >> 8)){
						case 0:		// Jogging spindle speeds
							R61529_FillRect(BLACK, SPINDLE_SPEED_SETTING_SLIDER_BASE_X, SPINDLE_SPEED_SETTING_SLIDER_BASE_Y, SPINDLE_SPEED_SETTING_SLIDER_END_X, SPINDLE_SPEED_SETTING_SLIDER_END_Y);
							R61529_WriteString(SPINDLE_SPEED_SETTING_SLIDER_BASE_X, SPINDLE_SPEED_SETTING_SLIDER_BASE_Y + SPINDLE_SPEED_SETTING_SLIDER_HEIGHT + 10, "Spindle speed:      ", Font_11x18, BLACK, BLACK);
							break;
						default:
							break;
					}
				}
			}
			break;
		default:
			return;
	}
}

/**
  * @brief Draw loading screen
  */
void draw_loading_screen(void){
	//Initialize button channels
	init_button_inputs();

	//Draw parts of loading screen
	R61529_FillScreen(BLACK);
	R61529_WriteString(170, 145, "TITAN CNC", Font_16x26, BLACK, CYAN);
	delay_ms(1500);

	//Initialize menu controller
	menu_controller.level = 0;
	menu_controller.buzzer_volume = 50;
	menu_controller.brightness_value = 50;
	menu_controller.touch_point_cnt = 0;
	for(uint8_t i = 0; i < FT_REG_NUMTOUCHES; i++){
		menu_controller.touch_points[i][0] = -1;	//NA
		menu_controller.touch_points[i][1] = -1;	//NA
	}
	for(uint8_t i = 0; i < LEVELS; i++)
		menu_controller.level_indexes[i] = 0;

	//Draw initial menu (main menu)
	R61529_FillScreen(BLACK);
	draw_menu();
}





//// Sub-menu drawing functions ////
/**
  * @brief Delete a given sub-menu
  */
void delete_submenu(uint8_t index){
	uint16_t blob_base_x = GET_SUBMENU_BLOCK_BASE_X(index);
	uint16_t blob_base_y = GET_SUBMENU_BLOCK_BASE_Y;

	if(index > (SUBMENUS - 1)){
		return;
	}

	R61529_FillRect(BLACK, blob_base_x - 1, blob_base_y - 1, blob_base_x + SUBMENU_BLOCK_WIDTH + 1, blob_base_y + SUBMENU_BLOCK_HEIGHT + 1);
}

/**
  * @brief Select a given sub-menu
  */
void select_submenu(uint8_t index){
	uint16_t blob_base_x = GET_SUBMENU_BLOCK_BASE_X(index);
	uint16_t blob_base_y = GET_SUBMENU_BLOCK_BASE_Y;
	uint16_t submenu_text_base_x = GET_SUBMENU_BLOCK_TEXT_BASE_X(index);
	uint16_t submenu_text_base_y = GET_SUBMENU_BLOCK_TEXT_BASE_Y;

	if(index > (SUBMENUS - 1)){
		return;
	}

	R61529_FillRect(CYAN, blob_base_x, blob_base_y, blob_base_x + SUBMENU_BLOCK_WIDTH, blob_base_y + SUBMENU_BLOCK_HEIGHT);
	R61529_WriteString(submenu_text_base_x, submenu_text_base_y, main_menu_options[index], Font_11x18, BLACK, CYAN);
}

/**
  * @brief Un-select a given sub-menu
  */
void unselect_submenu(uint8_t index){
	uint16_t blob_base_x = GET_SUBMENU_BLOCK_BASE_X(index);
	uint16_t blob_base_y = GET_SUBMENU_BLOCK_BASE_Y;
	uint16_t submenu_text_base_x = GET_SUBMENU_BLOCK_TEXT_BASE_X(index);
	uint16_t submenu_text_base_y = GET_SUBMENU_BLOCK_TEXT_BASE_Y;

	if(index > (SUBMENUS - 1)){
		return;
	}

	R61529_FillRect(ORANGE, blob_base_x, blob_base_y, blob_base_x + SUBMENU_BLOCK_WIDTH, blob_base_y + SUBMENU_BLOCK_HEIGHT);
	R61529_WriteString(submenu_text_base_x, submenu_text_base_y, main_menu_options[index], Font_11x18, WHITE, ORANGE);
}





//// Setting drawing functions ////
/**
  * @brief Delete a given setting in Settings sub-menu
  */
void delete_setting(uint8_t index){
	uint16_t setting_base_x = SETTING_BLOCK_X_MARGIN;
	uint16_t setting_base_y = GET_SETTING_BLOCK_BASE_Y(index);

	if(index > (SETTINGS_SUBMENUS - 1)){
		return;
	}

	R61529_FillRect(BLACK, setting_base_x, setting_base_y, setting_base_x + SETTING_BLOCK_WIDTH, setting_base_y + SETTING_BLOCK_HEIGHT);
}

/**
  * @brief Select a given setting in Settings sub-menu
  */
void select_setting(uint8_t index){
	uint16_t setting_base_x = GET_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_SETTING_BLOCK_TEXT_BASE_Y(index);

	if(index > (SETTINGS_SUBMENUS - 1)){
		return;
	}

	R61529_FillRect(GREEN, setting_base_x, setting_base_y, setting_base_x + SETTING_BLOCK_WIDTH, setting_base_y + SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, settings_options[index], Font_11x18, BROWN, GREEN);
}

/**
  * @brief Un-select a given setting in Settings sub-menu
  */
void unselect_setting(uint8_t index){
	uint16_t setting_base_x = GET_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_SETTING_BLOCK_TEXT_BASE_Y(index);

	if(index > (SETTINGS_SUBMENUS - 1)){
		return;
	}

	R61529_FillRect(GRAY, setting_base_x, setting_base_y, setting_base_x + SETTING_BLOCK_WIDTH, setting_base_y + SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, settings_options[index], Font_11x18, WHITE, GRAY);
}





// MPG axes setting drawing functions
/**
  * @brief Select a given mpg axis setting in "MPG axes settings" sub-menu
  */
void mpg_axes_delete_setting(uint8_t index){
	uint16_t setting_base_x = GET_MPG_AXES_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_MPG_AXES_SETTING_BLOCK_BASE_Y(index);

	if(index > (MPG_AXES_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(BLACK, setting_base_x, setting_base_y, setting_base_x + MPG_AXES_SETTING_BLOCK_WIDTH, setting_base_y + MPG_AXES_SETTING_BLOCK_HEIGHT);
}

/**
  * @brief Select a given mpg axis setting in "MPG axes settings" sub-menu
  */
void mpg_axes_select_setting(uint8_t index){
	uint16_t setting_base_x = GET_MPG_AXES_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_MPG_AXES_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_MPG_AXES_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_MPG_AXES_SETTING_BLOCK_TEXT_BASE_Y(index);

	if(index > (MPG_AXES_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(GREEN, setting_base_x, setting_base_y, setting_base_x + MPG_AXES_SETTING_BLOCK_WIDTH, setting_base_y + MPG_AXES_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, mpg_axes_settings[index], Font_11x18, BROWN, GREEN);
}

/**
  * @brief Un-select a given mpg axis setting in "MPG axes settings" sub-menu
  */
void mpg_axes_unselect_setting(uint8_t index){
	uint16_t setting_base_x = GET_MPG_AXES_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_MPG_AXES_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_MPG_AXES_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_MPG_AXES_SETTING_BLOCK_TEXT_BASE_Y(index);

	if(index > (MPG_AXES_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(GRAY, setting_base_x, setting_base_y, setting_base_x + MPG_AXES_SETTING_BLOCK_WIDTH, setting_base_y + MPG_AXES_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, mpg_axes_settings[index], Font_11x18, WHITE, GRAY);
}





// MPG spindle setting drawing functions
/**
  * @brief Select a given mpg spindle setting in "MPG spindle settings" sub-menu
  */
void mpg_spindle_delete_setting(uint8_t index){
	uint16_t setting_base_x = GET_MPG_SPINDLE_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_MPG_SPINDLE_SETTING_BLOCK_BASE_Y(index);

	if(index > (MPG_SPINDLE_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(BLACK, setting_base_x, setting_base_y, setting_base_x + MPG_SPINDLE_SETTING_BLOCK_WIDTH, setting_base_y + MPG_SPINDLE_SETTING_BLOCK_HEIGHT);
}

/**
  * @brief Select a given mpg spindle setting in "MPG spindle settings" sub-menu
  */
void mpg_spindle_select_setting(uint8_t index){
	uint16_t setting_base_x = GET_MPG_SPINDLE_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_MPG_SPINDLE_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_MPG_SPINDLE_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_MPG_SPINDLE_SETTING_BLOCK_TEXT_BASE_Y(index);

	if(index > (MPG_SPINDLE_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(GREEN, setting_base_x, setting_base_y, setting_base_x + MPG_SPINDLE_SETTING_BLOCK_WIDTH, setting_base_y + MPG_SPINDLE_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, mpg_spindle_settings[index], Font_11x18, BROWN, GREEN);
}

/**
  * @brief Un-select a given mpg spindle setting in "MPG spindle settings" sub-menu
  */
void mpg_spindle_unselect_setting(uint8_t index){
	uint16_t setting_base_x = GET_MPG_SPINDLE_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_MPG_SPINDLE_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_MPG_SPINDLE_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_MPG_SPINDLE_SETTING_BLOCK_TEXT_BASE_Y(index);

	if(index > (MPG_SPINDLE_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(GRAY, setting_base_x, setting_base_y, setting_base_x + MPG_SPINDLE_SETTING_BLOCK_WIDTH, setting_base_y + MPG_SPINDLE_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, mpg_spindle_settings[index], Font_11x18, WHITE, GRAY);
}





// Level 1 menu special functions
/**
  * @brief Update screen data for auto mode sub-menu
  */
void auto_mode_update_screen(void){
	// Display coordinates
	for(uint8_t i = 0; i < 3; i++){
		double coordinate = 0.000;

		if(i == 0)
			coordinate = grbl_data.position.x;
		else if(i == 1)
			coordinate = grbl_data.position.y;
		else
			coordinate = grbl_data.position.z;

		if(grbl_position[i] != coordinate){
			char pos_str[10] = {0}, prev_pos_str[10] = {0};
			uint8_t pos_str_len, prev_pos_str_len;

			sprintf(pos_str, "%4.3f", coordinate);
			sprintf(prev_pos_str, "%4.3f", grbl_position[i]);
			pos_str_len = strlen(pos_str);
			prev_pos_str_len = strlen(prev_pos_str);
			grbl_position[i] = coordinate;

			if(pos_str_len < prev_pos_str_len)
				R61529_FillRect(BLACK, 50, 100 + ((26 - 18) / 2) + (30 * i), 50 + (prev_pos_str_len * 11), 100 + ((26 - 18) / 2) + (30 * i) + 18);
			R61529_WriteString(50, 100 + ((26 - 18) / 2) + (30 * i), pos_str, Font_11x18, PURPLE, BLACK);
		}
	}

	// Display offset
	for(uint8_t i = 0; i < 3; i++){
		double _offset = 0.000;

		if(i == 0)
			_offset = grbl_data.offset.x;
		else if(i == 1)
			_offset = grbl_data.offset.y;
		else
			_offset = grbl_data.offset.z;

		if(grbl_offset[i] != _offset){
			char offset_str[10] = {0}, prev_offset_str[10] = {0};
			uint8_t offset_str_len, prev_offset_str_len;

			sprintf(offset_str, "%4.3f", _offset);
			sprintf(prev_offset_str, "%4.3f", grbl_offset[i]);
			offset_str_len = strlen(offset_str);
			prev_offset_str_len = strlen(prev_offset_str);
			grbl_offset[i] = _offset;

			if(offset_str_len < prev_offset_str_len)
				R61529_FillRect(BLACK, 240, 100 + ((26 - 18) / 2) + (30 * i), 240 + (prev_offset_str_len * 11), 100 + ((26 - 18) / 2) + (30 * i) + 18);
			R61529_WriteString(240, 100 + ((26 - 18) / 2) + (30 * i), offset_str, Font_11x18, PURPLE, BLACK);
		}
	}

	// Display coolant status
	if(grbl_data.coolant.mist != grbl_coolant_status[0]){
		uint16_t color = RED, bg_color = GRAY;
		grbl_coolant_status[0] = grbl_data.coolant.mist;

		if(grbl_data.coolant.mist){
			color = GREEN;
			bg_color = BLACK;
		}

		R61529_WriteString(390, 100, "MIST", Font_11x18, color, bg_color);
	}
	if(grbl_data.coolant.flood != grbl_coolant_status[1]){
		uint16_t color = RED, bg_color = GRAY;
		grbl_coolant_status[1] = grbl_data.coolant.flood;

		if(grbl_data.coolant.flood){
			color = GREEN;
			bg_color = BLACK;
		}

		R61529_WriteString(390, 130, "FLOOD", Font_11x18, color, bg_color);
	}
	if(grbl_data.coolant.trough_spindle != grbl_coolant_status[2]){
		uint16_t color = RED, bg_color = GRAY;
		grbl_coolant_status[2] = grbl_data.coolant.trough_spindle;

		if(grbl_data.coolant.trough_spindle){
			color = GREEN;
			bg_color = BLACK;
		}

		R61529_WriteString(390, 160, "SPINDLE", Font_11x18, color, bg_color);
	}

	// Display machine state
	if(grbl_state != grbl_data.grbl.state){
		uint16_t color = WHITE;
		char state_str[10] = {0};

		grbl_state = grbl_data.grbl.state;
		strcpy(state_str, grbl_data.grbl.state_text);
		if(grbl_state == Idle){
			color = CYAN;
		}else if(grbl_state == Run || grbl_state == Jog){
			color = GREEN;
		}else if(grbl_state == Hold || grbl_state == Tool){
			color = YELLOW;
		}else if(grbl_state == Alarm){
			color = RED;
		}else if(grbl_state == Check){
			color = GRAY;
		}else if(grbl_state == Door){
			color = ORANGE;
		}else if(grbl_state == Home){
			color = MAGENTA;
		}else if(grbl_state == Sleep){
			color = BLUE;
		}

		R61529_FillRect(BLACK, 90, 290, 170, 310);
		R61529_WriteString(90, 290, state_str, Font_11x18, color, BLACK);
	}

	// Display RPM
	if(grbl_rpm != grbl_data.spindle.rpm_programmed){
		uint16_t color = WHITE;
		char rpm_str[10] = {0}, prev_rpm_str[10] = {0};
		uint8_t rpm_str_len, prev_rpm_str_len;

		if(grbl_rpm != 0){
			color = ORANGE;
		}
		sprintf(prev_rpm_str, "%4.3f", grbl_rpm);
		sprintf(rpm_str, "%4.3f", grbl_data.spindle.rpm_programmed);
		rpm_str_len = strlen(rpm_str);
		prev_rpm_str_len = strlen(prev_rpm_str);
		grbl_rpm = grbl_data.spindle.rpm_programmed;

		if(rpm_str_len < prev_rpm_str_len)
			R61529_FillRect(BLACK, 80, 250, 80 + (prev_rpm_str_len * 11), 270);
		R61529_WriteString(80, 250 + ((26 - 18) / 2), rpm_str, Font_11x18, color, BLACK);
	}
}

/**
  * @brief Update screen data for manual mode sub-menu
  */
void manual_mode_update_screen(void){
	// Display coordinates
	for(uint8_t i = 0; i < 3; i++){
		double coordinate = 0.000;

		if(i == 0)
			coordinate = grbl_data.position.x;
		else if(i == 1)
			coordinate = grbl_data.position.y;
		else
			coordinate = grbl_data.position.z;

		if(grbl_position[i] != coordinate){
			char pos_str[10] = {0}, prev_pos_str[10] = {0};
			uint8_t pos_str_len, prev_pos_str_len;

			sprintf(pos_str, "%4.3f", coordinate);
			sprintf(prev_pos_str, "%4.3f", grbl_position[i]);
			pos_str_len = strlen(pos_str);
			prev_pos_str_len = strlen(prev_pos_str);
			grbl_position[i] = coordinate;

			if(pos_str_len < prev_pos_str_len)
				R61529_FillRect(BLACK, 50, 100 + ((26 - 18) / 2) + (30 * i), 50 + (prev_pos_str_len * 11), 100 + ((26 - 18) / 2) + (30 * i) + 18);
			R61529_WriteString(50, 100 + ((26 - 18) / 2) + (30 * i), pos_str, Font_11x18, PURPLE, BLACK);
		}
	}

	// Display axes movement steps
	for(uint8_t i = 0; i < 3; i++){
		char multiplier[5] = {0};

		if(i == 0){
			sprintf(multiplier, "%3.2f", mpg_settings.x_axis_multiplicity[pendant_data.jog_mode]);
		}else{
			sprintf(multiplier, "%3.2f", mpg_settings.yz_axis_multiplicity[pendant_data.jog_mode]);
		}

		R61529_WriteString(150, 100 + (i * 30) + (26 - 10), multiplier, Font_7x10, YELLOW, BLACK);
	}


	// Display offset
	for(uint8_t i = 0; i < 3; i++){
		double _offset = 0.000;

		if(i == 0)
			_offset = grbl_data.offset.x;
		else if(i == 1)
			_offset = grbl_data.offset.y;
		else
			_offset = grbl_data.offset.z;

		if(grbl_offset[i] != _offset){
			char offset_str[10] = {0}, prev_offset_str[10] = {0};
			uint8_t offset_str_len, prev_offset_str_len;

			sprintf(offset_str, "%4.3f", _offset);
			sprintf(prev_offset_str, "%4.3f", grbl_offset[i]);
			offset_str_len = strlen(offset_str);
			prev_offset_str_len = strlen(prev_offset_str);
			grbl_offset[i] = _offset;

			if(offset_str_len < prev_offset_str_len)
				R61529_FillRect(BLACK, 290, 100 + ((26 - 18) / 2) + (30 * i), 290 + (prev_offset_str_len * 11), 100 + ((26 - 18) / 2) + (30 * i) + 18);
			R61529_WriteString(290, 100 + ((26 - 18) / 2) + (30 * i), offset_str, Font_11x18, PURPLE, BLACK);
		}
	}

	// Display coolant status
	if(grbl_data.coolant.mist != grbl_coolant_status[0]){
		uint16_t color = RED, bg_color = GRAY;
		grbl_coolant_status[0] = grbl_data.coolant.mist;

		if(grbl_data.coolant.mist){
			color = GREEN;
			bg_color = BLACK;
		}

		R61529_WriteString(390, 100, "MIST", Font_11x18, color, bg_color);
	}
	if(grbl_data.coolant.flood != grbl_coolant_status[1]){
		uint16_t color = RED, bg_color = GRAY;
		grbl_coolant_status[1] = grbl_data.coolant.flood;

		if(grbl_data.coolant.flood){
			color = GREEN;
			bg_color = BLACK;
		}

		R61529_WriteString(390, 130, "FLOOD", Font_11x18, color, bg_color);
	}
	if(grbl_data.coolant.trough_spindle != grbl_coolant_status[2]){
		uint16_t color = RED, bg_color = GRAY;
		grbl_coolant_status[2] = grbl_data.coolant.trough_spindle;

		if(grbl_data.coolant.trough_spindle){
			color = GREEN;
			bg_color = BLACK;
		}

		R61529_WriteString(390, 160, "SPINDLE", Font_11x18, color, bg_color);
	}

	// Display machine state
	if(grbl_state != grbl_data.grbl.state){
		uint16_t color = WHITE;
		char state_str[10] = {0};

		grbl_state = grbl_data.grbl.state;
		strcpy(state_str, grbl_data.grbl.state_text);
		if(grbl_state == Idle){
			color = CYAN;
		}else if(grbl_state == Run || grbl_state == Jog){
			color = GREEN;
		}else if(grbl_state == Hold || grbl_state == Tool){
			color = YELLOW;
		}else if(grbl_state == Alarm){
			color = RED;
		}else if(grbl_state == Check){
			color = GRAY;
		}else if(grbl_state == Door){
			color = ORANGE;
		}else if(grbl_state == Home){
			color = MAGENTA;
		}else if(grbl_state == Sleep){
			color = BLUE;
		}

		R61529_FillRect(BLACK, 90, 290, 170, 310);
		R61529_WriteString(90, 290, state_str, Font_11x18, color, BLACK);
	}

	// Display MPG mode state
	if((mpg_state & 0x01) != (uint8_t)grbl_data.mpgMode){
		uint16_t color = RED;
		char mpg_state_str[15] = {0};

		mpg_state = (uint8_t)grbl_data.mpgMode;
		if(mpg_state){
			color = GREEN;
			strcpy(mpg_state_str, "MPG mode ON");
		}else{
			strcpy(mpg_state_str, "MPG mode OFF");
		}

		R61529_FillRect(BLACK, 10, 40, 150, 60);
		R61529_WriteString(10, 40, mpg_state_str, Font_11x18, color, BLACK);
	}

	// Display MPG pendant status
	if(pendant_status == -1 || (pendant_status != get_pendant_status() && \
	   get_pendant_status() != PENDANT_AWAITING_DATA && \
	   get_pendant_status() != PENDANT_DATA_RECEIVED)){
		uint16_t color = WHITE;
		char pendant_status_str[30] = {0};

		pendant_status = get_pendant_status();
		switch(pendant_status){
			case PENDANT_DISCONNECTED:
				strcpy(pendant_status_str, "Pendant DISCONNECTED");
				color = RED;
				break;
			case PENDANT_CONNECTED:
				strcpy(pendant_status_str, "Pendant CONNECTED");
				color = GREEN;
				break;
			case PENDANT_CONNECTING:
				strcpy(pendant_status_str, "Pendant CONNECTING");
				color = ORANGE;
				break;
			case PENDANT_DISCONNECTING:
				color = ORANGE;
				strcpy(pendant_status_str, "Pendant DISCONNECTING");
				break;
			case PENDANT_ERROR:
				strcpy(pendant_status_str, "Pendant COMM. ERROR");
				color = YELLOW;
				break;
			default:
				break;
		}

		R61529_FillRect(BLACK, 160, 40, 395, 60);
		R61529_WriteString(160, 40, pendant_status_str, Font_11x18, color, BLACK);
	}

	// Display RPM
	if(grbl_rpm != grbl_data.spindle.rpm_programmed){
		uint16_t color = WHITE;
		char rpm_str[10] = {0}, prev_rpm_str[10] = {0};
		uint8_t rpm_str_len, prev_rpm_str_len;

		if(grbl_rpm != 0){
			color = ORANGE;
		}
		sprintf(prev_rpm_str, "%4.3f", grbl_rpm);
		sprintf(rpm_str, "%4.3f", grbl_data.spindle.rpm_programmed);
		rpm_str_len = strlen(rpm_str);
		prev_rpm_str_len = strlen(prev_rpm_str);
		grbl_rpm = grbl_data.spindle.rpm_programmed;

		if(rpm_str_len < prev_rpm_str_len)
			R61529_FillRect(BLACK, 80, 250, 80 + (prev_rpm_str_len * 11), 270);
		R61529_WriteString(80, 250 + ((26 - 18) / 2), rpm_str, Font_11x18, color, BLACK);
	}

	// Display jogging mode
	if(pendant_jog_mode != pendant_data.jog_mode){
		pendant_jog_mode = pendant_data.jog_mode;
		R61529_FillRect(BLACK, 400, 270, 470, 290);
		switch(pendant_data.jog_mode){
			case 0:
				R61529_WriteString(400, 270, "SLOW", Font_11x18, GREEN, BLACK);
				break;
			case 1:
				R61529_WriteString(400, 270, "MEDIUM", Font_11x18, ORANGE, BLACK);
				break;
			case 2:
				R61529_WriteString(400, 270, "FAST", Font_11x18, RED, BLACK);
				break;
			default:
				break;
		}
	}
}





// Level 2 menu special functions
/**
  * @brief Update screen data for screen brightness setting sub-menu
  */
void screen_brightness_setting_update_screen(void){
	uint16_t end_x = ((menu_controller.brightness_value * (SCREEN_BRIGHTNESS_SETTING_SLIDER_WIDTH - 2)) / 100) + SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X + 1;
	char brightness_str[5] = {0};

	sprintf(brightness_str, "%3d", menu_controller.brightness_value);

	R61529_WriteString(SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X + strlen("Brightness - ") * 11, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y + SCREEN_BRIGHTNESS_SETTING_SLIDER_HEIGHT + 10, brightness_str, Font_11x18, GREEN, BLACK);
	R61529_FillRect(CYAN, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X + 1, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y + 1, end_x, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_Y - 1);
	if((end_x + 1) != SCREEN_BRIGHTNESS_SETTING_SLIDER_END_X){
		R61529_FillRect(GRAY, end_x + 1, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y + 1, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_X - 1, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_Y - 1);
	}
}





// Level 4 menu special functions
/**
  * @brief Delete a given x axis distance setting
  */
void delete_x_axis_dist_setting(uint8_t index){
	uint16_t setting_base_x = GET_X_AXIS_DIST_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_X_AXIS_DIST_SETTING_BLOCK_BASE_Y(index);

	if(index > (X_AXIS_DIST_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(BLACK, setting_base_x, setting_base_y, setting_base_x + X_AXIS_DIST_SETTING_BLOCK_WIDTH, setting_base_y + X_AXIS_DIST_SETTING_BLOCK_HEIGHT);
}

/**
  * @brief Select a given x axis distance setting
  */
void select_x_axis_dist_setting(uint8_t index){
	uint16_t setting_base_x = GET_X_AXIS_DIST_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_X_AXIS_DIST_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(index);
	uint16_t setting_decimal_base_x, setting_decimal_base_y;
	char decimal_str[10] = {0};

	if(index > (X_AXIS_DIST_SETTINGS - 1)){
		return;
	}

	sprintf(decimal_str, "%1.3f", mpg_settings.x_axis_multiplicity[index]);
	setting_decimal_base_x = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X + X_AXIS_DIST_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
	setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)

	R61529_FillRect(GREEN, setting_base_x, setting_base_y, setting_base_x + X_AXIS_DIST_SETTING_BLOCK_WIDTH, setting_base_y + X_AXIS_DIST_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, x_axis_dist_settings[index], Font_16x26, BROWN, GREEN);
	R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, PURPLE, GREEN);
}

/**
  * @brief Un-select a given x axis distance setting
  */
void unselect_x_axis_dist_setting(uint8_t index){
	uint16_t setting_base_x = GET_X_AXIS_DIST_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_X_AXIS_DIST_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(index);
	uint16_t setting_decimal_base_x, setting_decimal_base_y;
	char decimal_str[10] = {0};

	if(index > (X_AXIS_DIST_SETTINGS - 1)){
		return;
	}

	sprintf(decimal_str, "%1.3f", mpg_settings.x_axis_multiplicity[index]);
	setting_decimal_base_x = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X + X_AXIS_DIST_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
	setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)

	R61529_FillRect(GRAY, setting_base_x, setting_base_y, setting_base_x + X_AXIS_DIST_SETTING_BLOCK_WIDTH, setting_base_y + X_AXIS_DIST_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, x_axis_dist_settings[index], Font_16x26, WHITE, GRAY);
	R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, BROWN, GRAY);
}

/**
  * @brief Delete a given y/z axis distance setting
  */
void delete_yz_axis_dist_setting(uint8_t index){
	uint16_t setting_base_x = GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_Y(index);

	if(index > (YZ_AXIS_DIST_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(BLACK, setting_base_x, setting_base_y, setting_base_x + YZ_AXIS_DIST_SETTING_BLOCK_WIDTH, setting_base_y + YZ_AXIS_DIST_SETTING_BLOCK_HEIGHT);
}

/**
  * @brief Select a given yz axis distance setting
  */
void select_yz_axis_dist_setting(uint8_t index){
	uint16_t setting_base_x = GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(index);
	uint16_t setting_decimal_base_x, setting_decimal_base_y;
	char decimal_str[10] = {0};

	if(index > (X_AXIS_DIST_SETTINGS - 1)){
		return;
	}

	sprintf(decimal_str, "%1.3f", mpg_settings.yz_axis_multiplicity[index]);
	setting_decimal_base_x = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X + YZ_AXIS_DIST_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
	setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)

	R61529_FillRect(GREEN, setting_base_x, setting_base_y, setting_base_x + YZ_AXIS_DIST_SETTING_BLOCK_WIDTH, setting_base_y + YZ_AXIS_DIST_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, yz_axis_dist_settings[index], Font_16x26, BROWN, GREEN);
	R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, PURPLE, GREEN);
}

/**
  * @brief Un-select a given yz axis distance setting
  */
void unselect_yz_axis_dist_setting(uint8_t index){
	uint16_t setting_base_x = GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_YZ_AXIS_DIST_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(index);
	uint16_t setting_decimal_base_x, setting_decimal_base_y;
	char decimal_str[10] = {0};

	if(index > (YZ_AXIS_DIST_SETTINGS - 1)){
		return;
	}

	sprintf(decimal_str, "%1.3f", mpg_settings.yz_axis_multiplicity[index]);
	setting_decimal_base_x = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X + YZ_AXIS_DIST_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
	setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)

	R61529_FillRect(GRAY, setting_base_x, setting_base_y, setting_base_x + YZ_AXIS_DIST_SETTING_BLOCK_WIDTH, setting_base_y + YZ_AXIS_DIST_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, yz_axis_dist_settings[index], Font_16x26, WHITE, GRAY);
	R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, BROWN, GRAY);
}

/**
  * @brief Delete a given jog speed setting
  */
void delete_jog_feed_setting(uint8_t index){
	uint16_t setting_base_x = GET_JOG_FEED_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_JOG_FEED_SETTING_BLOCK_BASE_Y(index);

	if(index > (JOG_FEED_SETTINGS - 1)){
		return;
	}

	R61529_FillRect(BLACK, setting_base_x, setting_base_y, setting_base_x + JOG_FEED_SETTING_BLOCK_WIDTH, setting_base_y + JOG_FEED_SETTING_BLOCK_HEIGHT);
}

/**
  * @brief Select a given jog speed setting
  */
void select_jog_feed_setting(uint8_t index){
	uint16_t setting_base_x = GET_JOG_FEED_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_JOG_FEED_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_Y(index);
	uint16_t setting_decimal_base_x, setting_decimal_base_y;
	char decimal_str[10] = {0};

	if(index > (JOG_FEED_SETTINGS - 1)){
		return;
	}

	sprintf(decimal_str, "%4u", mpg_settings.jog_speeds[index]);
	setting_decimal_base_x = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_X + JOG_FEED_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
	setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)

	R61529_FillRect(GREEN, setting_base_x, setting_base_y, setting_base_x + JOG_FEED_SETTING_BLOCK_WIDTH, setting_base_y + JOG_FEED_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, jog_feed_settings[index], Font_16x26, BROWN, GREEN);
	R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, PURPLE, GREEN);
}

/**
  * @brief Un-select a given jog speed setting
  */
void unselect_jog_feed_setting(uint8_t index){
	uint16_t setting_base_x = GET_JOG_FEED_SETTING_BLOCK_BASE_X;
	uint16_t setting_base_y = GET_JOG_FEED_SETTING_BLOCK_BASE_Y(index);
	uint16_t setting_text_base_x = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_X;
	uint16_t setting_text_base_y = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_Y(index);
	uint16_t setting_decimal_base_x, setting_decimal_base_y;
	char decimal_str[10] = {0};

	if(index > (JOG_FEED_SETTINGS - 1)){
		return;
	}

	sprintf(decimal_str, "%4u", mpg_settings.jog_speeds[index]);
	setting_decimal_base_x = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_X + JOG_FEED_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
	setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)

	R61529_FillRect(GRAY, setting_base_x, setting_base_y, setting_base_x + JOG_FEED_SETTING_BLOCK_WIDTH, setting_base_y + JOG_FEED_SETTING_BLOCK_HEIGHT);
	R61529_WriteString(setting_text_base_x, setting_text_base_y, jog_feed_settings[index], Font_16x26, WHITE, GRAY);
	R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, BROWN, GRAY);
}


void mpg_axes_update_settings(void){
	if((menu_controller.level_indexes[0] & 0xFF00 >> 8) == 2 && (menu_controller.level_indexes[1] & 0xFF00 >> 8) == 1){
		uint16_t setting_text_base_y = 0;
		uint16_t setting_decimal_base_x = 0;
		uint16_t setting_decimal_base_y = 0;
		char decimal_str[10] = {0};

		if((menu_controller.level_indexes[2] & 0xFF00 >> 8) == 0){ ////TODO: X axis jogging distances
			switch((menu_controller.level_indexes[3] & 0xFF00 >> 8)){
				case 0: //Slow
					setting_text_base_y = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(0);
					sprintf(decimal_str, "%1.3f", mpg_settings.x_axis_multiplicity[0]);
					break;
				case 1: //Medium
					setting_text_base_y = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(1);
					sprintf(decimal_str, "%1.3f", mpg_settings.x_axis_multiplicity[1]);
					break;
				case 2:	//Fast
					setting_text_base_y = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(2);
					sprintf(decimal_str, "%1.3f", mpg_settings.x_axis_multiplicity[2]);
					break;
				default:
					break;
			}
			setting_decimal_base_x = GET_X_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X + X_AXIS_DIST_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
			setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)
			R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, PURPLE, GREEN);
		}else if((menu_controller.level_indexes[2] & 0xFF00 >> 8) == 1){ //TODO: Y/Z axis jogging distances
			switch((menu_controller.level_indexes[3] & 0xFF00 >> 8)){
				case 0: //Slow
					setting_text_base_y = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(0);
					sprintf(decimal_str, "%1.3f", mpg_settings.yz_axis_multiplicity[0]);
					break;
				case 1: //Medium
					setting_text_base_y = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(1);
					sprintf(decimal_str, "%1.3f", mpg_settings.yz_axis_multiplicity[1]);
					break;
				case 2:	//Fast
					setting_text_base_y = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_Y(2);
					sprintf(decimal_str, "%1.3f", mpg_settings.yz_axis_multiplicity[2]);
					break;
				default:
					break;
			}
			setting_decimal_base_x = GET_YZ_AXIS_DIST_SETTING_BLOCK_TEXT_BASE_X + YZ_AXIS_DIST_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
			setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)
			R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, PURPLE, GREEN);
		}else if((menu_controller.level_indexes[2] & 0xFF00 >> 8) == 2){ //TODO: Jogging feed rates
			switch((menu_controller.level_indexes[3] & 0xFF00 >> 8)){
				case 0: //Slow
					setting_text_base_y = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_Y(0);
					sprintf(decimal_str, "%4u", mpg_settings.jog_speeds[0]);
					break;
				case 1: //Medium
					setting_text_base_y = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_Y(1);
					sprintf(decimal_str, "%4u", mpg_settings.jog_speeds[1]);
					break;
				case 2:	//Fast
					setting_text_base_y = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_Y(2);
					sprintf(decimal_str, "%4u", mpg_settings.jog_speeds[2]);
					break;
				default:
					break;
			}
			setting_decimal_base_x = GET_JOG_FEED_SETTING_BLOCK_TEXT_BASE_X + JOG_FEED_SETTING_BLOCK_WIDTH - 40 - (strlen(decimal_str) * 11);
			setting_decimal_base_y = setting_text_base_y + 4; // + ((26 - 18) / 2)
			R61529_WriteString(setting_decimal_base_x, setting_decimal_base_y, decimal_str, Font_11x18, PURPLE, GREEN);
		}
	}
}

void mpg_spindle_update_setting(void){
	if((menu_controller.level_indexes[0] & 0xFF00 >> 8) == 2 && (menu_controller.level_indexes[1] & 0xFF00 >> 8) == 2){
		if((menu_controller.level_indexes[2] & 0xFF00 >> 8) == 0){
			uint16_t speed_max_min_diff = SPINDLE_SPEED_MAX - SPINDLE_SPEED_MIN;
			uint16_t end_x = (((mpg_settings.spindle_speed - SPINDLE_SPEED_MIN) * (SPINDLE_SPEED_SETTING_SLIDER_WIDTH - 2)) / speed_max_min_diff) + SPINDLE_SPEED_SETTING_SLIDER_BASE_X + 1;
			char spindle_speed_str[10] = {0};

			sprintf(spindle_speed_str, "%4u", mpg_settings.spindle_speed);

			R61529_WriteString(SPINDLE_SPEED_SETTING_SLIDER_BASE_X + strlen("Spindle speed: ") * 11, SPINDLE_SPEED_SETTING_SLIDER_BASE_Y + SPINDLE_SPEED_SETTING_SLIDER_HEIGHT + 10, spindle_speed_str, Font_11x18, MAGENTA, BLACK);
			R61529_FillRect(YELLOW, SPINDLE_SPEED_SETTING_SLIDER_BASE_X + 1, SPINDLE_SPEED_SETTING_SLIDER_BASE_Y + 1, end_x, SPINDLE_SPEED_SETTING_SLIDER_END_Y - 1);
			if((end_x + 1) != SPINDLE_SPEED_SETTING_SLIDER_END_X){
				R61529_FillRect(GRAY, end_x + 1, SPINDLE_SPEED_SETTING_SLIDER_BASE_Y + 1, SPINDLE_SPEED_SETTING_SLIDER_END_X - 1, SPINDLE_SPEED_SETTING_SLIDER_END_Y - 1);
			}
		}
	}
}





// Other functions
/**
  * @brief Process received information from the MPG pendant
  * and do the according actions
  * (send the according data packets to GRBL)
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
	action.buttons = current_data.buttons;

	// Translate pendant data into actions
	pendant_data_to_action(action);

	// Save current data
	memcpy(&data, &current_data, sizeof(pendant_data_t));
}
