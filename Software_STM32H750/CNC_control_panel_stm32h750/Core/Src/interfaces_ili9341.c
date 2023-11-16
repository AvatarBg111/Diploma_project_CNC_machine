// Includes
#include "interfaces_ili9341.h"
#include "ili9341.h"
#include "systick_timer.h"
#include "buttons.h"
#include "grbl_cpu_comm.h"


// Private macros and typedefs
#define BUT_UP_CHANNEL 0
#define BUT_DOWN_CHANNEL 1
#define BUT_RIGHT_CHANNEL 2
#define BUT_LEFT_CHANNEL 3
#define BUT_OP_SEL_CHANNEL 4
#define BUT_CYC_ST_CHANNEL 5


// Private variables
/** Positions and options */
int8_t main_menu_pos = 0;
int8_t prev_main_menu_pos = -1;
uint8_t selected_submenu = 0;
uint8_t submenu1_state = 0x00;

/** Main screen menu options */
static const char main_menu_option_1_str[] = "MPG mode";
static const char main_menu_option_2_str[] = "Option 2";
static const char main_menu_option_3_str[] = "Option 3";
static const char main_menu_option_4_str[] = "Option 4";
static const char submenu1_str1[] = "Connecting MPG...";
static const char submenu1_str2_1[] = "MPG could not connect";
static const char submenu1_str2_2[] = "Press button \"Cycle Start\" to exit";
static const char submenu1_str3[] = "MPG enabled";
static const char submenu1_str4[] = "X: 0.000";
static const char submenu1_str5[] = "Y: 0.000";
static const char submenu1_str6[] = "Z: 0.000";
static const char submenu1_str7_1[] = "Could not turn off MPG!";
static const char submenu1_str7_2[] = "Try again";


// Private function definitions
/**
  * @brief Turn on LEDs on control panel
  */
void turn_on_leds(void){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);
}

/**
  * @brief Turn off LEDs on control panel
  */
void turn_off_leds(void){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
}

/**
  * @brief Set button channels
  */
void set_button_channels(){
	if(set_button_channel(BUT_UP_GPIO_Port, BUT_UP_Pin, BUT_UP_CHANNEL) != HAL_OK){
		Error_Handler();
	}
	if(set_button_channel(BUT_DOWN_GPIO_Port, BUT_DOWN_Pin, BUT_DOWN_CHANNEL) != HAL_OK){
		Error_Handler();
	}
	if(set_button_channel(BUT_RIGHT_GPIO_Port, BUT_RIGHT_Pin, BUT_RIGHT_CHANNEL) != HAL_OK){
		Error_Handler();
	}
	if(set_button_channel(BUT_LEFT_GPIO_Port, BUT_LEFT_Pin, BUT_LEFT_CHANNEL) != HAL_OK){
		Error_Handler();
	}
	if(set_button_channel(BUT_OPTION_SELECT_GPIO_Port, BUT_OPTION_SELECT_Pin, BUT_OP_SEL_CHANNEL) != HAL_OK){
		Error_Handler();
	}
	if(set_button_channel(BUT_CYCLE_START_GPIO_Port, BUT_CYCLE_START_Pin, BUT_CYC_ST_CHANNEL) != HAL_OK){
		Error_Handler();
	}
}

/**
  * @brief Check button statuses
  */
static void check_button_statuses(){
	static GPIO_PinState active_level = GPIO_PIN_RESET;

	// Check whether in main menu or in a submenu
	if(main_menu_pos != -1){
		if(get_button_state(BUT_UP_CHANNEL) == active_level){		//HAL_GPIO_ReadPin(BUT_UP_GPIO_Port, BUT_UP_Pin)
			if(--main_menu_pos < 0){
				main_menu_pos = 3;
			}
		}else if(get_button_state(BUT_DOWN_CHANNEL) == active_level){	//HAL_GPIO_ReadPin(BUT_DOWN_GPIO_Port, BUT_DOWN_Pin)
			if(++main_menu_pos > 3){
				main_menu_pos = 0;
			}
		}else if(get_button_state(BUT_OP_SEL_CHANNEL) == active_level){		//HAL_GPIO_ReadPin(BUT_OPTION_SELECT_GPIO_Port, BUT_OPTION_SELECT_Pin)
			selected_submenu = main_menu_pos;
			main_menu_pos = -1;
			prev_main_menu_pos = -1;
		}
	}else{
		uint8_t button_states = 0;

		if(get_button_state(BUT_CYC_ST_CHANNEL) == active_level){	//HAL_GPIO_ReadPin(BUT_CYCLE_START_GPIO_Port, BUT_CYCLE_START_Pin)
			button_states |= 0x01;
		}

		if(button_states & 0x01){
			switch(selected_submenu){
				case 0:
					if(submenu1_state != 0x04 && disable_mpg() == HAL_OK){
						main_menu_pos = 0;
						selected_submenu = 0;
						prev_main_menu_pos = -1;
						submenu1_state = 0x00;
					}else{
						main_menu_pos = -1;
					}
					break;
				default:
					main_menu_pos = 0;
					break;
			}
		}
	}
}

/**
  * @brief Exit sub-menu
  */
