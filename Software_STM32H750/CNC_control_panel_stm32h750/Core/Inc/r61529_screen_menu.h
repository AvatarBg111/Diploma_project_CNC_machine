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


/* Private defines -----------------------------------------------------------*/
#define OPTIONS 2


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
static const char *main_menu_options[OPTIONS] = {
	"Enter MPG mode",
	"Settings",
};


/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void draw_loading_screen(void);
void r61529_screen_menu(void);


#ifdef __cplusplus
}
#endif

#endif /* __R61529_SCREEN_MENU_H */
