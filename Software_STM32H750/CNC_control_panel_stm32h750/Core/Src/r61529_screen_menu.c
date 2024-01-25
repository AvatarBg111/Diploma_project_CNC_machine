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


/* Private typedef -----------------------------------------------------------*/
typedef struct{
	bool inside_main_menu;
	bool main_menu_drawn;
	bool submenu_drawn;

	uint8_t menu_flags;
	int8_t submenu_index;
	uint8_t submenu_flags;
	uint8_t button_delay;
	uint8_t button_delay_cnt;

	uint32_t encoder_value;
	int8_t brightness_value;
	uint8_t touch_point_cnt;
	int16_t touch_points[FT_REG_NUMTOUCHES][2];
}_menu_controller;


/* Private define ------------------------------------------------------------*/
#define OPTIONS 3
#define BUTTON_UP_CH 0
#define BUTTON_DOWN_CH 1
#define BUTTON_LEFT_CH 2
#define BUTTON_RIGHT_CH 3
#define BUTTON_ENTER_CH 4
#define BUTTON_BACK_CH 5
#define INPUT_DELAY_TIME 20
#define DEFAULT_DELAY 200

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern uint8_t touch_detected;
extern uint16_t touchX[FT_REG_NUMTOUCHES];
extern uint16_t touchY[FT_REG_NUMTOUCHES];


/* Private variables ---------------------------------------------------------*/
_menu_controller menu_controller = {0};
static const char *main_menu_options[OPTIONS] = {
	"Enter MPG mode - 0",
	"MPG Settings - 1",
	"Screen brightness - 2",
};


/* Private function prototypes -----------------------------------------------*/
void main_menu(void);
void submenu(void);
void process_user_input(void);
void r61529_screen_menu(void);
void init_button_inputs(void);
void draw_loading_screen(void);
void draw_main_menu(void);
void delete_main_menu(void);
void draw_submenu(uint8_t);
void update_submenu_graphics(uint8_t);
void delete_submenu(uint8_t);
void select_submenu(uint8_t);
void unselect_submenu(uint8_t);


/* Private user code -----------------------------------------------*/
/**
  * @brief Main menu
  */
void main_menu(void){
	if(menu_controller.inside_main_menu && !menu_controller.main_menu_drawn){
		draw_main_menu();
		menu_controller.main_menu_drawn = true;
	}
}

/**
  * @brief Sub-menu selector
  */
void submenu(void){
	if(!menu_controller.submenu_drawn){
		draw_submenu(menu_controller.submenu_index);
		menu_controller.submenu_drawn = true;
	}
	switch(menu_controller.submenu_index){
		case 0:		//TODO: MPG mode
			break;
		case 1:		//TODO: MPG settings
			break;
		case 2:		//Screen brightness
			int16_t diff = 0;
			if(menu_controller.encoder_value > htim2.Instance->CNT && (menu_controller.encoder_value - htim2.Instance->CNT) > 60000){
				diff += (htim2.Instance->CNT + (0xFFFF - menu_controller.encoder_value)) / 2;
				menu_controller.brightness_value++;
			}else if(menu_controller.encoder_value < htim2.Instance->CNT && (htim2.Instance->CNT - menu_controller.encoder_value) > 60000){
				diff -= (menu_controller.encoder_value + (0xFFFF - htim2.Instance->CNT)) / 2;
				menu_controller.brightness_value--;
			}else{
				diff = (htim2.Instance->CNT - menu_controller.encoder_value) / 2;
				if(diff < 0)
					menu_controller.brightness_value--;
				else if(diff > 0)
					menu_controller.brightness_value++;
				else break;
			}

			if(menu_controller.brightness_value < 0){
				menu_controller.brightness_value = 0;
			}else if(menu_controller.brightness_value > 100){
				menu_controller.brightness_value = 100;
			}

			update_submenu_graphics(2);
			menu_controller.encoder_value = htim2.Instance->CNT;
			htim3.Instance->CCR2 = (menu_controller.brightness_value * 10) > 1000 ? 999 : menu_controller.brightness_value * 10;

			break;
		default:
			break;
	}
}

/**
  * @brief Process user input
  */