static void unhighlight_submenu(uint8_t pos){
	switch(pos){
		case 0:
			ILI9341_WriteString(5, 5, main_menu_option_1_str, Font_16x26, ILI9341_WHITE, ILI9341_BLACK);
			break;
		case 1:
			ILI9341_WriteString(5, 35, main_menu_option_2_str, Font_16x26, ILI9341_WHITE, ILI9341_BLACK);
			break;
		case 2:
			ILI9341_WriteString(5, 65, main_menu_option_3_str, Font_16x26, ILI9341_WHITE, ILI9341_BLACK);
			break;
		case 3:
			ILI9341_WriteString(5, 95, main_menu_option_4_str, Font_16x26, ILI9341_WHITE, ILI9341_BLACK);
			break;
		default:
			break;
	}
}

/**
  * @brief Enter sub-menu
  */
static void highlight_submenu(uint8_t pos){
	switch(pos){
		case 0:
			ILI9341_WriteString(5, 5, main_menu_option_1_str, Font_16x26, ILI9341_BLACK, ILI9341_WHITE);
			break;
		case 1:
			ILI9341_WriteString(5, 35, main_menu_option_2_str, Font_16x26, ILI9341_BLACK, ILI9341_WHITE);
			break;
		case 2:
			ILI9341_WriteString(5, 65, main_menu_option_3_str, Font_16x26, ILI9341_BLACK, ILI9341_WHITE);
			break;
		case 3:
			ILI9341_WriteString(5, 95, main_menu_option_4_str, Font_16x26, ILI9341_BLACK, ILI9341_WHITE);
			break;
		default:
			break;
	}
}

/**
  * @brief Function implementing sub-menu 1
  */
static void submenu1(){
	if(submenu1_state == 0x00){
		ILI9341_FillScreen(ILI9341_BLUE);
		ILI9341_WriteString(55, 110, submenu1_str1, Font_11x18, ILI9341_BLACK, ILI9341_BLUE);

		if(enable_mpg() == HAL_OK){
			submenu1_state = 0x01;
		}else{
			submenu1_state = 0x02;
		}
	}else if(submenu1_state == 0x01){
		ILI9341_FillScreen(ILI9341_BLACK);
		ILI9341_WriteString(5, 5, submenu1_str3, Font_7x10, ILI9341_WHITE, ILI9341_BLACK);
		ILI9341_WriteString(5, 50, submenu1_str4, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
		ILI9341_WriteString(5, 70, submenu1_str5, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
		ILI9341_WriteString(5, 90, submenu1_str6, Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
		submenu1_state = 0x03;
	}else if(submenu1_state == 0x02){
		ILI9341_FillScreen(ILI9341_BLUE);
		ILI9341_WriteString(5, 110, submenu1_str2_1, Font_11x18, ILI9341_BLACK, ILI9341_BLUE);
		ILI9341_WriteString(5, 225, submenu1_str2_2, Font_7x10, ILI9341_WHITE, ILI9341_BLACK);
		submenu1_state = 0x04;
	}else if(submenu1_state == 0x03){
	}else if(submenu1_state == 0x04){
	}else if(submenu1_state == 0x05){
		ILI9341_WriteString(5, 195, submenu1_str7_1, Font_11x18, ILI9341_RED, ILI9341_WHITE);
		ILI9341_WriteString(5, 215, submenu1_str7_2, Font_11x18, ILI9341_RED, ILI9341_WHITE);
		submenu1_state = 0x06;
	}else if(submenu1_state == 0x06){
	}
}

/**
  * @brief Sub-menu MUX
  */
static void submenu(uint8_t submenu){
	switch(selected_submenu){
		case 0:
			submenu1();
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
	}
}

/**
  * @brief Show main menu on screen
  */
void main_menu_show(void){
	//Background
	ILI9341_FillScreen(ILI9341_BLACK);

	//Options
	ILI9341_WriteString(5, 5, main_menu_option_1_str, Font_16x26, ILI9341_WHITE, ILI9341_BLACK);
	ILI9341_WriteString(5, 35, main_menu_option_2_str, Font_16x26, ILI9341_WHITE, ILI9341_BLACK);
	ILI9341_WriteString(5, 65, main_menu_option_3_str, Font_16x26, ILI9341_WHITE, ILI9341_BLACK);
	ILI9341_WriteString(5, 95, main_menu_option_4_str, Font_16x26, ILI9341_WHITE, ILI9341_BLACK);

	// Set main menu position
	main_menu_pos = 0;
	prev_main_menu_pos = 0;
}

/**
  * @brief Function for traversing through menu
  * Must be called from a loop in order to refresh interface on certain actions
  */
void menu(void){
	if(wait_ms_ch(5, 125)){
		check_button_statuses();
	}

	if(main_menu_pos >= 0 && main_menu_pos == prev_main_menu_pos){
		return;
	}else if(main_menu_pos == -1){
		if(prev_main_menu_pos == -1){
			uint8_t buf[] = {',', selected_submenu, ','};
			HAL_UART_Transmit(&huart2, buf, 3, 5);
			prev_main_menu_pos = 0;
		}
		submenu(selected_submenu);
	}else if(prev_main_menu_pos == -1){
		main_menu_show();
	}else{
		unhighlight_submenu(prev_main_menu_pos);
		highlight_submenu(main_menu_pos);
		prev_main_menu_pos = main_menu_pos;
	}
}
