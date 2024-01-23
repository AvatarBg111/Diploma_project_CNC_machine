/*
 * parser.c
 *
 *  Created on: Sep 19, 2023
 *      Author: AvatarBg111
 */

/* Private includes ----------------------------------------------------------*/
#include "parser.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
bool mpg_enabled(uint8_t*,uint8_t);
bool mpg_disabled(uint8_t*,uint8_t);


/* Private user code ---------------------------------------------------------*/
/**
  * @brief Check if parsed GRBL message constitutes MPG is on
  */
bool mpg_enabled(uint8_t *pckt, uint8_t len){
    for(uint8_t i = 0; i < len - 5; i++){
    	if(pckt[i] == (uint8_t)'M'){
    		if(pckt[i+1] == (uint8_t)'P' && pckt[i+2] == (uint8_t)'G' && pckt[i+3] == (uint8_t)':' && pckt[i+4] == (uint8_t)'1'){
    			return true;
    		}
    	}
    }

    return false;
}

/**
  * @brief Check if parsed GRBL message constitutes MPG is off
  */
bool mpg_disabled(uint8_t *pckt, uint8_t len){
    for(uint8_t i = 0; i < len - 5; i++){
    	if(pckt[i] == (uint8_t)'M'){
    		if(pckt[i+1] == (uint8_t)'P' && pckt[i+2] == (uint8_t)'G' && pckt[i+3] == (uint8_t)':' && pckt[i+4] == (uint8_t)'0'){
    			return true;
    		}
    	}
    }

    return false;
}
