/*
 * screen_menu_r61529.c
 *
 *  Created on: Oct 17, 2023
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include "r61529_screen_menu.h"
#include "r61529.h"
#include "ft5436.h"
#include "buttons.h"
#include "systick_timer.h"
#include "sound_fx.h"
#include "stm32h7xx_it.h"
#include "stm32h7xx_hal.h"
#include "../grblHALComm/parser.h"
#include "../grblHALComm/sender.h"


/* Private define ------------------------------------------------------------*/
#define LEVELS 3
#define MAXIMUM_BRIGHTNESS	100
#define MINIMUM_BRIGHTNESS	10

#define SUBMENUS 3
#define SUBMENU_BLOCK_WIDTH 150
#define SUBMENU_BLOCK_HEIGHT 60
#define SUBMENU_BLOCK_X_OFFSET (480 - (SUBMENU_BLOCK_WIDTH * SUBMENUS)) / (SUBMENUS + 1)
#define SUBMENU_BLOCK_X_MARGIN SUBMENU_BLOCK_X_OFFSET
#define SUBMENU_BLOCK_Y_MARGIN 30

#define SETTINGS_SUBMENUS 3
#define SETTING_BLOCK_WIDTH 250
#define SETTING_BLOCK_HEIGHT 40
#define SETTING_BLOCK_Y_OFFSET 20
#define SETTING_BLOCK_X_MARGIN 20
#define SETTING_BLOCK_Y_MARGIN 20

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


/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern uint8_t touch_detected;
extern uint16_t touchX[FT_REG_NUMTOUCHES];
extern uint16_t touchY[FT_REG_NUMTOUCHES];
extern grbl_data_t grbl_data;
extern settings_t settings;

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

//Auto and Manual mode variables
static grbl_state_t grbl_state = -1;
static double grbl_position[3] = {-1,-1,-1};
static double grbl_offset[3] = {-1,-1,-1};
static bool grbl_coolant_status[3] = {false, false, false};
static double grbl_rpm = -1;
static uint8_t mpg_state = -1;


/* Private function prototypes -----------------------------------------------*/
void init_button_inputs(void);
void clear_button_channel_cnts(void);
void process_user_input(void);
int8_t touch_select_submenu(uint8_t);
bool touch_exit_submenu(uint8_t);
void r61529_screen_menu(void);
void draw_loading_screen(void);
void draw_menu(void);
void delete_menu(void);
void delete_submenu(uint8_t);
void select_submenu(uint8_t);
void unselect_submenu(uint8_t);
void delete_setting(uint8_t);
void select_setting(uint8_t);
void unselect_setting(uint8_t);

// Level 1 menu special functions
void auto_mode_update_screen(void);
void manual_mode_update_screen(void);

// Level 2 menu special functions
void screen_brightness_setting_update_screen(void);


/* Private user code -----------------------------------------------*/
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

/**
  * @brief Return if sub-menu is selected using touch screen
  */