void process_user_input(void){
	// Check for screen touch data
	menu_controller.touch_point_cnt = touch_detected;
	for(uint8_t i = 0; (i < FT_REG_NUMTOUCHES) && touch_detected; i++){
		if(i >= menu_controller.touch_point_cnt){
			menu_controller.touch_points[i][0] = -1;	// Sort of NaN
			menu_controller.touch_points[i][1] = -1;	// Sort of NaN
		}else{
			menu_controller.touch_points[i][0] = touchY[i];		// This is because of screen rotation
			menu_controller.touch_points[i][1] = (320 - touchX[i]);		// This is because of screen rotation
		}

		/*
		// Draw coordinates
		char touch_data[20] = {};
		sprintf(touch_data, "Touch: (%3d, %3d)", menu_controller.touch_points[0][0], menu_controller.touch_points[0][1]);
		R61529_WriteString(10, 200, touch_data, Font_11x18, CYAN, BLACK);
		*/
	}

	if(menu_controller.inside_main_menu){
		/** Main menu logic */
		uint8_t previous_submenu_index = menu_controller.submenu_index;

		if(menu_controller.touch_point_cnt){
			uint16_t x = menu_controller.touch_points[0][0];
			uint16_t y = menu_controller.touch_points[0][1];

			for(uint8_t i = 0; i < OPTIONS; i++){
				if((x > 10 && x < (10 + strlen(main_menu_options[i]) * 16)) && (y > ((i + 1) * 50) && y < (((i + 1) * 50) + 30))){
					menu_controller.submenu_index = i;
					menu_controller.main_menu_drawn = false;
					menu_controller.inside_main_menu = false;
					menu_controller.button_delay_cnt = menu_controller.button_delay;
					return;
				}
			}
		}

		if(get_button_state(BUTTON_UP_CH) == GPIO_PIN_RESET || menu_controller.encoder_value > htim2.Instance->CNT){
			if(menu_controller.button_delay_cnt == 0 || menu_controller.button_delay_cnt == menu_controller.button_delay){
				menu_controller.button_delay_cnt = menu_controller.button_delay - 1;
				previous_submenu_index = menu_controller.submenu_index;
				if(menu_controller.submenu_index-- == 0)
					menu_controller.submenu_index = OPTIONS - 1;
				menu_controller.encoder_value = htim2.Instance->CNT;
			}
			menu_controller.button_delay_cnt--;
		}else if(get_button_state(BUTTON_DOWN_CH) == GPIO_PIN_RESET || menu_controller.encoder_value < htim2.Instance->CNT){
			if(menu_controller.button_delay_cnt == 0 || menu_controller.button_delay_cnt == menu_controller.button_delay){
				menu_controller.button_delay_cnt = menu_controller.button_delay - 1;
				previous_submenu_index = menu_controller.submenu_index;
				if(++menu_controller.submenu_index == OPTIONS)
					menu_controller.submenu_index = 0;
				menu_controller.encoder_value = htim2.Instance->CNT;
			}
			menu_controller.button_delay_cnt--;
		}else if(get_button_state(BUTTON_ENTER_CH) == GPIO_PIN_RESET){
			menu_controller.main_menu_drawn = false;
			menu_controller.inside_main_menu = false;
			menu_controller.button_delay_cnt = menu_controller.button_delay;
		}

		if(menu_controller.submenu_index != previous_submenu_index){
			unselect_submenu(previous_submenu_index);
			select_submenu(menu_controller.submenu_index);
			buzzer_short_ring(450, (uint16_t)(menu_controller.button_delay * INPUT_DELAY_TIME));
		}
	}else{
		bool touch_exit = (menu_controller.touch_point_cnt != 0);
		touch_exit = (touch_exit && (menu_controller.touch_points[0][0] > 430));
		touch_exit = (touch_exit && (menu_controller.touch_points[0][1] < 50));

		if(get_button_state(BUTTON_UP_CH) == GPIO_PIN_RESET){
			// NOP
		}else if(get_button_state(BUTTON_DOWN_CH) == GPIO_PIN_RESET){
			// NOP
		}else if(get_button_state(BUTTON_ENTER_CH) == GPIO_PIN_RESET){
			// NOP
		}else if(get_button_state(BUTTON_BACK_CH) == GPIO_PIN_RESET || touch_exit){
			menu_controller.main_menu_drawn = false;
			menu_controller.submenu_drawn = false;
			menu_controller.inside_main_menu = true;
			menu_controller.button_delay_cnt = menu_controller.button_delay;
			delete_submenu(menu_controller.submenu_index);
		}
	}
}

/**
  * @brief Function that implements graphical menu of the system
  */
void r61529_screen_menu(void){
	if(menu_controller.inside_main_menu){
		main_menu();
	}else{
		submenu();
	}

	if(wait_ms_ch(0, INPUT_DELAY_TIME)){
		process_user_input();
	}
}

/**
  * @brief Initialize button inputs
  * Button channels (physical to logical representation)
  *    0		B
  * 2     3		B,
  *    1		D
  *  4   5
  * 0 - UP, 1 - DOWN, 2 - LEFT, 3 - RIGHT, 4 - ENTER, 5 - BACK
  */
void init_button_inputs(void){
	set_button_channel(USER_BUT_A_GPIO_Port, USER_BUT_A_Pin, BUTTON_RIGHT_CH);
	set_button_channel(USER_BUT_B_GPIO_Port, USER_BUT_B_Pin, BUTTON_UP_CH);
	set_button_channel(USER_BUT_C_GPIO_Port, USER_BUT_C_Pin, BUTTON_LEFT_CH);
	set_button_channel(USER_BUT_D_GPIO_Port, USER_BUT_D_Pin, BUTTON_DOWN_CH);
	set_button_channel(USER_BUT_E_GPIO_Port, USER_BUT_E_Pin, BUTTON_BACK_CH);
	set_button_channel(USER_BUT_F_GPIO_Port, USER_BUT_F_Pin, BUTTON_ENTER_CH);
}

