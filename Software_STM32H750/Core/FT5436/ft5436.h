/*
 * ft5436.h
 *
 *  Created on: Oct 12, 2023
 *      Author: AvatarBg111
 */

#ifndef _FT5436_H_
#define _FT5436_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_hal.h"


/* Private defines -----------------------------------------------------------*/
#define FT_REG_NUMTOUCHES 0x05  // Number of touch points


/* Exported types ------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c3;
extern uint8_t touch_detected;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Init_FT5436(void);
void request_available_points(void);
void transmission_complete_callback(DMA_HandleTypeDef*);
void reception_complete_callback(DMA_HandleTypeDef*);


#endif /* _FT5436_H_ */
