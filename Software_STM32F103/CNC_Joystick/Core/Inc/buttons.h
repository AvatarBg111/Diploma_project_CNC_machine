/*
 * buttons.h
 *
 *  Created on: Sep 18, 2023
 *      Author: AvatarBg111
 */

#ifndef __BUTTONS_H
#define __BUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void update_button_status(void);
void update_encoder_status(void);
void change_update_time(uint8_t, uint8_t);
HAL_StatusTypeDef set_button_channel(GPIO_TypeDef*, uint16_t, uint8_t);
GPIO_PinState get_button_state(uint8_t);


/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __BUTTONS_H */
