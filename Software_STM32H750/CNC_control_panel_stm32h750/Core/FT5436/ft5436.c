/*
 * ft5436.c
 *
 *  Created on: Oct 12, 2023
 *      Author: AvatarBg111
 */

// Includes
#include "ft5436.h"
#include "stm32h7xx_hal.h"
#include "r61529.h"
#include "systick_timer.h"
#include<stdio.h>
#include<string.h>


// Private macros and typedefs


// Private variables
uint8_t FT5436_rx_arr[50] = {0};
uint8_t FT5436_tx_arr[50] = {0};

uint8_t ft5436_flags = 0x00, points_detected = 0x00;
uint16_t touchX[FT_REG_NUMTOUCHES] = {0}, touchY[FT_REG_NUMTOUCHES] = {0}, touchID[FT_REG_NUMTOUCHES] = {0};


// Private function definitions
/**
  * @brief Initialize FT5436 controller
  */
void Init_FT5436(void){
	uint8_t initialization_arr[] = {0x00, 0x00, 0x88, 0x0E};
	uint8_t meta_arr[] = {0xA8, 0xA3, 0x80, 0x80};
	uint8_t panel_id = 0x00, chip_id = 0x00;
	uint8_t counter = 25;
	char screen_text[50] = {0};

	while(counter--){
		FT5436_RESET_ACTIVE;
		HAL_Delay(200);
		FT5436_RESET_IDLE;
		HAL_Delay(200);
		FT5436_RESET_IDLE;

		if(HAL_I2C_Master_Transmit(&hi2c3, 0x38 << 1, initialization_arr, 4, 10) == HAL_OK){
			break;
		}
	}

	// Get panel ID if FT module is a panel
	if(HAL_I2C_Master_Transmit(&hi2c3, 0x38 << 1, meta_arr, 1, 25) != HAL_OK){
		Error_Handler();
	}
	HAL_I2C_Master_Receive(&hi2c3, 0x38 << 1, &panel_id, 1, 20);

	// Get chip ID if FT is a chip
	if(HAL_I2C_Master_Transmit(&hi2c3, 0x38 << 1, meta_arr + 1, 1, 25) != HAL_OK){
		Error_Handler();
	}
	HAL_I2C_Master_Receive(&hi2c3, 0x38 << 1, &chip_id, 1, 20);

	// Format output string and draw it on screen
	sprintf(screen_text, "Panel Id: 0x%X  Chip Id: 0x%X", panel_id, chip_id);
	R61529_WriteString(5, 300, screen_text, Font_7x10, GREEN, BLACK);

	// Set touch detection threshold
	if(HAL_I2C_Master_Transmit(&hi2c3, 0x38 << 1, meta_arr + 2, 2, 25) != HAL_OK){
		Error_Handler();
	}
}

/**
  * @brief Send request for touch data and receive it
  */
void touch_move(void){
	uint8_t meta_arr[] = {FT_REG_NUMTOUCHES, 0x00};
	uint8_t res_arr[3 + (5 * 6)] = {0x00};

	if(touch_detected){
		touch_detected = 0x00;

		if(HAL_I2C_Master_Transmit(&hi2c3, 0x38 << 1, meta_arr, 1, 30) != HAL_OK){
			Error_Handler();
		}
		HAL_I2C_Master_Receive(&hi2c3, 0x38 << 1, res_arr, 1, 5);

		if(res_arr[0] > FT_REG_NUMTOUCHES){
			return;
		}else{
			if(HAL_I2C_Master_Transmit(&hi2c3, 0x38 << 1, meta_arr + 1, 1, 30) != HAL_OK){
				Error_Handler();
			}
			HAL_I2C_Master_Receive(&hi2c3, 0x38 << 1, res_arr, 33, 10);

			if(!((res_arr[2] > FT_REG_NUMTOUCHES) || (res_arr[2] == 0))){
			    for(uint8_t i = 0; i < res_arr[2]; i++){
			        touchX[i] = res_arr[0x03 + i * 6] & 0x0F;
			        touchX[i] <<= 8;
			        touchX[i] |= res_arr[0x04 + i * 6];
			        touchY[i] = res_arr[0x05 + i * 6] & 0x0F;
			        touchY[i] <<= 8;
			        touchY[i] |= res_arr[0x06 + i * 6];
			        touchID[i] = res_arr[0x05 + i * 6] >> 4;
			    }
			}
		}
	}
}

/**
  * @brief Request available points
  */
