/*
 * parser.h
 *
 *  Created on: Sep 19, 2023
 *      Author: AvatarBg111
 */

#ifndef __COMM_PARSER_H
#define __COMM_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
bool mpg_enabled(uint8_t*,uint8_t);
bool mpg_disabled(uint8_t*,uint8_t);


/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __COMM_PARSER_H */
