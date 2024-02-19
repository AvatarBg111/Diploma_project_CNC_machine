/*
 * sender.h
 *
 *  Created on: Feb 8, 2024
 *      Author: AvatarBg111
 */

#ifndef __GRBLHALCOMM_SENDER_H
#define __GRBLHALCOMM_SENDER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"


/* Private defines -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
uint8_t enable_mpg(void);
uint8_t disable_mpg(void);
uint8_t grbl_send_packet(uint8_t*,uint16_t);
bool request_report(void);

#ifdef __cplusplus
}
#endif

#endif /* __GRBLHALCOMM_SENDER_H */