void request_available_points(void){
	if(touch_detected && ft5436_flags == 0x00){
		FT5436_tx_arr[0] = FT_REG_NUMTOUCHES;

		// Wait for I2C to switch to READY state
		while(__HAL_I2C_GET_FLAG(&hi2c3, I2C_FLAG_BUSY) == SET){
			if(wait_ms_ch(1, 2)){
				ft5436_flags = 0x00;
				points_detected = 0x00;
				return;
			}
		}
		reset_wait_ms_ch(1);

		if(HAL_I2C_Master_Transmit_DMA(&hi2c3, 0x38 << 1, FT5436_tx_arr, 1) != HAL_OK){
			Error_Handler();
		}

		ft5436_flags |= POINT_CNT_SENT_MASK;
	}
}

void transfer_complete_callback(DMA_HandleTypeDef *hdma){
	if(hdma->Instance == DMA1_Stream1){
		if(ft5436_flags & POINT_CNT_SENT_MASK){
			FT5436_tx_arr[0] = 0x00;

			// Wait for I2C to switch to READY state
			while(__HAL_I2C_GET_FLAG(&hi2c3, I2C_FLAG_BUSY) == SET){
				if(wait_ms_ch(1, 2)){
					ft5436_flags = 0x00;
					points_detected = 0x00;
					return;
				}
			}
			reset_wait_ms_ch(1);

			if(HAL_I2C_Master_Receive_DMA(&hi2c3, 0x38 << 1, FT5436_rx_arr, 1) != HAL_OK){
				Error_Handler();
			}
		}else if(ft5436_flags & POINT_ARB_RECEIVED_MASK){
			ft5436_flags |= POINT_DATA_REQUEST_SENT_MASK;
			ft5436_flags &= ~POINT_ARB_RECEIVED_MASK;

			// Wait for I2C to switch to READY state
			while(__HAL_I2C_GET_FLAG(&hi2c3, I2C_FLAG_BUSY) == SET){
				if(wait_ms_ch(1, 2)){
					ft5436_flags = 0x00;
					points_detected = 0x00;
					return;
				}
			}
			reset_wait_ms_ch(1);

			if(HAL_I2C_Master_Receive_DMA(&hi2c3, 0x38 << 1, FT5436_rx_arr, 3 + (5 * 6)) != HAL_OK){
				Error_Handler();
			}
		}
	}
}

void reception_complete_callback(DMA_HandleTypeDef *hdma){
	if(hdma->Instance == DMA1_Stream0){
		if(ft5436_flags & POINT_CNT_SENT_MASK){
			if(FT5436_rx_arr[0] > FT_REG_NUMTOUCHES){
				ft5436_flags = 0x00;
				points_detected = 0x00;
			}else{
				ft5436_flags |= POINT_ARB_RECEIVED_MASK;
				ft5436_flags &= ~POINT_CNT_SENT_MASK;
				FT5436_rx_arr[0] = 0x00;

				// Wait for I2C to switch to READY state
				while(__HAL_I2C_GET_FLAG(&hi2c3, I2C_FLAG_BUSY) == SET){
					if(wait_ms_ch(1, 2)){
						ft5436_flags = 0x00;
						points_detected = 0x00;
						return;
					}
				}
				reset_wait_ms_ch(1);

				if(HAL_I2C_Master_Transmit_DMA(&hi2c3, 0x38 << 1, FT5436_tx_arr, 1) != HAL_OK){
					Error_Handler();
				}
			}
		}else if(ft5436_flags & POINT_DATA_REQUEST_SENT_MASK){
			if(!( (FT5436_rx_arr[2] > FT_REG_NUMTOUCHES) || (FT5436_rx_arr[2] == 0) )){
			    points_detected = FT5436_rx_arr[2];
			    ft5436_flags &= ~POINT_DATA_RECEIVED_MASK;

			    for(uint8_t i = 0; i < points_detected; i++){
			        touchX[i] = FT5436_rx_arr[3 + (i * 6)] & 0x0F;
			        touchX[i] <<= 8;
			        touchX[i] |= FT5436_rx_arr[4 + (i * 6)];
			        touchY[i] = FT5436_rx_arr[5 + (i * 6)] & 0x0F;
			        touchY[i] <<= 8;
			        touchY[i] |= FT5436_rx_arr[6 + (i * 6)];
			        touchID[i] = FT5436_rx_arr[5 + (i * 6)] >> 4;
			    }

			    ft5436_flags |= POINT_DATA_RECEIVED_MASK;
			}else{
				points_detected = 0x00;
			}

			ft5436_flags = 0x00;
			touch_detected = 0x00;
		}
	}
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
    if(hi2c->Instance == hi2c3.Instance){
        HAL_DMA_Abort_IT(hi2c->hdmatx);
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
    if(hi2c->Instance == hi2c3.Instance){
        HAL_DMA_Abort_IT(hi2c->hdmarx);
    }
}
