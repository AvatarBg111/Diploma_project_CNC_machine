/*
 * screen_menu_r61529.c
 *
 *  Created on: Oct 17, 2023
 *      Author: AvatarBg111
 */

// Includes
#include "r61529.h"
#include "ft5436.h"
#include "systick_timer.h"


// Private macros and typedefs
#define MAIN_MENU_SELECT_FLAG_MASK 0x01
#define MAIN_MENU_SHOW_FLAG_MASK 0x02
#define SUB_MENU_SELECT_FLAG_MASK 0x01
#define SUB_MENU_SHOW_FLAG_MASK 0x02
#define SUB_MENU_NUM_OFFSET 0x02	// Only 6 bits are used for the number of the selected sub-menu, meaning there can maximum 63 sub-menus


// Private variables
//static uint8_t submenu = 0x00;
static uint8_t menu_flags = 0x00;

/** Main menu option texts */
/*
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
*/

static const char main_menu_option_1[] = "Go to main screen";
static const char main_menu_option_2[] = "Enter MPG mode";
static const char main_menu_option_3[] = "Settings";
static const char main_menu_option_4[] = "Run screen test sequence";


// Private function definitions
/**
  * @brief Main menu for R61529
  */
static void main_menu_r61529(void){
	if(!(menu_flags & MAIN_MENU_SHOW_FLAG_MASK)){
		R61529_FillScreen(BLACK);

		R61529_WriteString(10, 20, main_menu_option_1, Font_16x26, WHITE, BLACK);
		R61529_WriteString(10, 50, main_menu_option_2, Font_16x26, WHITE, BLACK);
		R61529_WriteString(10, 80, main_menu_option_3, Font_16x26, WHITE, BLACK);
		R61529_WriteString(10, 110, main_menu_option_4, Font_16x26, WHITE, BLACK);
	}
}

/**
  * @brief Sub-menu MUX for R61529
  */
static void submenu_r61529(void){
}

/**
  * @brief Service the inputs made by the user
  */
void service_menu_input_r61529(void){
}

/**
  * @brief Screen menu using R61529 screen controller and FT5436 touch screen controller
  */
void screen_menu_r61529(void){
	if(wait_ms_ch(2, 8)){
		service_menu_input_r61529();
	}

	if(menu_flags & MAIN_MENU_SHOW_FLAG_MASK){
		main_menu_r61529();
	}else{
		submenu_r61529();
	}
}
