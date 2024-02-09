/*
 * buttons.c
 *
 *  Created on: Sep 18, 2023
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include "buttons.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct button{
	GPIO_TypeDef *port;
	uint16_t pin;
}button_t;


/* Private define ------------------------------------------------------------*/
#define BUTTON_CHANNELS 16
#define UPDATE_TIME_LOW_TO_HIGH_DEFAULT 2		//TIM1 has 120Hz frequency -> e.g. 2 updates are equal to ~17ms
#define UPDATE_TIME_HIGH_TO_LOW_DEFAULT 2		//TIM1 has 120Hz frequency -> e.g. 2 updates are equal to ~17ms


/* Private macro -------------------------------------------------------------*/


/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;


/* Private variables ---------------------------------------------------------*/
button_t button_matrix[BUTTON_CHANNELS] = {0};
GPIO_PinState but_stat_channels[BUTTON_CHANNELS] = {0};
uint32_t but_stat_cnts[BUTTON_CHANNELS] = {0};
uint8_t update_time_high_to_low = UPDATE_TIME_HIGH_TO_LOW_DEFAULT;
uint8_t update_time_low_to_high = UPDATE_TIME_LOW_TO_HIGH_DEFAULT;
uint8_t update_time;
bool startup = true;


/* Private function prototypes -----------------------------------------------*/
void update_button_status(void);
void change_update_time(uint8_t, uint8_t);
void clear_button_statuses(void);
HAL_StatusTypeDef set_button_channel(GPIO_TypeDef*, uint16_t, uint8_t);
GPIO_PinState get_button_state(uint8_t);


/* Private user code ---------------------------------------------------------*/
/**
  * @brief Updates button statuses
  * Must be called from interrupt handler of a TIMER
  */
void update_button_status(void){
	for(uint8_t i = 0; i < BUTTON_CHANNELS; i++){
		if(button_matrix[i].port == NULL){
			continue;
		}else if(HAL_GPIO_ReadPin(button_matrix[i].port, button_matrix[i].pin) != but_stat_channels[i]){
			if(++but_stat_cnts[i] >= update_time){
				but_stat_cnts[i] = 0;

				if(but_stat_channels[i] == GPIO_PIN_RESET){
					but_stat_channels[i] = GPIO_PIN_SET;
					update_time = update_time_high_to_low;
				}else{
					but_stat_channels[i] = GPIO_PIN_RESET;
					update_time = update_time_low_to_high;
				}
			}
		}else{
			if(but_stat_cnts[i] > 0){
				but_stat_cnts[i]--;
			}
		}
	}

	return;
}

/**
  * @brief Change button status update time
  * time_high - the time needed for a button to be high to change it's status to activated
  * time_low - the time needed for a button to be low to change it's status to deactivated
  * Check UPDATE_TIME_LOW_TO_HIGH_DEFAULT and UPDATE_TIME_HIGH_TO_LOW_DEFAULT defines
  * to understand what parameters to send to this function
  */
void change_update_time(uint8_t time_high, uint8_t time_low){
	update_time_low_to_high = time_high;
	update_time_high_to_low = time_low;
}

/**
  * @brief Clear the buttons statuses
  */
void clear_button_statuses(void){
	for(uint8_t i = 0; i < BUTTON_CHANNELS; i++){
		button_matrix[i].port = NULL;
		button_matrix[i].pin = 0;
		but_stat_channels[i] = GPIO_PIN_RESET;
	}
}

/**
  * @brief Set button pin association with a channel
  */
HAL_StatusTypeDef set_button_channel(GPIO_TypeDef *port, uint16_t pin, uint8_t channel){
	if(startup == true){
		startup = false;
		clear_button_statuses();
	}

	if(channel < BUTTON_CHANNELS && port != NULL){
		button_matrix[channel].port = port;
		button_matrix[channel].pin = pin;
		return HAL_OK;
	}

	return HAL_ERROR;
}

/**
  * @brief Get button status
  */
GPIO_PinState get_button_state(uint8_t channel){
	return but_stat_channels[channel];
}
