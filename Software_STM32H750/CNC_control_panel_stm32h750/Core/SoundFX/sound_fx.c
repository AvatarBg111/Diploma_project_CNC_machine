/*
 * sound_fx.c
 *
 *  Created on: Dec 29, 2023
 *      Author: AvatarBg111
 */

/* Includes ------------------------------------------------------------------*/
#include "sound_fx.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
bool buzzer_enabled = false;


/* Private function prototypes -----------------------------------------------*/
void enable_buzzer(void);
void disable_buzzer(void);
uint8_t buzzer_short_ring(uint16_t, uint16_t);


/* Private user code ---------------------------------------------------------*/
void enable_buzzer(void){
	buzzer_enabled = true;
}

void disable_buzzer(void){
	// Enable Buzzer PWM channel
	buzzer_enabled = false;
	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
	__HAL_TIM_SET_COUNTER(&htim4, 0);
}

/**
 * Ring buzzer for a short period of time
 * uint16_t freq - frequency ranging from 1000Hz to 10000Hz
 * uint16_t ms - time in milliseconds ranging from 1ms to 1000ms
 *
 * freq - desired frequency
 * seconds / freq = X
 * Y - desired time in milliseconds
 * Z - update events of the timer
 *
 * (Y / 1000) / X = Z
 * Y / (1000 . X) = Z
 * Y / (1000 . (sec / freq)) = Z
 * (10 . Y) / (10000 . (sec / freq)) = Z
 * (10 . Y) / (10000 / freq) = Z
 * (10 . Y . freq) / 10000 = Z
 */
uint8_t buzzer_short_ring(uint16_t freq, uint16_t ms){
	uint32_t base_freq;
	uint16_t update_events;

	if(freq < 1000 || freq > 10000 || ms < 50 || ms > 1000 || buzzer_enabled == false){
		return 1;
	}

	base_freq = HAL_RCC_GetHCLKFreq() / (htim4.Init.Prescaler + 1);
	htim4.Instance->ARR = (uint16_t)(base_freq / freq);
	htim4.Instance->CCR3 = (uint16_t)(htim4.Instance->ARR / 2);

	update_events = (10 * ms * freq) / 10000;

	if(turn_on_buzzer(update_events) == false){
		return 2;
	}

	return 0;
}
