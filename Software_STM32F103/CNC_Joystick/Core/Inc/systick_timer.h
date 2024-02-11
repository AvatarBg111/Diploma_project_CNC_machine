#ifndef __SYSTICK_TIMER_H
#define __SYSTICK_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "main.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
uint32_t GetSysTickTimer_ms(void);
void delay_ms(uint32_t);
void reset_wait_ms_ch(uint8_t);
uint8_t wait_ms_ch(uint8_t, uint32_t);


/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SYSTICK_TIMER_H */

/************************ (C) COPYRIGHT GreenBo ****************END OF FILE****/
