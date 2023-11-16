/*
 * ft5436.h
 *
 *  Created on: Oct 12, 2023
 *      Author: AvatarBg111
 */

#ifndef FT5436_FT5436_H_
#define FT5436_FT5436_H_

// Includes
#include "main.h"
#include "stm32h7xx_hal.h"


// Exported macros and typedefs
/** FocalTech I2C Address */
#define FT_ADDR 0x38            // I2C address

// Register addresses
#define FT_REG_NUMTOUCHES 0x05  // Number of touch points
#define FT_REG_MODE 0x00        // Device mode, either WORKING or FACTORY
#define FT_REG_CALIBRATE 0x02   // Calibrate mode
#define FT_REG_WORKMODE 0x00    // Work mode
#define FT_REG_FACTORYMODE 0x40 // Factory mode
#define FT_REG_THRESHHOLD 0x80  // Threshold for touch detection
#define FT_REG_POINTRATE 0x88   // Point rate
#define FT_REG_FIRMVERS 0xA6    // Firmware version
#define FT_REG_CHIPID 0xA3      // Chip selecting
#define FT_REG_VENDID 0xA8      // FocalTech's panel ID

/** FocalTech ID's */
#define FT6234_VENVID 0x79  // FocalTech's panel ID
#define FT6236_VENDID 0x11  // FocalTech's panel ID
#define FT5436_VENDID 0x79  // FocalTech's panel ID
#define FT6206_CHIPID 0x06  // FT6206 ID
#define FT6234_CHIPID 0x54  // FT6234 ID
#define FT6236_CHIPID 0x36  // FT6236 ID
#define FT5436_CHIPID 0x54  // FT5436 ID
#define FT6236U_CHIPID 0x64 // FT6236U ID

#define FT5436_RESET_ACTIVE HAL_GPIO_WritePin(FT5436_RESET_GPIO_Port, FT5436_RESET_Pin, GPIO_PIN_RESET);
#define FT5436_RESET_IDLE HAL_GPIO_WritePin(FT5436_RESET_GPIO_Port, FT5436_RESET_Pin, GPIO_PIN_SET);

/** Touch chip macros */
#define POINT_CNT_SENT_MASK 0x01
#define POINT_ARB_RECEIVED_MASK 0x02	//some arbitration phase
#define POINT_DATA_REQUEST_SENT_MASK 0x04
#define POINT_DATA_RECEIVED_MASK 0x08

// External variables
//extern I2C_HandleTypeDef hi2c4;
extern I2C_HandleTypeDef hi2c3;
extern uint8_t touch_detected;


// Exported variables
extern uint16_t touchX[FT_REG_NUMTOUCHES];
extern uint16_t touchY[FT_REG_NUMTOUCHES];


// Exported function declarations
void Init_FT5436(void);
void touch_move(void);
void request_available_points(void);
void transfer_complete_callback(DMA_HandleTypeDef*);
void reception_complete_callback(DMA_HandleTypeDef*);

#endif /* FT5436_FT5436_H_ */
