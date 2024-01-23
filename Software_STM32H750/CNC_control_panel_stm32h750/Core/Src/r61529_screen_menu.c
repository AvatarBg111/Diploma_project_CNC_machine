/*
 * screen_menu_r61529.c
 *
 *  Created on: Oct 17, 2023
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include "r61529_screen_menu.h"
#include "r61529.h"
#include "systick_timer.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct{
	bool inside_main_menu;
	bool main_menu_drawn;
	bool sub_menu_drawn;

	uint8_t menu_flags;
	int8_t submenu_num;
	uint8_t submenu_flags;
}_menu_controller;


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
_menu_controller menu_controller = {0};


/* Private function prototypes -----------------------------------------------*/
void main_menu(void);
void submenu(void);
void process_user_input(void);
void r61529_screen_menu(void);
void draw_loading_screen(void);


/* Private user code -----------------------------------------------*/
/**
  * @brief Main menu
  */
void main_menu(void){
	if(menu_controller.inside_main_menu && !menu_controller.main_menu_drawn){
		R61529_FillScreen(BLACK);

		for(uint8_t i = 0; i < OPTIONS; i++){
			R61529_WriteString(10, 50 + (i * 50), main_menu_options[i], Font_16x26, GREEN, BLACK);
		}

		menu_controller.main_menu_drawn = true;
	}
}

/**
  * @brief Submenu selector
  */
void submenu(void){
	switch(menu_controller.submenu_num){
		case 1:
			break;
		case 2:
			break;
	}
}

/**
  * @brief Process user input
  */
void process_user_input(void){
}

/**
  * @brief Function that implements graphical menu of the system
  */
void r61529_screen_menu(void){
	if(wait_ms_ch(0, 20)){
		process_user_input();
	}

	if(menu_controller.inside_main_menu){
		main_menu();
	}else{
		submenu();
	}
}

/**
  * @brief Draw loading screen
  */
void draw_loading_screen(void){
	R61529_FillScreen(BLACK);
	R61529_WriteString(170, 145, "TITAN CNC", Font_16x26, BLACK, CYAN);
	menu_controller.inside_main_menu = true;
}