/**
  * @brief Draw loading screen
  */
void draw_loading_screen(void){
	init_button_inputs();
	R61529_FillScreen(BLACK);
	R61529_WriteString(170, 145, "TITAN CNC", Font_16x26, BLACK, CYAN);
	menu_controller.inside_main_menu = true;
	menu_controller.button_delay = (uint8_t)(DEFAULT_DELAY / INPUT_DELAY_TIME);
	menu_controller.button_delay_cnt = 0;
	menu_controller.encoder_value = htim2.Instance->CNT;
	menu_controller.brightness_value = 50;

	menu_controller.touch_point_cnt = 0;
	for(uint8_t i = 0; i < FT_REG_NUMTOUCHES; i++){
		menu_controller.touch_points[i][0] = -1;	// Sort of NaN
		menu_controller.touch_points[i][1] = -1;	// Sort of NaN
	}
}

/**
  * @brief Draw main menu
  */
void draw_main_menu(void){
	for(uint8_t i = 0; i < OPTIONS; i++){
		if(i == menu_controller.submenu_index){
			select_submenu(i);
		}else{
			unselect_submenu(i);
		}
	}
}

/**
  * @brief Delete main menu
  */
void delete_main_menu(void){
	for(uint8_t i = 0; i < OPTIONS; i++){
		R61529_WriteString(10, 50 + (i * 50), main_menu_options[i], Font_16x26, BLACK, BLACK);
	}
}

/**
  * @brief Draw a given sub-menu
  */
void draw_submenu(uint8_t index){
	switch(index){
		case 0: //TODO: MPG mode
			R61529_WriteString(10, 10, "Hello from MPG submenu!", Font_16x26, CYAN, BLACK);
			R61529_WriteString(410, 5, "Exit", Font_16x26, WHITE, RED);
			break;
		case 1: //TODO: MPG settings
			R61529_WriteString(10, 10, "Hello from MPG settings!", Font_16x26, WHITE, BLACK);
			R61529_WriteString(410, 5, "Exit", Font_16x26, WHITE, RED);
			break;
		case 2:
			delete_main_menu();
			R61529_WriteString(10, 150, "Brightness level: ", Font_16x26, YELLOW, BLACK);
			R61529_DrawRect(WHITE, 10, 180, 310, 200);
			R61529_WriteString(410, 5, "Exit", Font_16x26, WHITE, RED);
			update_submenu_graphics(2);
			break;
	}
}

/**
  * @brief Update some graphics on a given sub-menu
  */
void update_submenu_graphics(uint8_t index){
	switch(index){
		case 0: //TODO: MPG mode
			break;
		case 1: //TODO: MPG settings
			break;
		case 2:
			char brightess[10] = {0};

			sprintf(brightess, "%3d%%", menu_controller.brightness_value);
			R61529_WriteString(290, 150, brightess, Font_16x26, YELLOW, BLACK);
			R61529_FillRect(CYAN, 10 + 1, 180 + 1, 11 + ((298 * menu_controller.brightness_value) / 100), 200 - 1);
			R61529_FillRect(GRAY, 11 + ((298 * menu_controller.brightness_value) / 100), 180 + 1, 309, 200 - 1);
			break;
	}
}

/**
  * @brief Delete given sub-menu
  */
void delete_submenu(uint8_t index){
	switch(index){
		case 0: //TODO: MPG mode
			R61529_WriteString(10, 10, "Hello from MPG submenu!", Font_16x26, BLACK, BLACK);
			R61529_FillRect(BLACK, 410, 0, 480, 35);
			break;
		case 1: //TODO: MPG settings
			R61529_WriteString(10, 10, "Hello from MPG settings!", Font_16x26, BLACK, BLACK);
			R61529_FillRect(BLACK, 410, 0, 480, 35);
			break;
		case 2:
			R61529_FillRect(BLACK, 10, 150, 360, 200);
			R61529_FillRect(BLACK, 410, 0, 480, 35);
			break;
	}
}

/**
  * @brief Select a given sub-menu
  */
void select_submenu(uint8_t index){
	if(index > (OPTIONS - 1)){
		return;
	}
	R61529_WriteString(10, 50 + (index * 50), main_menu_options[index], Font_16x26, BLUE, WHITE);
}

/**
  * @brief Un-select a given sub-menu
  */
void unselect_submenu(uint8_t index){
	if(index > (OPTIONS - 1)){
		return;
	}
	R61529_WriteString(10, 50 + (index * 50), main_menu_options[index], Font_16x26, GREEN, BLACK);
}
