/*
 * screen_menu_r61529.h
 *
 *  Created on: Oct 17, 2023
 *      Author: AvatarBg111
 */

#ifndef __R61529_SCREEN_MENU_H
#define __R61529_SCREEN_MENU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "ft5436.h"


/* Private defines -----------------------------------------------------------*/
#define LEVELS 4
#define BUTTONS 6
#define ENTER_MENU_DELAY_TIME 50
#define DEFAULT_DELAY 200


/* Exported types ------------------------------------------------------------*/
typedef struct{
	uint8_t level;
	uint16_t level_indexes[LEVELS];

	uint8_t menu_cnt;
	bool enter_menu;

	uint8_t button_delay;
	uint8_t button_delay_cnts[BUTTONS];

	uint8_t touch_point_cnt;
	int16_t touch_points[FT_REG_NUMTOUCHES][2];

	int8_t brightness_value;
	bool entered_manual_mode;
}menu_controller_t;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void r61529_screen_menu(void);
void init_button_inputs(void);
void draw_loading_screen(void);


#ifdef __cplusplus
}
#endif

#endif /* __R61529_SCREEN_MENU_H */
