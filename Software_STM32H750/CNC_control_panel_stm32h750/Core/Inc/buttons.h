/*
 * buttons.h
 *
 *  Created on: Sep 18, 2023
 *      Author: AvatarBg111
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

// Includes
#include "main.h"


// Exported macros and typedefs


// Exported variables


// Exported function declarations
/**
  * @brief Updates button statuses
  * Must be called from interrupt handler of a TIMER
  */
void update_button_status(void);

/**
  * @brief Change button status update time
  * time_high - the time needed for a button to be high to change it's status to activated
  * time_low - the time needed for a button to be low to change it's status to deactivated
  */
void change_update_time(uint8_t, uint8_t);

/**
  * @brief Set button pin association with a channel
  */
HAL_StatusTypeDef set_button_channel(GPIO_TypeDef*, uint16_t, uint8_t);

/**
  * @brief Get button status
  */
GPIO_PinState get_button_state(uint8_t);

#endif /* INC_BUTTONS_H_ */
