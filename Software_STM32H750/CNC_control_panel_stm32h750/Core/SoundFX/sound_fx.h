/*
 * sound_fx.h
 *
 *  Created on: Dec 29, 2023
 *      Author: AvatarBg111
 */

#ifndef __SOUNDFX_SOUND_FX_H
#define __SOUNDFX_SOUND_FX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"


/* Private defines -----------------------------------------------------------*/
#if(0)
#define INCLUDE_DOOM
#endif


/* Exported types ------------------------------------------------------------*/
extern TIM_HandleTypeDef htim4;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void enable_buzzer(void);
void disable_buzzer(void);
uint8_t buzzer_short_ring(uint16_t,uint16_t);
#ifdef INCLUDE_DOOM
void DOOM(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SOUNDFX_SOUND_FX_H */