int8_t touch_select_submenu(uint8_t level){
	int8_t submenu = -1;

	if(menu_controller.touch_point_cnt){
		uint16_t x = menu_controller.touch_points[0][0];
		uint16_t y = menu_controller.touch_points[0][1];
		switch(level){
			case 0:		//Main menu
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
					case 2:
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

		//Clear the counters of the button channels
		clear_button_channel_cnts();

		//Go to a given sub-menu based on touch screen data
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
				int8_t index = menu_controller.level_indexes[0] & 0x00FF;
				if(get_button_state(BUTTON_RIGHT_CH) == GPIO_PIN_RESET){
					if(++menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == 1){
						menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] = 0;
						if(++index == SUBMENUS)	index = 0;
					}
				}else if(get_button_state(BUTTON_LEFT_CH) == GPIO_PIN_RESET){
					if(++menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == 1){
						menu_controller.button_delay_cnts[BUTTON_LEFT_CH] = 0;
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
					if(++menu_controller.button_delay_cnts[BUTTON_BACK_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_BACK_CH] == 1){
						menu_controller.button_delay_cnts[BUTTON_BACK_CH] = 0;
						delete_menu();
						menu_controller.level = 0;
						menu_controller.level_indexes[1] = 0;
						menu_controller.level_indexes[0] &= 0x00FF;
						draw_menu();
						break;
					}
				}

				if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 0){
					auto_mode_update_screen();
				}else if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 1){		//TODO: Manual mode sub-menu controls
					manual_mode_update_screen();

					if(get_button_state(BUTTON_ENTER_CH) == GPIO_PIN_RESET){
						if(++menu_controller.button_delay_cnts[BUTTON_ENTER_CH] == menu_controller.button_delay){
							menu_controller.button_delay_cnts[BUTTON_ENTER_CH] = 0;

							HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_SET);
							if(grbl_data.mpgMode){
								disable_mpg();
							}else{
								enable_mpg();
							}
							HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_RESET);
						}
					}

					if(grbl_data.mpgMode){
						// Request GRBL data every 100ms
						if(wait_ms_ch(4, 100)){
							request_report();
						}

						HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_SET);
						/*MOVE X axis +1mm*/if(get_button_state(BUTTON_RIGHT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == menu_controller.button_delay){
								menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] = 0;
								grbl_send_packet((uint8_t*)"$J=G91G21X1F1000\n", 17);
							}
						/*MOVE X axis -1mm*/}else if(get_button_state(BUTTON_LEFT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == menu_controller.button_delay){
								menu_controller.button_delay_cnts[BUTTON_LEFT_CH] = 0;
								grbl_send_packet((uint8_t*)"$J=G91G21X-1F1000\n", 18);
							}
						/*MOVE Y axis +1mm*/}else if(get_button_state(BUTTON_UP_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_UP_CH] == menu_controller.button_delay){
								menu_controller.button_delay_cnts[BUTTON_UP_CH] = 0;
								grbl_send_packet((uint8_t*)"$J=G91G21Y1F1000\n", 17);
							}
						/*MOVE Y axis -1mm*/}else if(get_button_state(BUTTON_DOWN_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_DOWN_CH] == menu_controller.button_delay){
								menu_controller.button_delay_cnts[BUTTON_DOWN_CH] = 0;
								grbl_send_packet((uint8_t*)"$J=G91G21Y-1F1000\n", 18);
							}
						}
						HAL_GPIO_WritePin(TEST_GPIO_Port, TEST_Pin, GPIO_PIN_RESET);
					}
				}else if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 2){
					int8_t index = menu_controller.level_indexes[1] & 0x00FF;
					if(get_button_state(BUTTON_DOWN_CH) == GPIO_PIN_RESET){
						if(++menu_controller.button_delay_cnts[BUTTON_DOWN_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_DOWN_CH] == 1){
							menu_controller.button_delay_cnts[BUTTON_DOWN_CH] = 0;
							if(++index == SETTINGS_SUBMENUS)	index = 0;
						}
					}else if(get_button_state(BUTTON_UP_CH) == GPIO_PIN_RESET){
						if(++menu_controller.button_delay_cnts[BUTTON_UP_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_UP_CH] == 1){
							menu_controller.button_delay_cnts[BUTTON_UP_CH] = 0;
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
					if(++menu_controller.button_delay_cnts[BUTTON_BACK_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_BACK_CH] == 1){
						menu_controller.button_delay_cnts[BUTTON_BACK_CH] = 0;
						delete_menu();
						menu_controller.level = 1;
						menu_controller.level_indexes[2] = 0;
						menu_controller.level_indexes[1] &= 0x00FF;
						draw_menu();
						break;
					}
				}

				if(((menu_controller.level_indexes[0] & 0xFF00) >> 8) == 2){
					if(((menu_controller.level_indexes[1] & 0xFF00) >> 8) == 0){
						int8_t previous_brightness_value = menu_controller.brightness_value;

						if(get_button_state(BUTTON_RIGHT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] == 1){
								menu_controller.button_delay_cnts[BUTTON_RIGHT_CH] = 0;
								if(++previous_brightness_value > MAXIMUM_BRIGHTNESS)	previous_brightness_value = MAXIMUM_BRIGHTNESS;
							}
						}else if(get_button_state(BUTTON_LEFT_CH) == GPIO_PIN_RESET){
							if(++menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == menu_controller.button_delay || menu_controller.button_delay_cnts[BUTTON_LEFT_CH] == 1){
								menu_controller.button_delay_cnts[BUTTON_LEFT_CH] = 0;
								if(--previous_brightness_value < MINIMUM_BRIGHTNESS)	previous_brightness_value = MINIMUM_BRIGHTNESS;
							}
						}

						if(previous_brightness_value != menu_controller.brightness_value){
							menu_controller.brightness_value = previous_brightness_value;
							htim3.Instance->CCR2 = (menu_controller.brightness_value * htim3.Instance->ARR) / 100;
							screen_brightness_setting_update_screen();
						}
					}
				}
				break;
			default:
				break;
		}
	}
}

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
			R61529_WriteString(410, 5, "Back", Font_16x26, WHITE, RED);
			switch((menu_controller.level_indexes[0] & 0xFF00 >> 8)){
				case 0: 	//TODO: Auto mode sub-menu draw
					R61529_WriteString(10, 10, "Auto mode engaged", Font_11x18, PURPLE, BLACK);

					// Position section
					R61529_WriteString(10, 80,  "Position", Font_11x18, YELLOW, BLACK);
					R61529_WriteString(10, 100, "X:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(10, 130, "Y:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(10, 160, "Z:", Font_16x26, WHITE, BLACK);

					// Offset section
					R61529_WriteString(150, 80,  "Offset", Font_11x18, WHITE, BLACK);
					R61529_WriteString(150, 100, "X:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(150, 130, "Y:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(150, 160, "Z:", Font_16x26, WHITE, BLACK);

					// Coolant status section
					R61529_WriteString(290, 80,  "Coolant", Font_11x18, WHITE, BLACK);
					R61529_WriteString(290, 100, "MIST", Font_11x18, RED, GRAY);
					R61529_WriteString(290, 130, "FLOOD", Font_11x18, RED, GRAY);
					R61529_WriteString(290, 160, "SPINDLE", Font_11x18, RED, GRAY);

					// RPM
					R61529_WriteString(10, 215,  "RPM:", Font_16x26, WHITE, BLACK);

					// Status
					R61529_WriteString(10, 270,  "Status:", Font_11x18, WHITE, BLACK);

					// Show data on screen
					auto_mode_update_screen();
					break;
				case 1: 	//TODO: Manual mode sub-menu draw
					R61529_WriteString(10, 10, "Manual mode engaged", Font_11x18, PURPLE, BLACK);

					// Position section
					R61529_WriteString(10, 80,  "Position", Font_11x18, YELLOW, BLACK);
					R61529_WriteString(10, 100, "X:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(10, 130, "Y:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(10, 160, "Z:", Font_16x26, WHITE, BLACK);

					// Offset section
					R61529_WriteString(150, 80,  "Offset", Font_11x18, WHITE, BLACK);
					R61529_WriteString(150, 100, "X:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(150, 130, "Y:", Font_16x26, WHITE, BLACK);
					R61529_WriteString(150, 160, "Z:", Font_16x26, WHITE, BLACK);

					// Coolant status section
					R61529_WriteString(290, 80,  "Coolant", Font_11x18, WHITE, BLACK);
					R61529_WriteString(290, 100, "MIST", Font_11x18, RED, GRAY);
					R61529_WriteString(290, 130, "FLOOD", Font_11x18, RED, GRAY);
					R61529_WriteString(290, 160, "SPINDLE", Font_11x18, RED, GRAY);

					// RPM
					R61529_WriteString(10, 215,  "RPM:", Font_16x26, WHITE, BLACK);

					// Status
					R61529_WriteString(10, 270,  "Status:", Font_11x18, WHITE, BLACK);

					// Show data on screen
					manual_mode_update_screen();
					break;
				case 2:		// Settings
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
			break;
		case 2:
			R61529_WriteString(20, 5, settings_options[(menu_controller.level_indexes[1] & 0xFF00 >> 8)], Font_16x26, CYAN, BLACK);
			R61529_WriteString(410, 5, "Back", Font_16x26, WHITE, RED);

			switch((menu_controller.level_indexes[0] & 0xFF00 >> 8)){
				case 0:		//MPG Auto mode
					break;
				case 1:		//MPG Manual mode
					break;
				case 2:		//Settings
					if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 0){				//Screen brightness
						R61529_DrawRect(WHITE, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_Y);
						R61529_WriteString(SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y + SCREEN_BRIGHTNESS_SETTING_SLIDER_HEIGHT + 10, "Brightness - ", Font_11x18, GREEN, BLACK);
						screen_brightness_setting_update_screen();
					}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 1){		//MPG axes settings
					}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 2){		//MPG spindle settings
					}
					break;
				default:
					break;
			}
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
			switch((menu_controller.level_indexes[0] & 0xFF00 >> 8)){
				case 0: 	//TODO: Auto mode sub-menu screen deletion
					R61529_FillScreen(BLACK);
					grbl_state = -1;
					grbl_rpm = -1;
					for(uint8_t i = 0; i < 3; i++){
						grbl_position[i] = -1;
						grbl_offset[i] = -1;
						grbl_coolant_status[i] = -1;
					}
					break;
				case 1: 	//TODO: Manual mode sub-menu screen deletion
					R61529_FillScreen(BLACK);
					grbl_state = -1;
					grbl_rpm = -1;
					mpg_state = -1;
					for(uint8_t i = 0; i < 3; i++){
						grbl_position[i] = -1;
						grbl_offset[i] = -1;
						grbl_coolant_status[i] = -1;
					}
					break;
				case 2:		//Settings
					for(uint8_t i = 0; i < SETTINGS_SUBMENUS; i++){
						delete_setting(i);
					}
					break;
				default:
					break;
			}
			break;
		case 2:
			R61529_WriteString(20, 5, settings_options[(menu_controller.level_indexes[1] & 0xFF00 >> 8)], Font_16x26, BLACK, BLACK);
			R61529_WriteString(410, 5, "Back", Font_16x26, BLACK, BLACK);
			switch((menu_controller.level_indexes[0] & 0xFF00 >> 8)){
				case 0:		//MPG Auto mode
					break;
				case 1:		//MPG Manual mode
					break;
				case 2:		//Settings
					if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 0){				//Screen brightness
						R61529_FillRect(BLACK, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_BASE_Y, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_X, SCREEN_BRIGHTNESS_SETTING_SLIDER_END_Y + 30);
					}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 1){		//MPG axes settings
					}else if((menu_controller.level_indexes[1] & 0xFF00 >> 8) == 2){		//MPG spindle settings
					}
					break;
				default:
					break;
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
				R61529_FillRect(BLACK, 50, 104 + (30 * i), 50 + (prev_pos_str_len * 11), 104 + (30 * i) + 18);
			R61529_WriteString(50, 104 + (30 * i), pos_str, Font_11x18, PURPLE, BLACK);
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
				R61529_FillRect(BLACK, 190, 104 + (30 * i), 190 + (prev_offset_str_len * 11), 104 + (30 * i) + 18);
			R61529_WriteString(190, 104 + (30 * i), offset_str, Font_11x18, PURPLE, BLACK);
		}
	}

	// Display coolant status
	for(uint8_t i = 0; i < 3; i++){
		if(grbl_data.coolant.mist != grbl_coolant_status[0]){
			uint16_t color = RED, bg_color = GRAY;
			grbl_coolant_status[0] = grbl_data.coolant.mist;

			if(grbl_data.coolant.mist){
				color = GREEN;
				bg_color = BLACK;
			}

			R61529_WriteString(290, 100, "MIST", Font_11x18, color, bg_color);
		}
		if(grbl_data.coolant.flood != grbl_coolant_status[1]){
			uint16_t color = RED, bg_color = GRAY;
			grbl_coolant_status[1] = grbl_data.coolant.flood;

			if(grbl_data.coolant.flood){
				color = GREEN;
				bg_color = BLACK;
			}

			R61529_WriteString(290, 130, "FLOOD", Font_11x18, color, bg_color);
		}
		if(grbl_data.coolant.trough_spindle != grbl_coolant_status[2]){
			uint16_t color = RED, bg_color = GRAY;
			grbl_coolant_status[2] = grbl_data.coolant.trough_spindle;

			if(grbl_data.coolant.trough_spindle){
				color = GREEN;
				bg_color = BLACK;
			}

			R61529_WriteString(290, 160, "SPINDLE", Font_11x18, color, bg_color);
		}
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

		R61529_FillRect(BLACK, 90, 270, 170, 290);
		R61529_WriteString(90, 270, state_str, Font_11x18, color, BLACK);
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
			R61529_FillRect(BLACK, 80, 219, 80 + (prev_rpm_str_len * 11), 219 + 18);
		R61529_WriteString(80, 219, rpm_str, Font_11x18, color, BLACK);
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
				R61529_FillRect(BLACK, 50, 104 + (30 * i), 50 + (prev_pos_str_len * 11), 104 + (30 * i) + 18);
			R61529_WriteString(50, 104 + (30 * i), pos_str, Font_11x18, PURPLE, BLACK);
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
				R61529_FillRect(BLACK, 190, 104 + (30 * i), 190 + (prev_offset_str_len * 11), 104 + (30 * i) + 18);
			R61529_WriteString(190, 104 + (30 * i), offset_str, Font_11x18, PURPLE, BLACK);
		}
	}

	// Display coolant status
	for(uint8_t i = 0; i < 3; i++){
		if(grbl_data.coolant.mist != grbl_coolant_status[0]){
			uint16_t color = RED, bg_color = GRAY;
			grbl_coolant_status[0] = grbl_data.coolant.mist;

			if(grbl_data.coolant.mist){
				color = GREEN;
				bg_color = BLACK;
			}

			R61529_WriteString(290, 100, "MIST", Font_11x18, color, bg_color);
		}
		if(grbl_data.coolant.flood != grbl_coolant_status[1]){
			uint16_t color = RED, bg_color = GRAY;
			grbl_coolant_status[1] = grbl_data.coolant.flood;

			if(grbl_data.coolant.flood){
				color = GREEN;
				bg_color = BLACK;
			}

			R61529_WriteString(290, 130, "FLOOD", Font_11x18, color, bg_color);
		}
		if(grbl_data.coolant.trough_spindle != grbl_coolant_status[2]){
			uint16_t color = RED, bg_color = GRAY;
			grbl_coolant_status[2] = grbl_data.coolant.trough_spindle;

			if(grbl_data.coolant.trough_spindle){
				color = GREEN;
				bg_color = BLACK;
			}

			R61529_WriteString(290, 160, "SPINDLE", Font_11x18, color, bg_color);
		}
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

		R61529_FillRect(BLACK, 90, 270, 170, 290);
		R61529_WriteString(90, 270, state_str, Font_11x18, color, BLACK);
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
			R61529_FillRect(BLACK, 80, 219, 80 + (prev_rpm_str_len * 11), 219 + 18);
		R61529_WriteString(80, 219, rpm_str, Font_11x18, color, BLACK);
	}
}


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
